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

#include "firstrundialog.h"
#include "ui_firstrundialog.h"

FirstRunDialog::FirstRunDialog(QWidget *parent, int index) :
	QDialog(parent),
	ui(new Ui::FirstRunDialog)
{
	ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(index);
    ui->lineEdit_database_server_port->setValidator(serviceCore.GetDigitalValidator(ui->lineEdit_database_server_port));
    ui->lineEdit_service_email_server_port->setValidator(serviceCore.GetDigitalValidator(ui->lineEdit_service_email_server_port));
    ui->lineEdit_service_email->setValidator(serviceCore.GetEmailValidator(ui->lineEdit_service_email));
}

FirstRunDialog::~FirstRunDialog()
{
	delete ui;
}

bool FirstRunDialog::CheckDatabaseConnection()
{
	QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
	database.setHostName(ui->lineEdit_database_server_adress->text());
	database.setPort(ui->lineEdit_database_server_port->text().toInt());
	database.setDatabaseName(ui->lineEdit_database_name->text());
	database.setUserName(ui->lineEdit_database_user->text());
	database.setPassword(ui->lineEdit_database_password->text());
	if(database.open())
	{
		database.close();
		return true;
	}
	else return false;
}

void FirstRunDialog::SaveDatabaseConfig()
{
	QSettings settings;
	settings.setValue("database/database_server_adress", ui->lineEdit_database_server_adress->text());
	settings.setValue("database/database_server_port", ui->lineEdit_database_server_port->text().toInt());
	settings.setValue("database/database_name", ui->lineEdit_database_name->text());
	settings.setValue("database/database_user", ui->lineEdit_database_user->text());
    settings.setValue("database/temp_database_password", ui->lineEdit_database_password->text());
    settings.setValue("database/database_password_hash", QString(QCryptographicHash::hash(ui->lineEdit_database_password->text().toUtf8(), QCryptographicHash::Sha256).toHex()));
}

void FirstRunDialog::on_pushButton_clicked()
{
	this->close();
}

void FirstRunDialog::on_pushButton_save_clicked()
{
    switch(ui->stackedWidget->currentIndex())
    {
    case 0:
        if(!ui->lineEdit_database_server_adress->text().isEmpty() && !ui->lineEdit_database_server_port->text().isEmpty() &&
            !ui->lineEdit_database_name->text().isEmpty() && !ui->lineEdit_database_user->text().isEmpty() &&
            !ui->lineEdit_database_password->text().isEmpty())
        {
            if(CheckDatabaseConnection())
            {
                SaveDatabaseConfig();
                accepted = true;
                this->close();
            }
        }
        break;
    case 1:
        if(!ui->lineEdit_service_adress->text().isEmpty() && !ui->lineEdit_service_owner->text().isEmpty()
            && !ui->lineEdit_service_name->text().isEmpty())
        {
            bool allOK = true;
            if(!connector.SaveStringGlobalSettings("serviceAdress", ui->lineEdit_service_adress->text())) allOK = false;
            if(!connector.SaveStringGlobalSettings("serviceOwner", ui->lineEdit_service_owner->text())) allOK = false;
            if(!connector.SaveStringGlobalSettings("serviceWebsite", ui->lineEdit_service_website->text())) allOK = false;
            if(!connector.SaveStringGlobalSettings("serviceName", ui->lineEdit_service_name->text())) allOK = false;
            if(!connector.SaveIntGlobalSettings("isSet", true)) allOK = false;

            if(allOK)
            {
                accepted = true;
                ui->stackedWidget->setCurrentIndex(2);
            }
        }
        break;
    case 2:
        if(!ui->lineEdit_service_email->text().isEmpty() && !ui->lineEdit_service_email_server->text().isEmpty()
            && !ui->lineEdit_database_server_port->text().isEmpty())
        {
            bool allOK = true;
            if(!connector.SaveStringGlobalSettings("serviceEmail", ui->lineEdit_service_email->text())) allOK = false;
            if(!connector.SaveStringGlobalSettings("serviceEmailPassword", ui->lineEdit_email_password->text())) allOK = false;
            if(!connector.SaveStringGlobalSettings("serviceEmailServer", ui->lineEdit_service_email_server->text())) allOK = false;
            if(!connector.SaveIntGlobalSettings("serviceEmailServerPort", ui->lineEdit_service_email_server_port->text().toInt())) allOK = false;
            if(!connector.SaveIntGlobalSettings("emailSet", true)) allOK = false;

            if(allOK) this->close();
        }
        break;
    }
}

