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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, User *user) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    currentUser = user;
    SetupDialog();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::SetupDialog()
{
    ui->tabWidget->setCurrentIndex(0);

    ui->label_current_user->setText(currentUser->login);
    ui->pushButton_user_admin->setVisible(currentUser->admin);
    ui->tabWidget->setTabVisible(0, currentUser->admin);
    ui->tabWidget->setTabVisible(2, currentUser->admin);
    ui->tabWidget->setTabVisible(3, currentUser->admin);
    ui->tabWidget->setTabVisible(4, currentUser->admin);
    ui->tabWidget->setTabVisible(5, currentUser->admin);
    ui->pushButton_save->setVisible(currentUser->admin);

    ui->lineEdit_database_server_port->setValidator(serviceCore.GetDigitalValidator(ui->lineEdit_database_server_port));
    ui->lineEdit_service_email_server_port->setValidator(serviceCore.GetDigitalValidator(ui->lineEdit_service_email_server_port));
    ui->lineEdit_service_email->setValidator(serviceCore.GetEmailValidator(ui->lineEdit_service_email));
    ui->lineEdit_ip->setValidator(serviceCore.GetIPValidator(ui->lineEdit_ip));
    ui->lineEdit_pin->setValidator(serviceCore.GetDigitalValidator(ui->lineEdit_pin));

    ReadSettings();
}

void SettingsDialog::ReadSettings()
{
    MyCrypto crypto(currentUser->password, QCryptographicHash::Sha256);

    ui->spinBox_logout_time->setValue(settings.value("logout_time").toInt());
    ui->checkBox_autoclose->setChecked(settings.value("autoclose_dialogs").toBool());

    ui->lineEdit_database_server_adress->setText(settings.value("database/database_server_adress").toString());
    ui->lineEdit_database_server_port->setText(settings.value("database/database_server_port").toString());
    ui->lineEdit_database_name->setText(settings.value("database/database_name").toString());
    ui->lineEdit_database_user->setText(settings.value("database/database_user").toString());
    ui->lineEdit_database_password->setText(crypto.DecryptData(settings.value(currentUser->login + "/database_password").toString()));
    ui->lineEdit_backup_path->setText(settings.value("backup/backup_path").toString());

    ui->lineEdit_service_email->setText(connector.ReadStringGlobalSettings("serviceEmail"));
    ui->lineEdit_email_password->setText(connector.ReadStringGlobalSettings("serviceEmailPassword"));
    ui->lineEdit_service_email_server->setText(connector.ReadStringGlobalSettings("serviceEmailServer"));
    ui->lineEdit_service_email_server_port->setText(QString::number(connector.ReadIntGlobalSettings("serviceEmailServerPort")));
    ui->checkBox_enable_email->setChecked(connector.ReadIntGlobalSettings("enableEmail"));

    ui->lineEdit_service_name->setText(connector.ReadStringGlobalSettings("serviceName"));
    ui->lineEdit_service_owner->setText(connector.ReadStringGlobalSettings("serviceOwner"));
    ui->lineEdit_service_adress->setText(connector.ReadStringGlobalSettings("serviceAdress"));
    ui->lineEdit_service_website->setText(connector.ReadStringGlobalSettings("serviceWebsite"));

    ui->checkBox_enable_extensions->setChecked(connector.ReadIntGlobalSettings("enableExtensions"));
    ui->lineEdit_apache_server_adress->setText(connector.ReadStringGlobalSettings("apacheServerAdress"));
    ui->lineEdit_pin->setText(QString::number(connector.ReadIntGlobalSettings("pin")));

    ui->checkBox_url_search->setChecked(settings.value("url_search").toBool());
    ui->spinBox_url_search_port->setValue(settings.value("url_search_port").toInt());

    IPList = connector.ReadDevices();
    ui->listWidget_ip_list->addItems(IPList);

    ui->comboBox_language->setCurrentIndex(settings.value("language").toInt()-1);
}

