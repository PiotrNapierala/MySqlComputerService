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

#ifndef GETCLIENTDIALOG_H
#define GETCLIENTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "Classes/Client/client.h"

#include "Dialogs/AddClient/addclientdialog.h"

#include "Libraries/DatabaseConnector/databaseconnector.h"

namespace Ui {
class GetClientDialog;
}

class GetClientDialog : public QDialog
{
	Q_OBJECT

public:
	explicit GetClientDialog(QWidget *parent = nullptr);
	~GetClientDialog();

	bool found = false;
	Client *selectedClient = new Client;

private slots:
	void on_pushButton_search_clicked();
	void on_lineEdit_search_editingFinished();
	void on_listWidget_results_itemDoubleClicked(QListWidgetItem *item);
	void on_pushButton_add_client_clicked();

private:
	Ui::GetClientDialog *ui;
	DatabaseConnector connector;

	void SearchClient();

	QList<Client*> searchResults;

};

#endif // GETCLIENTDIALOG_H
