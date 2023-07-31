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
            selectedUser->password = ui->lineEdit_password->text();
            connector.ModifyUser(selectedUser);
            passwordChanged = true;
            this->close();
        }
    }
}
