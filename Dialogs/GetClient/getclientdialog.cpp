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

#include "getclientdialog.h"
#include "ui_getclientdialog.h"

GetClientDialog::GetClientDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GetClientDialog)
{
	ui->setupUi(this);
	ui->lineEdit_search->setFocus();
}

GetClientDialog::~GetClientDialog()
{
	delete ui;
}

void GetClientDialog::SearchClient()
{
	ui->listWidget_results->clear();

	QString search = ui->lineEdit_search->text();
	QString searchCommand = "CONCAT(name, ' ', surname) LIKE '%" + search + "%' OR phone LIKE '%" + search + "%' OR email LIKE '%" + search + "%' OR nip = '" + search + "'";
	searchResults = connector.ReadClientsWhere(connector.CLI_tb, searchCommand);

	foreach(Client *client, searchResults)
	{
		QString display = client->name + " " + client->surname + "\n";
		display += client->phone + " " + client->email;
		ui->listWidget_results->addItem(display);
	}
}

void GetClientDialog::on_pushButton_search_clicked()
{
	SearchClient();
}

void GetClientDialog::on_lineEdit_search_editingFinished()
{
	SearchClient();
}

void GetClientDialog::on_listWidget_results_itemDoubleClicked(QListWidgetItem *item)
{
	Q_UNUSED(item);
	selectedClient = searchResults[ui->listWidget_results->currentRow()];
	found = true;
	this->close();
}

void GetClientDialog::on_pushButton_add_client_clicked()
{
	AddClientDialog addClientDialog;
	addClientDialog.fastMode = true;
	addClientDialog.exec();

	if(addClientDialog.clientAdded)
	{
		ui->lineEdit_search->setText(addClientDialog.lastAddedClient->name + " " + addClientDialog.lastAddedClient->surname);
		SearchClient();
	}
}

