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

#ifndef FINISHORDERDIALOG_H
#define FINISHORDERDIALOG_H

#include <QDialog>

#include "Classes/Order/order.h"
#include "Classes/Client/client.h"
#include "Classes/Computer/computer.h"

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Libraries/EmailConnector/emailconnector.h"
#include "Libraries/ServiceCore/servicecore.h"
#include "Libraries/StatisticConnector/statisticconnector.h"

#include "Dialogs/Ask/askdialog.h"
#include "Dialogs/Info/infodialog.h"

namespace Ui {
class FinishOrderDialog;
}

class FinishOrderDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FinishOrderDialog(Order *order, QWidget *parent = nullptr);
	~FinishOrderDialog();

private slots:
	void on_pushButton_confirm_pickup_code_clicked();
	void on_pushButton_finish_order_clicked();

private:
	Ui::FinishOrderDialog *ui;
	DatabaseConnector connector;
	EmailConnector emailConnector;
    StatisticConnector stat;
	ServiceCore serviceCore;

	Order *currentOrder;
	Client *selectedClient;

	void CheckPickupCode();
	void LoadData(Order *order);

};

#endif // FINISHORDERDIALOG_H
