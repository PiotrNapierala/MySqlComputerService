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
        if(connector.SaveUser(user)) this->close();
    }
}
