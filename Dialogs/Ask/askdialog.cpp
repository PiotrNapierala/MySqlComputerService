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

#include "askdialog.h"
#include "ui_askdialog.h"

AskDialog::AskDialog(QString text, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AskDialog)
{
	ui->setupUi(this);
	ui->label_text->setText(text);
	QApplication::beep();
}

AskDialog::~AskDialog()
{
	delete ui;
}

void AskDialog::on_pushButton_accept_clicked()
{
	accepted = true;
	this->close();
}

void AskDialog::on_pushButton_cancel_clicked()
{
	accepted = false;
	this->close();
}

