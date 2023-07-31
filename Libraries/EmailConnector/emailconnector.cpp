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

#include "emailconnector.h"

EmailConnector::EmailConnector()
{
	sender = EmailAddress(databaseConnector.ReadStringGlobalSettings("serviceEmail"), databaseConnector.ReadStringGlobalSettings("serviceName"));
}

bool EmailConnector::SendHtmlEmail(QString html, QString subject, QString receiverEmail, QString receiverName)
{
    QString serviceEmail = databaseConnector.ReadStringGlobalSettings("serviceEmail");
    QString serviceEamilPassword = databaseConnector.ReadStringGlobalSettings("serviceEmailPassword");
    QString serviceEmailServer = databaseConnector.ReadStringGlobalSettings("serviceEmailServer");
    int serviceEmailServerPort = databaseConnector.ReadIntGlobalSettings("serviceEmailServerPort");

    if(serviceEmail.isEmpty() || serviceEmailServer.isEmpty() || serviceEmailServerPort == 0) return false;

	MimeMessage message;
	EmailAddress receiver(receiverEmail, receiverName);
	message.setSender(sender);
	message.addRecipient(receiver);
	message.setSubject(subject);
	MimeHtml htmlContent;
	htmlContent.setHtml(html);
	message.addPart(&htmlContent);

	SmtpClient smtp(databaseConnector.ReadStringGlobalSettings("serviceEmailServer"), databaseConnector.ReadIntGlobalSettings("serviceEmailServerPort"), SmtpClient::TcpConnection);
	smtp.connectToHost();
	if (!smtp.waitForReadyConnected()) {
		qDebug() << "Failed to connect to host!";
        return false;
	}

    smtp.login(serviceEmail, serviceEamilPassword);
	if (!smtp.waitForAuthenticated()) {
		qDebug() << "Failed to login!";
        return false;
	}

	smtp.sendMail(message);
	if (!smtp.waitForMailSent()) {
		qDebug() << "Failed to send mail!";
        return false;
	}

	smtp.quit();
    return true;
}

QString EmailConnector::ReadHtmlFile(QString path)
{
	QFile emailFile(path);
	if(emailFile.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&emailFile);
		return stream.readAll();
	}
	else return "";
}

bool EmailConnector::SendConfirmationEmail(QString receiverEmail, QString receiverName, QString serialNumber, QString pickupCode, QString orderNumber)
{
    QString html = ReadHtmlFile(qApp->applicationDirPath() + "/Files/confirmation_email.html");
	html.replace("$serialNumber", serialNumber);
	html.replace("$orderNumber", orderNumber);
	html.replace("$pickupCode", pickupCode);

    QString serviceName = databaseConnector.ReadStringGlobalSettings("serviceName");
    QString serviceWebsite = databaseConnector.ReadStringGlobalSettings("serviceWebsite");

    if(serviceWebsite.isEmpty()) html.replace("$serviceName", serviceName);
    else html.replace("$serviceName", "<a href=\"" + serviceWebsite + "\">" + serviceName + "</a>");

    return SendHtmlEmail(html, tr("Acknowledgment of computer receipt"), receiverEmail, receiverName);
}

bool EmailConnector::SendFinishEmail(QString receiverEmail, QString receiverName)
{
    QString html = ReadHtmlFile(qApp->applicationDirPath() + "/Files/thanks_email.html");
    QString serviceName = databaseConnector.ReadStringGlobalSettings("serviceName");
    QString serviceWebsite = databaseConnector.ReadStringGlobalSettings("serviceWebsite");

    if(serviceWebsite.isEmpty()) html.replace("$serviceName", serviceName);
    else html.replace("$serviceName", "<a href=\"" + serviceWebsite + "\">" + serviceName + "</a>");
    return SendHtmlEmail(html, tr("Thanks for picking up the computer"), receiverEmail, receiverName);
}
