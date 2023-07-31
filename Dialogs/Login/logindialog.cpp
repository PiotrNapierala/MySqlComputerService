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

#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
	ui->lineEdit_login->setFocus();
    ui->lineEdit_login->setValidator(serviceCore.GetUserValidator(ui->lineEdit_login));
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

void LoginDialog::on_pushButton_login_clicked()
{
	if(connector.ValidateUser(ui->lineEdit_login->text(), ui->lineEdit_password->text()))
	{
		loginCorrect = true;
		currentUser = connector.currentUser;
		this->close();
	}
	else
	{
		ui->lineEdit_login->clear();
		ui->lineEdit_password->clear();
        ui->lineEdit_login->setFocus();
    }
}
