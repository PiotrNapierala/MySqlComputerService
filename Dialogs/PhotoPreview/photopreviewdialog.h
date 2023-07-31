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

#ifndef PHOTOPREVIEWDIALOG_H
#define PHOTOPREVIEWDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QFile>

#include "Libraries/ImageDownloader/imagedownloader.h"
#include "Libraries/DatabaseConnector/databaseconnector.h"

#include "Dialogs/AddImage/addimagedialog.h"
#include "Dialogs/Info/infodialog.h"

namespace Ui {
class PhotoPreviewDialog;
}

class PhotoPreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PhotoPreviewDialog(QStringList photos, QWidget *parent = nullptr);
    ~PhotoPreviewDialog();

    QStringList imgList;
    bool accepted = false;

private slots:
    void on_listWidget_img_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_add_image_clicked();
    void on_pushButton_download_image_clicked();
    void on_pushButton_remove_image_clicked();
    void on_pushButton_save_clicked();

private:
    Ui::PhotoPreviewDialog *ui;
    ImageDownloader downloader;
    DatabaseConnector connector;

    QString selectedPhoto;
    QPixmap currentImage;

    bool imageSelected = false;

    void LoadData(QStringList photoList);
};

#endif // PHOTOPREVIEWDIALOG_H
