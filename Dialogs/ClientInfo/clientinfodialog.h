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

#ifndef CLIENTINFODIALOG_H
#define CLIENTINFODIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QPixmap>

#include "Classes/Order/order.h"
#include "Classes/Client/client.h"
#include "Classes/Computer/computer.h"
#include "Classes/User/user.h"

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Libraries/ServiceCore/servicecore.h"
#include "Libraries/QRConnector/qrconnector.h"

#include "Dialogs/OrderInfo/orderinfodialog.h"
#include "Dialogs/Ask/askdialog.h"

namespace Ui {
class ClientInfoDialog;
}

class ClientInfoDialog : public QDialog
{
	Q_OBJECT

public:
    explicit ClientInfoDialog(Client *client, User *user, QWidget *parent = nullptr);
	~ClientInfoDialog();

private slots:
	void on_listWidget_results_itemDoubleClicked(QListWidgetItem *item);
	void on_pushButton_remove_client_clicked();
	void on_pushButton_save_clicked();
    void on_pushButton_print_gdpr_clicked();
    void on_pushButton_qr_clicked();

private:
	Ui::ClientInfoDialog *ui;
	DatabaseConnector connector;
	ServiceCore serviceCore;
    QRConnector qrconnector;
    User *currentUser;

	Client *selectedClient;
	QList<Order*> clientOrders;

	void LoadData(Client *client);

};

#endif // CLIENTINFODIALOG_H
