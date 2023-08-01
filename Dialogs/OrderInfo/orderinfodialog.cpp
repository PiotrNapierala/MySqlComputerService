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

#include "orderinfodialog.h"
#include "Dialogs/ComputerInfo/computerinfodialog.h"
#include "ui_orderinfodialog.h"

OrderInfoDialog::OrderInfoDialog(Order *order, User *user, bool clientVisable, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OrderInfoDialog)
{
	ui->setupUi(this);
	ui->tabWidget->setCurrentIndex(0);
    currentUser = user;
	ui->pushButton_show_client->setVisible(clientVisable);
    ui->pushButton_remove_order->setVisible(user->admin);
	LoadData(order);
	currentOrder = order;
}

OrderInfoDialog::~OrderInfoDialog()
{
	delete ui;
}

void OrderInfoDialog::SetEditMode(bool editMode)
{
	ui->doubleSpinBox_valuation->setReadOnly(!editMode);
	ui->doubleSpinBox_paid->setReadOnly(!editMode);
	ui->lineEdit_seals->setReadOnly(!editMode);
	ui->lineEdit_serial_number->setReadOnly(!editMode);
	ui->lineEdit_system_password->setReadOnly(!editMode);
	ui->comboBox_receipt_fv->setEnabled(editMode);
	ui->checkBox_bag->setEnabled(editMode);
	ui->checkBox_power_supply->setEnabled(editMode);
	ui->checkBox_disks->setEnabled(editMode);
	ui->checkBox_cd_dvd->setEnabled(editMode);
	ui->textEdit_diagnosis->setReadOnly(!editMode);
	ui->textEdit_repair_description->setReadOnly(!editMode);
	ui->textEdit_symptoms->setReadOnly(!editMode);
	ui->textEdit_remarks->setReadOnly(!editMode);
	ui->pushButton_get_client->setVisible(editMode);
	ui->pushButton_get_computer->setVisible(editMode);
	ui->pushButton_save_order->setVisible(editMode);
}

void OrderInfoDialog::LoadData(Order *order)
{
    this->setWindowTitle(tr("Order ") + order->orderNumber);
    if(connector.ReadIntGlobalSettings("enableExtensions") == true && !connector.ReadStringGlobalSettings("apacheServerAdress").isEmpty())
    {
        ui->pushButton_photos->setVisible(true);
        ui->tabWidget->setTabVisible(5, true);
    }
    else
    {
        ui->pushButton_photos->setVisible(false);
        ui->tabWidget->setTabVisible(5, false);
    }

	QList<Client*> clientsFound = connector.ReadClientsWhere(connector.CLI_tb, "id = " + QString::number(order->clientID));
	QList<Computer*> computersFound = connector.ReadComputersWhere("id = " + QString::number(order->computerID));
	if(clientsFound.count() > 0)
	{
		selectedClient = clientsFound.first();
		ui->label_name->setText(selectedClient->name);
		ui->label_lastname->setText(selectedClient->surname);
		ui->pushButton_show_client->setVisible(true);
	}
	else
    {
        QList<Client*> removedClientsFound = connector.ReadClientsWhere(connector.RMC_tb, "id = " + QString::number(order->clientID));
        if(removedClientsFound.count() > 0)
        {
            selectedClient = removedClientsFound.first();
            ui->label_name->setText(selectedClient->name);
            ui->label_lastname->setText(selectedClient->surname + "\n\nREMOVED");
            ui->pushButton_show_client->setVisible(false);
        }
        else
        {
            ui->label_name->setText("NOT FOUND");
            ui->label_lastname->setText("NOT FOUND");
            ui->pushButton_show_client->setVisible(false);
        }
	}

	if(computersFound.count() > 0)
	{
		selectedComputer = computersFound.first();
		ui->label_computer_producer->setText(selectedComputer->producer);
		ui->label_computer_model->setText(selectedComputer->model);
		ui->pushButton_show_computer->setIcon(QPixmap(serviceCore.GetProducerIcon(selectedComputer->producer)));
		ui->pushButton_show_computer->setVisible(true);
	}
	else
	{
		ui->label_computer_producer->setText("NOT FOUND");
		ui->label_computer_model->setText("CNOT FOUND");
		ui->pushButton_show_computer->setIcon(QPixmap(":/PNG/IMG/question_mark.png").scaled(64, 64));
		ui->pushButton_show_computer->setVisible(false);
	}

	ui->lineEdit_admission_time->setText(order->dateOfAdmission.toString("dd.MM.yyyy hh:mm"));
	ui->lineEdit_release_time->setText(order->releaseDate.toString("dd.MM.yyyy hh:mm"));
	ui->doubleSpinBox_valuation->setValue(order->valuation);
	ui->doubleSpinBox_paid->setValue(order->paid);
	ui->lineEdit_seals->setText(order->seals);
	ui->lineEdit_serial_number->setText(order->serialNumber);
	ui->lineEdit_system_password->setText(order->systemPassword);
	ui->comboBox_receipt_fv->setCurrentIndex(order->receiptFV);
	ui->textEdit_symptoms->setText(order->symptoms);
	ui->textEdit_diagnosis->setText(order->diagnosis);
	ui->textEdit_repair_description->setText(order->repairDescription);
	ui->textEdit_remarks->setText(order->remarks);

	if(order->items[0] == '1') ui->checkBox_bag->setChecked(true);
	if(order->items[1] == '1') ui->checkBox_power_supply->setChecked(true);
	if(order->items[2] == '1') ui->checkBox_disks->setChecked(true);
	if(order->items[3] == '1') ui->checkBox_cd_dvd->setChecked(true);

	if(order->currentTable == connector.DON_tb || order->currentTable == connector.CO_tb || order->currentTable == connector.RMO_tb) SetEditMode(false);
}

