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

#include "finishorderdialog.h"
#include "ui_finishorderdialog.h"

FinishOrderDialog::FinishOrderDialog(Order *order, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FinishOrderDialog)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
	ui->lineEdit_pickup_code->setFocus();
	LoadData(order);
	currentOrder = order;
}

FinishOrderDialog::~FinishOrderDialog()
{
	delete ui;
}

void FinishOrderDialog::LoadData(Order *order)
{
	QList<Client*> clientsFound = connector.ReadClientsWhere(connector.CLI_tb, "id = " + QString::number(order->clientID));
	QList<Computer*> computersFound = connector.ReadComputersWhere("id = " + QString::number(order->computerID));

	if(clientsFound.count() > 0)
	{
		selectedClient = clientsFound.first();
		ui->label_client->setText(selectedClient->surname + " " + selectedClient->name);
	}
	else ui->label_client->setText("CLIENT NOT FOUND");

	if(computersFound.count() > 0)
	{
		Computer *computer = computersFound.first();
		ui->label_computer->setText(computer->producer + " " + computer->model);
	}
	else ui->label_computer->setText("COMPUTER NOT FOUND");


	ui->label_order_number->setText(order->orderNumber);
	if(order->items[0] == '0') ui->label_bag->setVisible(false);
	if(order->items[1] == '0') ui->label_power_supply->setVisible(false);
	if(order->items[2] == '0') ui->label_disk->setVisible(false);
	if(order->items[3] == '0') ui->label_cd_dvd->setVisible(false);
	ui->textEdit_remarks->setText(order->remarks);

	if(order->pickupCode.isEmpty()) ui->stackedWidget->setCurrentIndex(1);
}

void FinishOrderDialog::CheckPickupCode()
{
	if(ui->lineEdit_pickup_code->text() == currentOrder->pickupCode)
	{
		ui->textEdit_remarks->setFocus();
		ui->stackedWidget->setCurrentIndex(1);
	}
	else
	{
		ui->lineEdit_pickup_code->clear();
		ui->lineEdit_pickup_code->setFocus();
	}
}

void FinishOrderDialog::on_pushButton_confirm_pickup_code_clicked()
{
	CheckPickupCode();
}

void FinishOrderDialog::on_pushButton_finish_order_clicked()
{
    AskDialog askDialog(tr("Are you sure you want to complete the order?"));
	askDialog.exec();
	if(askDialog.accepted)
	{
		currentOrder->remarks = ui->textEdit_remarks->toPlainText();
		currentOrder->receiptFV = ui->comboBox_receipt_fv->currentIndex();
		connector.MoveOrder(currentOrder, connector.DON_tb);
        stat.AddMonthIntStatistic("done_orders", 1);
        stat.AddMonthDoubleStatisitic("income", currentOrder->paid);

		QList<Client*> clientsFound = connector.ReadClientsWhere(connector.CLI_tb, "id = " + QString::number(currentOrder->clientID));
		if(clientsFound.count() > 0)
		{
			selectedClient = clientsFound.first();

            if(connector.ReadIntGlobalSettings("enableEmail") == true && selectedClient->gdpr == true)
            {
                AskDialog emailAskDialog(tr("Send a thank you email to ") + "\"" + selectedClient->email + "\"?");
                emailAskDialog.exec();
                if(emailAskDialog.accepted) emailConnector.SendFinishEmail(selectedClient->email, selectedClient->name + " " + selectedClient->surname);
            }
		}

        InfoDialog info(tr("Order completed successfully"));
        info.exec();

		this->close();
	}
}

