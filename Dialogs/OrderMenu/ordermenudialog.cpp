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

#include "ordermenudialog.h"
#include "ui_ordermenudialog.h"

OrderMenuDialog::OrderMenuDialog(QWidget *parent, bool admin) :
	QDialog(parent),
	ui(new Ui::OrderMenuDialog)
{
	ui->setupUi(this);
    ui->pushButton_cancel_order->setVisible(admin);
    ui->pushButton_remove_order->setVisible(admin);
}

OrderMenuDialog::~OrderMenuDialog()
{
	delete ui;
}

void OrderMenuDialog::on_pushButton_order_info_clicked()
{
	selectedOption = 1;
	this->close();
}

void OrderMenuDialog::on_pushButton_move_order_clicked()
{
	selectedOption = 2;
	this->close();
}

void OrderMenuDialog::on_pushButton_cancel_order_clicked()
{
	selectedOption = 3;
	this->close();
}

void OrderMenuDialog::on_pushButton_remove_order_clicked()
{
    selectedOption = 4;
    this->close();
}
