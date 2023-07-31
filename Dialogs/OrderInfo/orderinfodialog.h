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

#ifndef ORDERINFODIALOG_H
#define ORDERINFODIALOG_H

#include <QDialog>

#include "Classes/Order/order.h"
#include "Classes/Client/client.h"
#include "Classes/Computer/computer.h"
#include "Classes/User/user.h"

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Libraries/ServiceCore/servicecore.h"

#include "Dialogs/GetClient/getclientdialog.h"
#include "Dialogs/GetComputer/getcomputerdialog.h"
#include "Dialogs/ClientInfo/clientinfodialog.h"
#include "Dialogs/Ask/askdialog.h"
#include "Dialogs/ComputerInfo/computerinfodialog.h"
#include "Dialogs/Info/infodialog.h"
#include "Dialogs/PhotoPreview/photopreviewdialog.h"

namespace Ui {
class OrderInfoDialog;
}

class OrderInfoDialog : public QDialog
{
	Q_OBJECT

public:
    explicit OrderInfoDialog(Order *order, User *user, bool clientVisable = true, QWidget *parent = nullptr);
	~OrderInfoDialog();


private slots:
	void on_pushButton_save_order_clicked();
	void on_pushButton_get_client_clicked();
	void on_pushButton_get_computer_clicked();
	void on_pushButton_show_client_clicked();
	void on_pushButton_remove_order_clicked();
	void on_pushButton_show_computer_clicked();
    void on_pushButton_photos_clicked();

private:
	Ui::OrderInfoDialog *ui;
	DatabaseConnector connector;
	ServiceCore serviceCore;
    User *currentUser;

	Order *currentOrder;
	Client *selectedClient;
	bool clientChanged = false;
	Computer *selectedComputer;
	bool computerChanged = false;

	void LoadData(Order *order);
	void SetEditMode(bool editMode);

};

#endif // ORDERINFODIALOG_H