void SettingsDialog::SaveSettings()
{
    //MyCrypto crypto(currentUser->password, QCryptographicHash::Sha256);

    settings.setValue("logout_time", ui->spinBox_logout_time->value());
    settings.setValue("autoclose_dialogs", ui->checkBox_autoclose->isChecked());

    //settings.setValue("database/database_server_adress", ui->lineEdit_database_server_adress->text());
    //settings.setValue("database/database_server_port", ui->lineEdit_database_server_port->text());
    //settings.setValue("database/database_name", ui->lineEdit_database_name->text());
    //settings.setValue("database/database_user", ui->lineEdit_database_user->text());
    //settings.setValue(currentUser->login + "/database_password", crypto.EncryptData(ui->lineEdit_database_password->text()));
    settings.setValue("backup/backup_path", ui->lineEdit_backup_path->text());

    connector.SaveStringGlobalSettings("serviceEmail", ui->lineEdit_service_email->text());
    connector.SaveStringGlobalSettings("serviceEmailPassword", ui->lineEdit_email_password->text());
    connector.SaveStringGlobalSettings("serviceEmailServer", ui->lineEdit_service_email_server->text());
    connector.SaveIntGlobalSettings("serviceEmailServerPort", ui->lineEdit_service_email_server_port->text().toInt());
    connector.SaveIntGlobalSettings("enableEmail", ui->checkBox_enable_email->isChecked());

    connector.SaveStringGlobalSettings("serviceName", ui->lineEdit_service_name->text());
    connector.SaveStringGlobalSettings("serviceOwner", ui->lineEdit_service_owner->text());
    connector.SaveStringGlobalSettings("serviceAdress", ui->lineEdit_service_adress->text());
    connector.SaveStringGlobalSettings("serviceWebsite", ui->lineEdit_service_website->text());

    connector.SaveIntGlobalSettings("enableExtensions", ui->checkBox_enable_extensions->isChecked());
    connector.SaveStringGlobalSettings("apacheServerAdress", ui->lineEdit_apache_server_adress->text());
    connector.SaveIntGlobalSettings("pin", ui->lineEdit_pin->text().toInt());

    settings.setValue("url_search", ui->checkBox_url_search->isChecked());
    settings.setValue("url_search_port", ui->spinBox_url_search_port->value());

    connector.SaveDevices(IPList);

    if(settings.value("language").toInt()-1 != ui->comboBox_language->currentIndex())
    {
        settings.setValue("language", ui->comboBox_language->currentIndex()+1);
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }

}

void SettingsDialog::on_pushButton_user_admin_clicked()
{
    UserAminDialog dialog(this, currentUser);
    dialog.exec();
}

void SettingsDialog::on_pushButton_change_password_clicked()
{
    ChangePasswordDialog dialog(this, currentUser, false);
    dialog.exec();
    if(dialog.passwordChanged)
    {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void SettingsDialog::on_pushButton_save_clicked()
{
    SaveSettings();
    InfoDialog info(tr("Settings saved"));
    info.exec();
}

void SettingsDialog::on_pushButton_generate_extensions_clicked()
{
    QDir pack(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/MySqlComputerService/uploads");
    if(!pack.exists()) pack.mkpath(".");

    QFile::copy(":/extensions/Extensions/client_info.php", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/MySqlComputerService/client_info.php");
    QFile::copy(":/extensions/Extensions/upload.php", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/MySqlComputerService/upload.php");
    QFile::copy(":/extensions/Extensions/upload_photo.php", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/MySqlComputerService/upload_photo.php");
    QFile::copy(":/extensions/Extensions/error.html", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/MySqlComputerService/error.html");
    QFile::copy(":/extensions/Extensions/success.html", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/MySqlComputerService/success.html");

    InfoDialog info(tr("The extensions folder has been created on the desktop, change the access details in the php files and put the whole folder on the server"));
    info.exec();
}

void SettingsDialog::on_pushButton_get_backup_path_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Choose your destination"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!path.isEmpty())
    {
        ui->lineEdit_backup_path->setText(path);
        settings.setValue("backup/backup_path", path);
    }
}

void SettingsDialog::on_pushButton_run_backup_clicked()
{
    QString backupPath = settings.value("backup/backup_path").toString();
    if(!backupPath.isEmpty() && QDir(backupPath).exists())
    {
        if(connector.BackupDatabase(backupPath))
        {
            InfoDialog info(tr("Backup completed successfully"));
            info.exec();
        }
        else
        {
            InfoDialog info(tr("Error creating backup"), 1);
            info.exec();
        }
    }
    else
    {
        InfoDialog info(tr("Error creating backup"), 1);
        info.exec();
    }
}

void SettingsDialog::on_pushButton_add_ip_clicked()
{
    if(!IPList.contains(ui->lineEdit_ip->text())) IPList << ui->lineEdit_ip->text();
    ui->listWidget_ip_list->clear();
    ui->listWidget_ip_list->addItems(IPList);
    ui->lineEdit_ip->clear();
}

void SettingsDialog::on_listWidget_ip_list_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    if(!IPList.isEmpty())
    {
        IPList.removeAt(ui->listWidget_ip_list->currentRow());
        ui->listWidget_ip_list->clear();
        ui->listWidget_ip_list->addItems(IPList);
    }
}

