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

#include "photopreviewdialog.h"
#include "ui_photopreviewdialog.h"

PhotoPreviewDialog::PhotoPreviewDialog(QStringList photos, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhotoPreviewDialog)
{
    ui->setupUi(this);
    LoadData(photos);
}

PhotoPreviewDialog::~PhotoPreviewDialog()
{
    delete ui;
}

void PhotoPreviewDialog::on_listWidget_img_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    selectedPhoto = imgList[ui->listWidget_img->currentRow()];
    qDebug() << selectedPhoto << " " << ui->listWidget_img->currentRow();
    QString url = "http://" + connector.ReadStringGlobalSettings("apacheServerAdress") + "/MySqlComputerService/uploads/" + selectedPhoto;
    currentImage = downloader.TryImageDownload(url);
    if(downloader.imageDownloaded)
    {
        ui->label_photo_preview->setPixmap(currentImage.scaled(ui->label_photo_preview->width(), ui->label_photo_preview->height(), Qt::KeepAspectRatio));
        imageSelected = true;
        ui->pushButton_download_image->setVisible(true);
        ui->pushButton_remove_image->setVisible(true);
    }
    else
    {
        ui->label_photo_preview->setPixmap(QPixmap(":/PNG/IMG/cancel.png"));
        ui->pushButton_download_image->setVisible(false);
        ui->pushButton_remove_image->setVisible(false);
    }
}

void PhotoPreviewDialog::LoadData(QStringList photoList)
{
    imgList.clear();
    foreach(QString photo, photoList) if(!photo.isEmpty()) imgList << photo;

    ui->label_photo_preview->clear();
    imageSelected = false;
    selectedPhoto.clear();
    ui->listWidget_img->clear();

    foreach(QString img, imgList)
    {
        if(!img.isEmpty()) ui->listWidget_img->addItem(img);
    }

    ui->pushButton_download_image->setVisible(false);
    ui->pushButton_remove_image->setVisible(false);
}

void PhotoPreviewDialog::on_pushButton_add_image_clicked()
{
    AddImageDialog dialog;
    dialog.exec();
    if(dialog.imageAccepted) imgList << dialog.finalImageName;
    LoadData(imgList);
}

void PhotoPreviewDialog::on_pushButton_download_image_clicked()
{
    if(imageSelected && !selectedPhoto.isEmpty())
    {
        QString path = QFileDialog::getSaveFileName(this, tr("Save the photo"), qApp->applicationDirPath(), ".png");
        QFile file(path + ".png");
        if(file.open(QIODevice::WriteOnly))
        {
            currentImage.save(&file, "PNG");
            file.close();
        }
        InfoDialog info(tr("Photo saved"));
        info.exec();
    }
}

void PhotoPreviewDialog::on_pushButton_remove_image_clicked()
{
    if(imageSelected && !selectedPhoto.isEmpty())
    {
        imgList.removeAll(selectedPhoto);
        LoadData(imgList);
    }
}

void PhotoPreviewDialog::on_pushButton_save_clicked()
{
    accepted = true;
    this->close();
}
