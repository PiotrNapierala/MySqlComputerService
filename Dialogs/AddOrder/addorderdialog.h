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

#ifndef ADDORDERDIALOG_H
#define ADDORDERDIALOG_H

#include <QDialog>
#include <QSettings>

#include "Dialogs/GetClient/getclientdialog.h"
#include "Dialogs/GetComputer/getcomputerdialog.h"
#include "Dialogs/Ask/askdialog.h"
#include "Dialogs/Info/infodialog.h"
#include "Dialogs/PhotoPreview/photopreviewdialog.h"

#include "Classes/Client/client.h"
#include "Classes/Computer/computer.h"
#include "Classes/Order/order.h"

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Libraries/ServiceCore/servicecore.h"
#include "Libraries/EmailConnector/emailconnector.h"

namespace Ui {
class AddOrderDialog;
}

class AddOrderDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddOrderDialog(QWidget *parent = nullptr);
	~AddOrderDialog();

private slots:
	void on_pushButton_get_client_clicked();
	void on_pushButton_get_computer_clicked();
	void on_pushButton_add_order_clicked();
    void on_pushButton_photos_clicked();

private:
	Ui::AddOrderDialog *ui;
	DatabaseConnector connector;
	ServiceCore serviceCore;
    QSettings settings;

    void SetupDialog();

	Client *selectedClient;
	bool clientSelected = false;
	Computer *selectedComputer;
	bool computerSelected = false;
    QStringList imgList;

};

#endif // ADDORDERDIALOG_H
