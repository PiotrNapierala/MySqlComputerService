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

#include "addclientdialog.h"
#include "ui_addclientdialog.h"

AddClientDialog::AddClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClientDialog)
{
    ui->setupUi(this);
    ui->lineEdit_name->setFocus();
    ui->lineEdit_email->setValidator(serviceCore.GetEmailValidator(ui->lineEdit_email));
}

AddClientDialog::~AddClientDialog()
{
    delete ui;
}

void AddClientDialog::AddClient()
{
    if(!ui->lineEdit_name->text().isEmpty() && !ui->lineEdit_lastname->text().isEmpty() &&
        !ui->lineEdit_phone->text().isEmpty())
    {
        Client *client = new Client;

        AskDialog ask(tr("Print consent to the processing of personal data?"));
        ask.exec();
        if(ask.accepted)
        {
            serviceCore.PrintGDPR();
            client->gdpr = true;
        }

        client->name = ui->lineEdit_name->text();
        client->surname = ui->lineEdit_lastname->text();
        client->phone = ui->lineEdit_phone->text();
        client->email = ui->lineEdit_email->text();
        client->nip = ui->lineEdit_nip->text();
        if(connector.SaveClient(client, connector.CLI_tb))
        {
            lastAddedClient = client;
            clientAdded = true;
            ui->lineEdit_name->clear();
            ui->lineEdit_lastname->clear();
            ui->lineEdit_phone->clear();
            ui->lineEdit_email->clear();
            ui->lineEdit_nip->clear();
            if(fastMode == true) this->close();
            if(settings.value("autoclose_dialogs").toBool() == true) this->close();

            InfoDialog info(tr("New client added"));
            info.exec();
        }
    }
}

void AddClientDialog::on_pushButton_add_clicked()
{
    AddClient();
}
