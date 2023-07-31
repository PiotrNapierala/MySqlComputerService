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

#include "addmenudialog.h"
#include "ui_addmenudialog.h"

AddMenuDialog::AddMenuDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddMenuDialog)
{
	ui->setupUi(this);
}

AddMenuDialog::~AddMenuDialog()
{
	delete ui;
}

void AddMenuDialog::on_pushButton_add_client_clicked()
{
	this->close();
	AddClientDialog addClientDialog;
	addClientDialog.exec();
}

void AddMenuDialog::on_pushButton_add_computer_clicked()
{
	this->close();
	AddComputerDialog addComputerDialog;
	addComputerDialog.exec();
}

void AddMenuDialog::on_pushButton_add_order_clicked()
{
	this->close();
	AddOrderDialog addOrderDialog;
	addOrderDialog.exec();
}

