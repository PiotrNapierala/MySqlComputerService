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

#include "movetorepairdialog.h"
#include "ui_movetorepairdialog.h"

MoveToRepairDialog::MoveToRepairDialog(Order *orderToMove, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MoveToRepairDialog)
{
	ui->setupUi(this);
	ui->label_computer_producer_logo->setPixmap(QPixmap(":/PNG/IMG/question_mark.png").scaled(64, 64));
	LoadData(orderToMove);
	currentOrder = orderToMove;
	ui->doubleSpinBox_valuation->setFocus();
}

MoveToRepairDialog::~MoveToRepairDialog()
{
	delete ui;
}

void MoveToRepairDialog::LoadData(Order *order)
{
	ui->label_order_number->setText(order->orderNumber);

	QList<Client*> clientsFound = connector.ReadClientsWhere(connector.CLI_tb, "id = " + QString::number(order->clientID));
	QList<Computer*> computersFound = connector.ReadComputersWhere("id = " + QString::number(order->computerID));
	QString display;
	if(clientsFound.count() > 0)
	{
		Client *client = clientsFound.first();
		display += client->surname + " " + client->name + "\n";
	}
	else display += "CLIENT NOT FOUND\n";
	if(computersFound.count() > 0)
	{
		Computer *computer = computersFound.first();
		display += computer->producer + " " + computer->model + " ";
		ui->label_computer_producer_logo->setPixmap(serviceCore.GetProducerIcon(computer->producer));
	}
	else display += "COMPUTER NOT FOUND ";
	display += order->serialNumber;

	ui->label_order_info->setText(display);
}

void MoveToRepairDialog::on_pushButton_move_order_clicked()
{
	currentOrder->valuation = ui->doubleSpinBox_valuation->value();
	currentOrder->diagnosis = ui->textEdit_diagnosis->toPlainText();
	if(connector.MoveOrder(currentOrder, connector.CRO_tb)) this->close();
}

