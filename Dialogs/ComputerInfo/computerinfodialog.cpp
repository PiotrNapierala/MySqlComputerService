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

#include "computerinfodialog.h"
#include "ui_computerinfodialog.h"

ComputerInfoDialog::ComputerInfoDialog(Computer *computer, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ComputerInfoDialog)
{
	ui->setupUi(this);
	selectedComputer = computer;
	this->setWindowTitle(selectedComputer->model);
	LoadData();
}

ComputerInfoDialog::~ComputerInfoDialog()
{
	delete ui;
}

void ComputerInfoDialog::LoadData()
{
	ui->lineEdit_model->setText(selectedComputer->model);
	ui->lineEdit_producer->setText(selectedComputer->producer);
	ui->label_producer_logo->setPixmap(serviceCore.GetProducerIcon(selectedComputer->producer));
	ui->label_repaired->setText(QString::number(connector.GetOrderCountWhere(connector.DON_tb, "computer_id = " + QString::number(selectedComputer->id))));
}

void ComputerInfoDialog::on_pushButton_save_clicked()
{
	selectedComputer->model = ui->lineEdit_model->text();
	selectedComputer->producer = ui->lineEdit_producer->text();
	if(connector.ModifyComputer(selectedComputer)) this->close();
}

void ComputerInfoDialog::on_pushButton_remove_clicked()
{
    AskDialog ask(tr("Do you want to remove this computer?"));
    ask.exec();
    if(ask.accepted)
    {
        if(connector.RemoveComputer(selectedComputer)) this->close();
    }
}

void ComputerInfoDialog::on_pushButton_open_data_folder_clicked()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/MySqlComputerService/Computers/" + selectedComputer->producer + "/" + selectedComputer->model);
	if(dir.exists()) QDesktopServices::openUrl(QUrl::fromLocalFile(dir.path()));
}

