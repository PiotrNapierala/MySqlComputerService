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

#include "useramindialog.h"
#include "ui_useramindialog.h"

UserAminDialog::UserAminDialog(QWidget *parent, User *user) :
    QDialog(parent),
    ui(new Ui::UserAminDialog)
{
    ui->setupUi(this);
    currentUser = user;
    SetupDialog();
}

UserAminDialog::~UserAminDialog()
{
    delete ui;
}

void UserAminDialog::SetupDialog()
{
    RefreshDialog();
}

void UserAminDialog::RefreshUsers()
{
    ui->listWidget_users->clear();
    users = connector.ReadUsers();
    QList<User*> usersCopy = users;
    users.clear();
    foreach(User* user, usersCopy) if(user->id != currentUser->id) users << user;
    foreach(User* user, users) ui->listWidget_users->addItem(user->login);
}

void UserAminDialog::RefreshDialog()
{
    RefreshUsers();
    if(userSelected)
    {
        ui->label_selected_user_login->setText(selectedUser->login);
        ui->checkBox_admin->setChecked(selectedUser->admin);
        ui->checkBox_active->setChecked(selectedUser->active);
        ui->checkBox_active->setEnabled(true);
        ui->checkBox_admin->setEnabled(true);
        ui->pushButton_change_password->setEnabled(true);
        ui->pushButton_remove_user->setEnabled(true);
    }
    else
    {
        ui->label_selected_user_login->setText("-");
        ui->checkBox_active->setChecked(false);
        ui->checkBox_admin->setChecked(false);
        ui->checkBox_active->setEnabled(false);
        ui->checkBox_admin->setEnabled(false);
        ui->pushButton_change_password->setEnabled(false);
        ui->pushButton_remove_user->setEnabled(false);
    }
}

void UserAminDialog::on_pushButton_add_user_clicked()
{
    AddUserDialog dialog;
    dialog.exec();
    RefreshDialog();
}

void UserAminDialog::on_listWidget_users_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    selectedUser = users[ui->listWidget_users->currentRow()];
    userSelected = true;
    RefreshDialog();
}

void UserAminDialog::on_checkBox_admin_clicked()
{
    selectedUser->admin = ui->checkBox_admin->isChecked();
    connector.ModifyUser(selectedUser);
    RefreshDialog();
}

void UserAminDialog::on_checkBox_active_clicked()
{
    selectedUser->active = ui->checkBox_active->isChecked();
    connector.ModifyUser(selectedUser);
    RefreshDialog();
}

void UserAminDialog::on_pushButton_remove_user_clicked()
{
    if(userSelected)
    {
        connector.RemoveUser(selectedUser);
        userSelected = false;
        RefreshDialog();
    }
}

void UserAminDialog::on_pushButton_change_password_clicked()
{
    ChangePasswordDialog dialog(this, selectedUser, true);
    dialog.exec();
}

