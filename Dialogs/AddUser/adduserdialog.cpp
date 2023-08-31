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

#include "adduserdialog.h"
#include "ui_adduserdialog.h"

AddUserDialog::AddUserDialog(QWidget *parent, bool forceAdmin) :
    QDialog(parent),
    ui(new Ui::AddUserDialog)
{
    ui->setupUi(this);
    ui->lineEdit_login->setFocus();
    ui->lineEdit_login->setValidator(serviceCore.GetUserValidator(ui->lineEdit_login));
    if(forceAdmin)
    {
        ui->checkBox_admin->setChecked(true);
        ui->checkBox_admin->setEnabled(false);
    }
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
}

void AddUserDialog::on_pushButton_save_clicked()
{
    accepted = true;
    if(!ui->lineEdit_login->text().isEmpty() && !ui->lineEdit_password->text().isEmpty() && !ui->lineEdit_password_confirm->text().isEmpty() &&
        ui->lineEdit_password->text() == ui->lineEdit_password_confirm->text())
    {
        User *user = new User;
        user->active = true;
        user->admin = ui->checkBox_admin->isChecked();
        user->login = ui->lineEdit_login->text();
        user->password = ui->lineEdit_password->text();

        MyCrypto crypto(crypto.HashString(user->password, QCryptographicHash::Sha256), QCryptographicHash::Sha256);
        if(ui->checkBox_two_factor->isChecked())
        {
            user->token = mytotp.GenerateRandomBase32Token(60);

            QString QRdata = "otpauth://totp/MySqlComputerService-" + user->login + "?secret=" + user->token;
            QPixmap QR = qrconnector.GenerateQR(QRdata);
            QLabel *QRLabel = new QLabel(this);
            QRLabel->setPixmap(QR.scaled(200, 200));

            QDialog QRdialog;
            QHBoxLayout *layout = new QHBoxLayout(&QRdialog);
            layout->setAlignment(Qt::AlignCenter);
            layout->addWidget(QRLabel);

            QRdialog.setLayout(layout);
            QRdialog.setWindowTitle("QR code");
            QRdialog.setFixedSize(QSize(240, 240));
            QRdialog.setWindowIcon(QPixmap(":/PNG/IMG/qr.png"));

            QRdialog.exec();

            StringDialog dialog(tr("Enter 2FA code"), QLineEdit::Password);
            dialog.exec();
            if(dialog.accepted && mytotp.GetTOTPFromToken(user->token) == dialog.text)
            {
                if(connector.SaveUser(user))
                {
                    QString decryptedDatabasePassword = connector.GetDatabasePassword();
                    QString encryptedDatabasePassword = crypto.EncryptData(decryptedDatabasePassword);
                    settings.setValue(user->login + "/database_password", encryptedDatabasePassword);
                    this->close();
                }
            }
        }
        else
        {
            if(connector.SaveUser(user))
            {
                QString decryptedDatabasePassword = connector.GetDatabasePassword();
                QString encryptedDatabasePassword = crypto.EncryptData(decryptedDatabasePassword);
                settings.setValue(user->login + "/database_password", encryptedDatabasePassword);
                this->close();
            }
        }
    }
}
