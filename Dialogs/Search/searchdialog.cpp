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

#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(User *user, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SearchDialog)
{
	ui->setupUi(this);
    ui->lineEdit_search->setFocus();
    currentUser = user;
}

SearchDialog::~SearchDialog()
{
	delete ui;
}

void SearchDialog::SearchClients()
{
	QString search = ui->lineEdit_search->text();
	QString searchCommand = "CONCAT(name, ' ', surname) LIKE '%" + search + "%' OR phone LIKE '%" + search + "%' OR email LIKE '%" + search + "%' OR nip = '" + search + "'";
	clientsFound = connector.ReadClientsWhere(connector.CLI_tb, searchCommand);

	ui->listWidget_results->clear();
	foreach(Client *client, clientsFound)
	{
		QString display = client->surname + " " + client->name + "\n" + client->phone + " " + client->email;
		if(!client->nip.isEmpty()) display += "\n" + client->nip;
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(display);
		ui->listWidget_results->addItem(item);
	}
}

void SearchDialog::SearchOrders()
{
	ordersFound.clear();
	QString search = ui->lineEdit_search->text();
	QString searchCommand = "order_number LIKE '%" + search + "%' OR date_of_admission LIKE '%" + search + "%' OR release_date LIKE '%" + search + "%' OR serial_number LIKE '%" + search + "%' OR seals LIKE '%" + search + "%'";
	if(ui->checkBox_diagnosis_orders->isChecked()) ordersFound << connector.ReadOrdersWhere(connector.CDO_tb, searchCommand);
	if(ui->checkBox_repair_orders->isChecked()) ordersFound << connector.ReadOrdersWhere(connector.CRO_tb, searchCommand);
	if(ui->checkBox_completed_orders->isChecked()) ordersFound << connector.ReadOrdersWhere(connector.CCO_tb, searchCommand);
	if(ui->checkBox_canceled_orders->isChecked()) ordersFound << connector.ReadOrdersWhere(connector.CO_tb, searchCommand);
	ordersFound << connector.ReadOrdersWhere(connector.DON_tb, searchCommand);

	ui->listWidget_results->clear();
	foreach(Order *order, ordersFound)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setIcon(QPixmap(":/PNG/IMG/question_mark.png").scaled(64, 64));

		QList<Client*> clientsFound = connector.ReadClientsWhere(connector.CLI_tb, "id = " + QString::number(order->clientID));
		QList<Computer*> computersFound = connector.ReadComputersWhere("id = " + QString::number(order->computerID));

		QString display = order->orderNumber + "\n";

		if(clientsFound.count() > 0)
		{
			Client *client = clientsFound.first();
			display += client->surname + " " + client->name + "\n";
		}
		else display += "CLIENT NOT FOUND\n";

		if(computersFound.count() > 0)
		{
			Computer *computer = computersFound.first();
			display += computer->producer + " " + computer->model + "\n";
			item->setIcon(serviceCore.GetProducerIcon(computer->producer));
		}
		else display += "COMPUTER NOT FOUND\n";

		display += order->serialNumber;

		item->setText(display);
		ui->listWidget_results->addItem(item);
	}
}

void SearchDialog::SearchComputers()
{
	QString search = ui->lineEdit_search->text();
	QString searchCommand = "CONCAT(producer, ' ', model) LIKE '%" + search + "%'";
	computersFound = connector.ReadComputersWhere(searchCommand);

	ui->listWidget_results->clear();
	foreach(Computer *computer, computersFound)
	{
		QString display = computer->producer + " " + computer->model;
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(display);
		item->setIcon(serviceCore.GetProducerIcon(computer->producer));
		ui->listWidget_results->addItem(item);
	}
}

void SearchDialog::on_pushButton_search_clicked()
{
	if(ui->radioButton_clients->isChecked()) SearchClients();
	else if(ui->radioButton_orders->isChecked()) SearchOrders();
	else if(ui->radioButton_computers->isChecked()) SearchComputers();
}

void SearchDialog::on_lineEdit_search_editingFinished()
{
	if(ui->radioButton_clients->isChecked()) SearchClients();
	else if(ui->radioButton_orders->isChecked()) SearchOrders();
	else if(ui->radioButton_computers->isChecked()) SearchComputers();
}

void SearchDialog::on_radioButton_orders_clicked()
{
	ui->checkBox_diagnosis_orders->setVisible(true);
	ui->checkBox_repair_orders->setVisible(true);
	ui->checkBox_completed_orders->setVisible(true);
	ui->checkBox_canceled_orders->setVisible(true);
	ui->listWidget_results->clear();
}

void SearchDialog::on_radioButton_clients_clicked()
{
	ui->checkBox_diagnosis_orders->setVisible(false);
	ui->checkBox_repair_orders->setVisible(false);
	ui->checkBox_completed_orders->setVisible(false);
	ui->checkBox_canceled_orders->setVisible(false);
	ui->listWidget_results->clear();
}

void SearchDialog::on_radioButton_computers_clicked()
{
	ui->checkBox_diagnosis_orders->setVisible(false);
	ui->checkBox_repair_orders->setVisible(false);
	ui->checkBox_completed_orders->setVisible(false);
	ui->checkBox_canceled_orders->setVisible(false);
	ui->listWidget_results->clear();
}

void SearchDialog::on_listWidget_results_itemDoubleClicked(QListWidgetItem *item)
{
	Q_UNUSED(item);
	if(ui->radioButton_orders->isChecked())
	{
        OrderInfoDialog orderInfoDialog(ordersFound[ui->listWidget_results->currentRow()], currentUser);
		orderInfoDialog.exec();
	}
	else if(ui->radioButton_clients->isChecked())
	{
        ClientInfoDialog clientInfoDialog(clientsFound[ui->listWidget_results->currentRow()], currentUser);
		clientInfoDialog.exec();
	}
	else if(ui->radioButton_computers->isChecked())
	{
        ComputerInfoDialog computerInfoDialog(computersFound[ui->listWidget_results->currentRow()]);
		computerInfoDialog.exec();
	}

	if(ui->radioButton_clients->isChecked()) SearchClients();
	else if(ui->radioButton_orders->isChecked()) SearchOrders();
	else if(ui->radioButton_computers->isChecked()) SearchComputers();
}

