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

#ifndef EMAILCONNECTOR_H
#define EMAILCONNECTOR_H

#include <QObject>
#include <QApplication>
#include <QFile>

#include "SmtpClient/src/SmtpMime"

#include "Libraries/DatabaseConnector/databaseconnector.h"

class EmailConnector : public QObject
{
    Q_OBJECT

public:
	EmailConnector();
    bool SendHtmlEmail(QString html, QString subject, QString receiverEmail, QString receiverName);
    bool SendConfirmationEmail(QString receiverEmail, QString receiverName, QString serialNumber, QString pickupCode, QString orderNumber);
    bool SendFinishEmail(QString receiverEmail, QString receiverName);

private:
	QString ReadHtmlFile(QString path);
	DatabaseConnector databaseConnector;
	EmailAddress sender;

};

#endif // EMAILCONNECTOR_H