void OrderInfoDialog::on_pushButton_save_order_clicked()
{
	if(clientChanged) currentOrder->clientID = selectedClient->id;
	if(computerChanged) currentOrder->computerID = selectedComputer->id;
	currentOrder->diagnosis = ui->textEdit_diagnosis->toPlainText();
	currentOrder->repairDescription = ui->textEdit_repair_description->toPlainText();
	currentOrder->remarks = ui->textEdit_remarks->toPlainText();
	currentOrder->valuation = ui->doubleSpinBox_valuation->value();
	currentOrder->paid = ui->doubleSpinBox_paid->value();
	currentOrder->seals = ui->lineEdit_seals->text();
	currentOrder->systemPassword = ui->lineEdit_system_password->text();
	currentOrder->receiptFV = ui->comboBox_receipt_fv->currentIndex();
	currentOrder->serialNumber = ui->lineEdit_serial_number->text();
	currentOrder->symptoms = ui->textEdit_symptoms->toPlainText();
	QString items;
	if(ui->checkBox_bag->isChecked()) items += "1";
	else items += "0";
	if(ui->checkBox_power_supply->isChecked()) items += "1";
	else items += "0";
	if(ui->checkBox_disks->isChecked()) items += "1";
	else items += "0";
	if(ui->checkBox_cd_dvd->isChecked()) items += "1";
	else items += "0";
	currentOrder->items = items;

    if(connector.ModifyOrder(currentOrder))
    {
        InfoDialog info(tr("Order saved"));
        info.exec();
    }
}

void OrderInfoDialog::on_pushButton_get_client_clicked()
{
	GetClientDialog getClientDialog;
	getClientDialog.exec();

	if(getClientDialog.found)
	{
		clientChanged = true;
		selectedClient = getClientDialog.selectedClient;
		ui->label_name->setText(selectedClient->name);
		ui->label_lastname->setText(selectedClient->surname);
		ui->pushButton_show_client->setVisible(true);
	}
}

void OrderInfoDialog::on_pushButton_get_computer_clicked()
{
	GetComputerDialog getComputerDialog;
	getComputerDialog.exec();

	if(getComputerDialog.found)
	{
		computerChanged = true;
		selectedComputer = getComputerDialog.selectedComputer;
		ui->label_computer_producer->setText(selectedComputer->producer);
		ui->label_computer_model->setText(selectedComputer->model);
		ui->pushButton_show_computer->setIcon(QPixmap(serviceCore.GetProducerIcon(selectedComputer->producer)));
		ui->pushButton_show_computer->setVisible(true);
	}
}

void OrderInfoDialog::on_pushButton_show_client_clicked()
{
    ClientInfoDialog clientInfoDialog(selectedClient, currentUser);
	clientInfoDialog.exec();

    QList<Client*> clientsFound = connector.ReadClientsWhere(connector.CLI_tb, "id = " + QString::number(currentOrder->clientID));
    if(clientsFound.count() > 0)
    {
        selectedClient = clientsFound.first();
        ui->label_name->setText(selectedClient->name);
        ui->label_lastname->setText(selectedClient->surname);
        ui->pushButton_show_client->setVisible(true);
    }
    else
    {
        QList<Client*> removedClientsFound = connector.ReadClientsWhere(connector.RMC_tb, "id = " + QString::number(currentOrder->clientID));
        if(removedClientsFound.count() > 0)
        {
            selectedClient = removedClientsFound.first();
            ui->label_name->setText(selectedClient->name);
            ui->label_lastname->setText(selectedClient->surname + "\n\nREMOVED");
            ui->pushButton_show_client->setVisible(false);
        }
        else
        {
            ui->label_name->setText("NOT FOUND");
            ui->label_lastname->setText("NOT FOUND");
            ui->pushButton_show_client->setVisible(false);
        }
    }
}

void OrderInfoDialog::on_pushButton_remove_order_clicked()
{
    AskDialog askDialog(tr("Delete an order?"));
	askDialog.exec();
	if(askDialog.accepted)
	{
		if(connector.MoveOrder(currentOrder, connector.RMO_tb)) this->close();
	}
}

void OrderInfoDialog::on_pushButton_show_computer_clicked()
{
	ComputerInfoDialog computerInfoDialog(selectedComputer);
	computerInfoDialog.exec();

	QList<Computer*> computersFound = connector.ReadComputersWhere("id = " + QString::number(currentOrder->computerID));
	if(computersFound.count() > 0)
	{
		selectedComputer = computersFound.first();
		ui->label_computer_producer->setText(selectedComputer->producer);
		ui->label_computer_model->setText(selectedComputer->model);
		ui->pushButton_show_computer->setIcon(QPixmap(serviceCore.GetProducerIcon(selectedComputer->producer)));
		ui->pushButton_show_computer->setVisible(true);
	}
	else
	{
		ui->label_computer_producer->setText("NOT FOUND");
		ui->label_computer_model->setText("NOT FOUND");
		ui->pushButton_show_computer->setIcon(QPixmap(":/PNG/IMG/question_mark.png").scaled(64, 64));
		ui->pushButton_show_computer->setVisible(false);
	}
}

void OrderInfoDialog::on_pushButton_photos_clicked()
{
    PhotoPreviewDialog dialog(currentOrder->photos, this);
    dialog.exec();
    if(dialog.accepted)
    {
        currentOrder->photos = dialog.imgList;
        connector.ModifyOrder(currentOrder);
    }
}

