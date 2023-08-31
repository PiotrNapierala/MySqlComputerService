/*
This file is part of MySqlComputerService.

Copyright (c) 2023 Piotr Napierała
MySqlComputerService is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see http://www.gnu.org/licenses/.

This program is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License
for the specific language governing permissions and limitations
under the License.
*/

#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
	ui->lineEdit_login->setFocus();
    ui->lineEdit_login->setValidator(serviceCore.GetUserValidator(ui->lineEdit_login));
    CheckTempDatabasePassword();
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

void LoginDialog::on_pushButton_login_clicked()
{
    QString userLogin = ui->lineEdit_login->text();
    QString userPassword = ui->lineEdit_password->text();

    MyCrypto crypto(crypto.HashString(userPassword, QCryptographicHash::Sha256), QCryptographicHash::Sha256);
    QString encrpytedPassword = settings.value(userLogin + "/database_password").toString();

    if(encrpytedPassword.isEmpty())
    {
        ui->lineEdit_login->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_login->setFocus();
    }

    QString decryptedPassword = crypto.DecryptData(encrpytedPassword);
    QString databasePasswordHash = settings.value("database/database_password_hash").toString();

    if(crypto.HashString(decryptedPassword, QCryptographicHash::Sha256) == databasePasswordHash)
    {
        if(connector.SetupConnection(decryptedPassword))
        {
            connector.CreateTables();

            while(connector.GetUsersCount() == 0)
            {
                AddUserDialog dialog(nullptr, true);
                dialog.exec();
                if(!dialog.accepted) exit(1);
                else return;
            }

            while(connector.ReadIntGlobalSettings("isSet") == false)
            {
                FirstRunDialog dialog(nullptr, 1);
                dialog.exec();
                if(!dialog.accepted) exit(1);
                else return;
            }

            if(connector.ValidateUser(ui->lineEdit_login->text(), ui->lineEdit_password->text()))
            {
                currentUser = connector.currentUser;

                if(!currentUser->token.isEmpty())
                {
                    StringDialog dialog(tr("Enter 2FA code"), QLineEdit::Password);
                    dialog.exec();
                    if(dialog.accepted)
                    {
                        QString currentTOTP = mytotp.GetTOTPFromToken(currentUser->token);
                        if(currentTOTP == dialog.text)
                        {
                            loginCorrect = true;
                            this->close();
                        }
                        else
                        {
                            ui->lineEdit_login->clear();
                            ui->lineEdit_password->clear();
                            ui->lineEdit_login->setFocus();
                        }
                    }
                    else
                    {
                        ui->lineEdit_login->clear();
                        ui->lineEdit_password->clear();
                        ui->lineEdit_login->setFocus();
                    }
                }
                else
                {
                    loginCorrect = true;
                    this->close();
                }
            }
            else
            {
                ui->lineEdit_login->clear();
                ui->lineEdit_password->clear();
                ui->lineEdit_login->setFocus();
            }
        }
        else
        {
            FirstRunDialog dialog(nullptr, 0);
            dialog.exec();
            if(!dialog.accepted) exit(1);
            else return;
        }
    }
    else
    {
        ui->lineEdit_login->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_login->setFocus();
    }
}

void LoginDialog::CheckTempDatabasePassword()
{
    QString tempDatabasePassword = settings.value("database/temp_database_password").toString();
    if(!tempDatabasePassword.isEmpty())
    {
        if(connector.SetupConnection(tempDatabasePassword))
        {
            connector.CreateTables();

            while(connector.GetUsersCount() == 0)
            {
                AddUserDialog dialog(nullptr, true);
                dialog.exec();
                if(!dialog.accepted) exit(1);
                else settings.setValue("database/temp_database_password", "");
            }
        }
        else
        {
            FirstRunDialog dialog(nullptr, 0);
            dialog.exec();
            if(!dialog.accepted) exit(1);
        }
    }
}
