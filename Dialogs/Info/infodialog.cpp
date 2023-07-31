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

#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog(QString text, int status, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
    ui->label_text->setText(text);
    SetIcon(status);
    QApplication::beep();
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::on_pushButton_close_clicked()
{
    this->close();
}

void InfoDialog::SetIcon(int code)
{
    switch(code)
    {
    case 0:
        ui->label_icon->setPixmap(QPixmap(":/PNG/IMG/info.png").scaled(60, 60));
        this->setWindowIcon(QPixmap(":/PNG/IMG/info.png"));
    break;
    case 1:
        ui->label_icon->setPixmap(QPixmap(":/PNG/IMG/error.png").scaled(60, 60));
        this->setWindowIcon(QPixmap(":/PNG/IMG/error.png"));
    break;
    case 2:
        ui->label_icon->setPixmap(QPixmap(":/PNG/IMG/high_priority_error.png").scaled(60, 60));
        this->setWindowIcon(QPixmap(":/PNG/IMG/high_priority_error.png"));
    break;
    default:
        ui->label_icon->setPixmap(QPixmap(":/PNG/IMG/info.png").scaled(60, 60));
        this->setWindowIcon(QPixmap(":/PNG/IMG/info.png"));
    break;
    }
}

