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

#include "addimagedialog.h"
#include "ui_addimagedialog.h"

AddImageDialog::AddImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddImageDialog)
{
    ui->setupUi(this);
    SetupDialog();
}

AddImageDialog::~AddImageDialog()
{
    connector.RemoveImageTemp(imageName);
    delete ui;
}

void AddImageDialog::on_pushButton_clicked()
{
    imageAccepted = true;
    this->close();
}

void AddImageDialog::CheckImage()
{
    if(!imageOK)
    {
        QString lastDatabaseImage = connector.ReadImageTemp(imageName);
        if(lastDatabaseImage.contains(imageName))
        {
            QPixmap downloadedImage;
            downloadedImage = downloader.TryImageDownload("http://" + connector.ReadStringGlobalSettings("apacheServerAdress") + "/MySqlComputerService/uploads/" + lastDatabaseImage);
            if(downloader.imageDownloaded)
            {
                ui->label_image->setPixmap(downloadedImage.scaled(ui->label_image->width(), ui->label_image->height(), Qt::KeepAspectRatio));
                finalImageName = lastDatabaseImage;
                ui->pushButton_new_photo->setVisible(true);
                connector.RemoveImageTemp(imageName);
                imageOK = true;
            }
        }
    }
}

void AddImageDialog::SetupDialog()
{
    ui->pushButton_new_photo->setVisible(false);
    GenerateQRCode();

    QTimer *imageTimer = new QTimer(this);
    connect(imageTimer, SIGNAL(timeout()), this, SLOT(CheckImage()));
    imageTimer->start(3000);
}

void AddImageDialog::GenerateQRCode()
{
    imageName = QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss");
    if(connector.SaveImageTemp(imageName))
    {
        QString url = "http://" + connector.ReadStringGlobalSettings("apacheServerAdress") + "/MySqlComputerService/upload_photo.php?id=" + imageName;
        ui->label_image->setPixmap(qrconnector.GenerateQR(url).scaled(200, 200));
    }
    else ui->label_image->setPixmap(QPixmap(":/PNG/IMG/cancel.png"));
}

void AddImageDialog::on_pushButton_new_photo_clicked()
{
    GenerateQRCode();
    imageOK = false;
    ui->pushButton_new_photo->setVisible(false);
}

