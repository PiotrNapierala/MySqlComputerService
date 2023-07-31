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

#include "getcomputerdialog.h"
#include "ui_getcomputerdialog.h"

GetComputerDialog::GetComputerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GetComputerDialog)
{
	ui->setupUi(this);
	ui->lineEdit_search->setFocus();
}

GetComputerDialog::~GetComputerDialog()
{
	delete ui;
}

void GetComputerDialog::SearchComputer()
{
	ui->listWidget_results->clear();

	QString search = ui->lineEdit_search->text();
	QString searchCommand = "CONCAT(producer, ' ', model) LIKE '%" + search + "%'";
	searchResults = connector.ReadComputersWhere(searchCommand);

	foreach(Computer *computer, searchResults)
	{
		QString display = computer->producer + "\n" + computer->model;
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(display);
		item->setIcon(serviceCore.GetProducerIcon(computer->producer));
		ui->listWidget_results->addItem(item);
	}
}

void GetComputerDialog::on_pushButton_search_clicked()
{
	SearchComputer();
}

void GetComputerDialog::on_lineEdit_search_editingFinished()
{
	SearchComputer();
}

void GetComputerDialog::on_listWidget_results_itemDoubleClicked(QListWidgetItem *item)
{
	Q_UNUSED(item);
	selectedComputer = searchResults[ui->listWidget_results->currentRow()];
	found = true;
	this->close();
}

void GetComputerDialog::on_pushButton_add_computer_clicked()
{
	AddComputerDialog addComputerDialog;
	addComputerDialog.fastMode = true;
	addComputerDialog.exec();

	if(addComputerDialog.computerAdded)
	{
		ui->lineEdit_search->setText(addComputerDialog.lastAddedComputer->producer + " " + addComputerDialog.lastAddedComputer->model);
		SearchComputer();
	}
}

