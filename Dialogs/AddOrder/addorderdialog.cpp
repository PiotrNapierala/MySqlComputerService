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

#include "addorderdialog.h"
#include "ui_addorderdialog.h"

AddOrderDialog::AddOrderDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddOrderDialog)
{
	ui->setupUi(this);
    SetupDialog();
}

AddOrderDialog::~AddOrderDialog()
{
	delete ui;
}

void AddOrderDialog::on_pushButton_get_client_clicked()
{
	GetClientDialog getClientDialog;
	getClientDialog.exec();

	if(getClientDialog.found)
	{
		selectedClient = getClientDialog.selectedClient;
		clientSelected = true;

		ui->label_name->setText(selectedClient->name);
		ui->label_lastname->setText(selectedClient->surname);
	}
}

void AddOrderDialog::on_pushButton_get_computer_clicked()
{
	GetComputerDialog getComputerDialog;
	getComputerDialog.exec();

	if(getComputerDialog.found)
	{
		selectedComputer = getComputerDialog.selectedComputer;
		computerSelected = true;

		ui->label_computer_producer->setText(selectedComputer->producer);
		ui->label_computer_model->setText(selectedComputer->model);
		ui->label_computer_producer_logo->setPixmap(serviceCore.GetProducerIcon(selectedComputer->producer));
	}
}

void AddOrderDialog::on_pushButton_add_order_clicked()
{
	if(clientSelected && computerSelected && !ui->lineEdit_computer_serial_number->text().isEmpty())
	{
		bool emailAccepted = false;

		Order *order = new Order;
		order->orderNumber = serviceCore.GetOrderNumber();
		order->systemPassword = ui->lineEdit_system_password->text();
		order->serialNumber = ui->lineEdit_computer_serial_number->text();
		order->symptoms = ui->textEdit_symptoms->toPlainText();
		QString items;
		if(ui->checkBox_bag->isChecked()) items += "1";
		else items += "0";
		if(ui->checkBox_power_supply->isChecked()) items += "1";
		else items += "0";
		if(ui->checkBox_disks->isChecked()) items += "1";
		else items += "0";
		if(ui->checkBox_cd_dvd->isChecked()) items += "1";
		else items += "0";
		order->items = items;
		order->dateOfAdmission = QDateTime::currentDateTime();
		order->clientID = selectedClient->id;
		order->computerID = selectedComputer->id;
        order->photos = imgList;
		order->pickupCode = "";

        if(connector.ReadIntGlobalSettings("enableEmail") == true && selectedClient->gdpr == true)
        {
            AskDialog askDialog(tr("Send an email confirming receipt of the computer to the address ") + "\"" + selectedClient->email + "\"?");
            askDialog.exec();
            if(askDialog.accepted)
            {
                if(!selectedClient->email.isEmpty())
                {
                    emailAccepted = true;
                    order->pickupCode = QString::number(serviceCore.GetPickupCode());
                }
            }
        }

        if(selectedClient->systemData == false)
        {
            AskDialog ask(tr("Print permission to view files?"));
            ask.exec();
            if(ask.accepted)
            {
                serviceCore.PrintConsentToViewFiles();
                selectedClient->systemData = true;
                connector.ModifyClient(selectedClient);
            }
        }

		if(connector.SaveOrder(order, connector.CDO_tb))
		{
            if(emailAccepted)
			{
				EmailConnector emailConnector;
                emailConnector.SendConfirmationEmail(selectedClient->email, selectedClient->name + " " + selectedClient->surname, order->serialNumber, order->pickupCode, order->orderNumber);
            }

			ui->label_computer_producer->setText("-");
			ui->label_computer_model->setText("-");
			ui->label_name->setText("-");
			ui->label_lastname->setText("-");
			ui->checkBox_bag->setChecked(false);
			ui->checkBox_power_supply->setChecked(false);
			ui->checkBox_disks->setChecked(false);
			ui->checkBox_cd_dvd->setChecked(false);
			ui->lineEdit_computer_serial_number->clear();
			ui->lineEdit_system_password->clear();
			ui->textEdit_symptoms->clear();
			ui->label_computer_producer_logo->clear();
			clientSelected = false;
			computerSelected = false;
            if(settings.value("autoclose_dialogs").toBool() == true) this->close();

            InfoDialog info(tr("New order added"));
            info.exec();
		}
	}
}

void AddOrderDialog::on_pushButton_photos_clicked()
{
    PhotoPreviewDialog dialog(imgList, this);
    dialog.exec();
    if(dialog.accepted) imgList = dialog.imgList;
}

void AddOrderDialog::SetupDialog()
{
    ui->label_computer_producer_logo->setPixmap(QPixmap(":/PNG/IMG/question_mark.png").scaled(64, 64));
    ui->lineEdit_computer_serial_number->setFocus();
    if(connector.ReadIntGlobalSettings("enableExtensions") == true && !connector.ReadStringGlobalSettings("apacheServerAdress").isEmpty()) ui->pushButton_photos->setVisible(true);
    else ui->pushButton_photos->setVisible(false);
}

