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

#ifndef ADDIMAGEDIALOG_H
#define ADDIMAGEDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QDateTime>

#include "Libraries/QRConnector/qrconnector.h"
#include "Libraries/ImageDownloader/imagedownloader.h"
#include "Libraries/DatabaseConnector/databaseconnector.h"

namespace Ui {
class AddImageDialog;
}

class AddImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddImageDialog(QWidget *parent = nullptr);
    ~AddImageDialog();

    bool imageAccepted = false;
    QString finalImageName;

private slots:
    void on_pushButton_clicked();
    void CheckImage();

    void on_pushButton_new_photo_clicked();

private:
    Ui::AddImageDialog *ui;
    QRConnector qrconnector;
    ImageDownloader downloader;
    DatabaseConnector connector;

    bool imageOK = false;
    QString imageName;

    void SetupDialog();
    void GenerateQRCode();

};

#endif // ADDIMAGEDIALOG_H
