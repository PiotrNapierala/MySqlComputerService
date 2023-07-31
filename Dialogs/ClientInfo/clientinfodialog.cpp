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

#include "clientinfodialog.h"
#include "ui_clientinfodialog.h"

ClientInfoDialog::ClientInfoDialog(Client *client, User *user, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ClientInfoDialog)
{
	ui->setupUi(this);
	LoadData(client);
	selectedClient = client;
    currentUser = user;
    ui->pushButton_remove_client->setVisible(user->admin);
    ui->lineEdit_email->setValidator(serviceCore.GetEmailValidator(ui->lineEdit_email));
}

ClientInfoDialog::~ClientInfoDialog()
{
	delete ui;
}

void ClientInfoDialog::LoadData(Client *client)
{
    if(connector.ReadIntGlobalSettings("enableExtensions") == true && !connector.ReadStringGlobalSettings("apacheServerAdress").isEmpty()) ui->pushButton_qr->setVisible(true);
    else ui->pushButton_qr->setVisible(false);

	ui->lineEdit_name->setText(client->name);
	ui->lineEdit_lastname->setText(client->surname);
	ui->lineEdit_phone->setText(client->phone);
	ui->lineEdit_email->setText(client->email);
	ui->lineEdit_nip->setText(client->nip);
    if(client->gdpr) ui->label_gdpr->setText("OK");
    if(client->systemData) ui->label_system_data->setText("OK");

	ui->label_orders_count_value->setText(QString::number(connector.GetOrderCountWhere(connector.DON_tb, "client_id = " + QString::number(client->id))));
	int maximum = connector.GetOrderCountWhere(connector.DON_tb, "client_id = " + QString::number(client->id)) + connector.GetOrderCountWhere(connector.CO_tb, "client_id = " + QString::number(client->id));
	int value = connector.GetOrderCountWhere(connector.DON_tb, "client_id = " + QString::number(client->id));
	if(value == maximum && maximum == 0) maximum = 1;
	ui->progressBar_rating->setMaximum(maximum);
	ui->progressBar_rating->setValue(value);

	clientOrders = connector.ReadOrdersWhere(connector.DON_tb, "client_id = " + QString::number(client->id));
	foreach(Order *order, clientOrders)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setIcon(QPixmap(":/PNG/IMG/question_mark.png").scaled(64, 64));
		QList<Computer*> computersFound = connector.ReadComputersWhere("id = " + QString::number(order->computerID));
		QString display = order->orderNumber + "\n";
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

void ClientInfoDialog::on_listWidget_results_itemDoubleClicked(QListWidgetItem *item)
{
	Q_UNUSED(item);
    OrderInfoDialog orderInfoDialog(clientOrders[ui->listWidget_results->currentRow()], currentUser, false);
	orderInfoDialog.exec();
}

void ClientInfoDialog::on_pushButton_remove_client_clicked()
{
    AskDialog askDialog(tr("Delete a client?"));
	askDialog.exec();
	if(askDialog.accepted)
	{
		if(connector.MoveClient(selectedClient, connector.RMC_tb)) this->close();
	}
}

void ClientInfoDialog::on_pushButton_save_clicked()
{
	selectedClient->name = ui->lineEdit_name->text();
	selectedClient->surname = ui->lineEdit_lastname->text();
	selectedClient->phone = ui->lineEdit_phone->text();
	selectedClient->email = ui->lineEdit_email->text();
	selectedClient->nip = ui->lineEdit_nip->text();
	if(connector.ModifyClient(selectedClient)) this->close();
}

void ClientInfoDialog::on_pushButton_print_gdpr_clicked()
{
    serviceCore.PrintGDPR();
    selectedClient->gdpr = true;
    connector.ModifyClient(selectedClient);
    LoadData(selectedClient);
}

void ClientInfoDialog::on_pushButton_qr_clicked()
{
    QString url = "http://" + connector.ReadStringGlobalSettings("apacheServerAdress") + "/MySqlComputerService/client_info.php?id=" + QString::number(selectedClient->id);
    QPixmap QR = qrconnector.GenerateQR(url);
    QLabel *QRLabel = new QLabel(this);
    QRLabel->setPixmap(QR.scaled(200, 200));
    QRLabel->setWindowFlags(Qt::Dialog);
    QRLabel->setWindowTitle(tr("QR code"));
    QRLabel->setWindowIcon(QPixmap(":/PNG/IMG/qr.png"));
    QRLabel->setFixedSize(QSize(200, 200));
    QRLabel->show();
}

