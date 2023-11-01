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

#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent, User *user, bool admin) :
    QDialog(parent),
    ui(new Ui::ChangePasswordDialog)
{
    ui->setupUi(this);
    selectedUser = user;
    adminRights = admin;
    if(adminRights) ui->lineEdit_old_password->setEnabled(false);
    ui->lineEdit_old_password->setFocus();
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}

void ChangePasswordDialog::on_pushButton_save_clicked()
{
    QString oldPassHash = QByteArray(QCryptographicHash::hash(ui->lineEdit_old_password->text().toUtf8(), QCryptographicHash::Sha256)).toHex();
    QString newPassHash = QByteArray(QCryptographicHash::hash(ui->lineEdit_password->text().toUtf8(), QCryptographicHash::Sha256)).toHex();

    if(oldPassHash == selectedUser->password || adminRights)
    {
        if(ui->lineEdit_password->text() == ui->lineEdit_password_confirm->text() && !ui->lineEdit_password->text().isEmpty())
        {
            if(ui->checkBox_two_factor->isChecked())
            {
                selectedUser->token = mytotp.GenerateRandomBase32Token(20);

                QString QRdata = "otpauth://totp/MySqlComputerService-" + selectedUser->login + "?secret=" + selectedUser->token;
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

                StringDialog dialog("Enter 2FA code", QLineEdit::Password);
                dialog.exec();
                if(dialog.accepted && mytotp.GetTOTPFromToken(selectedUser->token) == dialog.text)
                {
                    MyCrypto crypto(crypto.HashString(ui->lineEdit_password->text(), QCryptographicHash::Sha256), QCryptographicHash::Sha256);
                    selectedUser->password = ui->lineEdit_password->text();
                    if(connector.ModifyUser(selectedUser))
                    {
                        settings.setValue(selectedUser->login + "/database_password", crypto.EncryptData(connector.GetDatabasePassword()));
                        passwordChanged = true;
                        this->close();
                    }
                    else ui->lineEdit_old_password->setFocus();
                }
            }
            else
            {
                MyCrypto crypto(crypto.HashString(ui->lineEdit_password->text(), QCryptographicHash::Sha256), QCryptographicHash::Sha256);
                selectedUser->password = ui->lineEdit_password->text();
                selectedUser->token = "";
                if(connector.ModifyUser(selectedUser))
                {
                    settings.setValue(selectedUser->login + "/database_password", crypto.EncryptData(connector.GetDatabasePassword()));
                    passwordChanged = true;
                    this->close();
                }
                else ui->lineEdit_old_password->setFocus();
            }
        }
    }
}
