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

#include "servicecore.h"

QString ServiceCore::GetOrderNumber()
{
	if(QDate::fromString(connector.ReadStringGlobalSettings("lastOrderUpdateDate"), "dd.MM.yyyy") != QDate::currentDate())
	{
		connector.SaveStringGlobalSettings("lastOrderUpdateDate", QDate::currentDate().toString("dd.MM.yyyy"));
		connector.SaveIntGlobalSettings("orderCount", 0);
	}

	QString letters = "ABCDEFGHIJKLMNOPQRSTUWXYZ";
	int orderCount = connector.ReadIntGlobalSettings("orderCount");

	QString orderNumber = QDate::currentDate().toString("dd-MM-yyyy") + "/";
	if(connector.ReadIntGlobalSettings("orderCount") >= letters.length())
	{
		orderNumber += letters[orderCount % letters.length()];
		orderNumber += QString::number(orderCount / letters.length());
	}
	else orderNumber += letters[orderCount];

	connector.SaveIntGlobalSettings("orderCount", orderCount+1);
	return orderNumber;
}

QPixmap ServiceCore::GetProducerIcon(QString name)
{
	QPixmap icon = QPixmap(":/PNG/IMG/question_mark.png").scaled(64, 64);

	if(name.contains("apple", Qt::CaseInsensitive)) icon = appleLogo.scaled(64, 64, Qt::KeepAspectRatio);
	else if(name.contains("asus", Qt::CaseInsensitive)) icon = asusLogo.scaled(64, 64, Qt::KeepAspectRatio);
	else if(name.contains("acer", Qt::CaseInsensitive)) icon = acerLogo.scaled(64, 64, Qt::KeepAspectRatio);
	else if(name.contains("dell", Qt::CaseInsensitive)) icon = dellLogo.scaled(64, 64, Qt::KeepAspectRatio);
	else if(name.contains("hp", Qt::CaseInsensitive)) icon = hpLogo.scaled(64, 64, Qt::KeepAspectRatio);
	else if(name.contains("lenovo", Qt::CaseInsensitive)) icon = lenovoLogo.scaled(64, 64, Qt::KeepAspectRatio);
	else if(name.contains("msi", Qt::CaseInsensitive)) icon = msiLogo.scaled(64, 64, Qt::KeepAspectRatio);
	else if(name.contains("samsung", Qt::CaseInsensitive)) icon = samsungLogo.scaled(64, 64, Qt::KeepAspectRatio);

	return icon;
}

int ServiceCore::GetPickupCode()
{
    return QRandomGenerator::global()->bounded(100000, 999999);
}

void ServiceCore::PrintHtml(QString html)
{
    QTextDocument *document = new QTextDocument();
    document->setHtml(html);

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, nullptr);
    if (dialog->exec() != QDialog::Accepted)
        return;

    document->print(&printer);

    delete document;
}

void ServiceCore::PrintGDPR()
{
    QFile rodo(qApp->applicationDirPath() + "/Files/gdpr.html");
    if(rodo.exists())
    {
        if(rodo.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream s(&rodo);
            QString data = s.readAll();
            data.replace("$date", QDate::currentDate().toString("dd-MM-yyyy"));
            data.replace("$service_name", connector.ReadStringGlobalSettings("serviceName"));
            if(!data.isEmpty()) PrintHtml(data);
        }
    }
}

void ServiceCore::PrintConsentToViewFiles()
{
    QFile consent(qApp->applicationDirPath() + "/Files/system_data_consent.html");
    if(consent.exists())
    {
        if(consent.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream s(&consent);
            QString data = s.readAll();
            data.replace("$date", QDate::currentDate().toString("dd-MM-yyyy"));
            data.replace("$service_name", connector.ReadStringGlobalSettings("serviceName"));
            if(!data.isEmpty()) PrintHtml(data);
        }
    }
}

bool ServiceCore::CheckVersion()
{
    QUrl url("http://pnapierala.pl/MySqlComputerService/version.txt");
    QNetworkRequest request(url);

    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QString version = reply->readAll();

        if (version.trimmed() == QCoreApplication::applicationVersion().trimmed())
        {
            reply->deleteLater();
            return false;
        }
        else
        {
            reply->deleteLater();
            return true;
        }
    }
    else
    {
        reply->deleteLater();
        return false;
    }
}

bool ServiceCore::DownloadNewVersion()
{
    QUrl url("http://pnapierala.pl/MySqlComputerService/exe/latest.exe");
    QNetworkRequest request(url);

    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QDir dir(qApp->applicationDirPath() + "/Downloads/");
        if(!dir.exists()) dir.mkpath(".");

        QFile destinationFile(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/MySqlComputerService_latest_" + QDate::currentDate().toString("dd_MM_yyyy") + ".exe");
        if (!destinationFile.open(QIODevice::WriteOnly))
        {
            reply->deleteLater();
            return false;
        }

        destinationFile.write(reply->readAll());
        destinationFile.close();

        reply->deleteLater();
        return true;
    }
    else
    {
        reply->deleteLater();
        return false;
    }
}

QValidator *ServiceCore::GetDigitalValidator(QLineEdit *lineEdit)
{
    QRegularExpression regExp("\\d+");
    QValidator *validator = new QRegularExpressionValidator(regExp, lineEdit);
    return validator;
}

QValidator *ServiceCore::GetIPValidator(QLineEdit *lineEdit)
{
    QRegularExpression regExp("^((\\d{1,3})\\.){3}(\\d{1,3})$");
    QValidator *validator = new QRegularExpressionValidator(regExp, lineEdit);
    return validator;
}

QValidator *ServiceCore::GetEmailValidator(QLineEdit *lineEdit)
{
    QRegularExpression regExp("^\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b$");
    QValidator *validator = new QRegularExpressionValidator(regExp, lineEdit);
    return validator;
}

QValidator *ServiceCore::GetUserValidator(QLineEdit *lineEdit)
{
    QRegularExpression regExp("^[A-Za-z.]+$");
    QValidator *validator = new QRegularExpressionValidator(regExp, lineEdit);
    return validator;
}


