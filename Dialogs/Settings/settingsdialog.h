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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QProcess>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "Dialogs/UserAdmin/useramindialog.h"
#include "Dialogs/ChangePassword/changepassworddialog.h"
#include "Dialogs/Info/infodialog.h"

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Libraries/ServiceCore/servicecore.h"
#include "Libraries/MyCrypto/mycrypto.h"

#include "Classes/User/user.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr, User *user = nullptr);
    ~SettingsDialog();

private slots:
    void on_pushButton_user_admin_clicked();
    void on_pushButton_change_password_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_generate_extensions_clicked();
    void on_pushButton_get_backup_path_clicked();
    void on_pushButton_run_backup_clicked();
    void on_pushButton_add_ip_clicked();
    void on_listWidget_ip_list_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::SettingsDialog *ui;
    DatabaseConnector connector;
    ServiceCore serviceCore;
    QSettings settings;
    User *currentUser = new User;
    QStringList IPList;

    void SetupDialog();
    void ReadSettings();
    void SaveSettings();

};

#endif // SETTINGSDIALOG_H
