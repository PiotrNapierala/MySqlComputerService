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

#include "imagedownloader.h"

QPixmap ImageDownloader::TryImageDownload(QString url)
{
    QPixmap downloadedImage;

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QEventLoop loop;
    connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);  // Zakończ pętlę po zakończeniu pobierania

    QUrl imageUrl(url);
    QNetworkRequest request(imageUrl);
    QNetworkReply *reply = manager->get(request);

    qDebug() << url;

    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QImage image;
        image.load(reply, nullptr);

        if (!image.isNull())
        {
            downloadedImage = QPixmap::fromImage(image);
            imageDownloaded = true;
        }
        else
        {
            imageDownloaded = false;
            qDebug() << "Brak zdjęcia";
        }
    }
    else
    {
        imageDownloaded = false;
        qDebug() << "Network error";
    }

    reply->deleteLater();

    delete manager;

    return downloadedImage;
}
