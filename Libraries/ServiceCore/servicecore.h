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

#ifndef SERVICECORE_H
#define SERVICECORE_H

#include <QObject>
#include <QDateTime>
#include <QPixmap>
#include <QRandomGenerator>
#include <QTextDocument>
#include <QFile>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QValidator>
#include <QRegularExpression>
#include <QLineEdit>

#include "Libraries/DatabaseConnector/databaseconnector.h"

class ServiceCore
{
public:
	QString GetOrderNumber();
	QPixmap GetProducerIcon(QString name);
	int GetPickupCode();
    void PrintHtml(QString html);
    void PrintGDPR();
    void PrintConsentToViewFiles();
    QValidator *GetDigitalValidator(QLineEdit *lineEdit);
    QValidator *GetIPValidator(QLineEdit *lineEdit);
    QValidator *GetEmailValidator(QLineEdit *lineEdit);
    QValidator *GetUserValidator(QLineEdit *lineEdit);

private:
	DatabaseConnector connector;

	QPixmap appleLogo = QPixmap(":/logo/LOGO/apple.png");
	QPixmap asusLogo = QPixmap(":/logo/LOGO/asus.png");
	QPixmap acerLogo = QPixmap(":/logo/LOGO/acer.png");
	QPixmap dellLogo = QPixmap(":/logo/LOGO/dell.png");
	QPixmap hpLogo = QPixmap(":/logo/LOGO/hp.png");
	QPixmap lenovoLogo = QPixmap(":/logo/LOGO/lenovo.png");
	QPixmap msiLogo = QPixmap(":/logo/LOGO/msi.png");
	QPixmap samsungLogo = QPixmap(":/logo/LOGO/samsung.jpg");

};

#endif // SERVICECORE_H