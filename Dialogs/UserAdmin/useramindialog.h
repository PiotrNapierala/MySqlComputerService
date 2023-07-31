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

#ifndef USERAMINDIALOG_H
#define USERAMINDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "Dialogs/AddUser/adduserdialog.h"
#include "Dialogs/ChangePassword/changepassworddialog.h"

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Classes/User/user.h"

namespace Ui {
class UserAminDialog;
}

class UserAminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserAminDialog(QWidget *parent = nullptr, User* user = nullptr);
    ~UserAminDialog();

private slots:
    void on_pushButton_add_user_clicked();
    void on_listWidget_users_itemDoubleClicked(QListWidgetItem *item);
    void on_checkBox_admin_clicked();
    void on_checkBox_active_clicked();
    void on_pushButton_remove_user_clicked();
    void on_pushButton_change_password_clicked();

private:
    Ui::UserAminDialog *ui;
    DatabaseConnector connector;
    User *currentUser;

    QList<User*> users;
    User *selectedUser;
    bool userSelected = false;

    void SetupDialog();
    void RefreshUsers();
    void RefreshDialog();

};

#endif // USERAMINDIALOG_H
