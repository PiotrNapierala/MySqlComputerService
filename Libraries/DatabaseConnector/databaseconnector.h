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

#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QDateTime>
#include <QFile>
#include <QCryptographicHash>
#include <QSqlRecord>
#include <QSqlField>

#include "Libraries/ApplicationLog/applicationlog.h"

#include "Classes/User/user.h"
#include "Classes/Client/client.h"
#include "Classes/Order/order.h"
#include "Classes/Computer/computer.h"

class DatabaseConnector
{
public:
    User *currentUser = new User;

	const QString CDO_tb = "current_diagnosis_orders";
	const QString CRO_tb = "current_repair_orders";
	const QString CCO_tb = "current_completed_orders";
	const QString CO_tb = "canceled_orders";
	const QString CLI_tb = "clients";
	const QString DON_tb = "done_orders";
	const QString GSET_tb = "global_settings";
	const QString LOG_tb = "logs";
	const QString RMC_tb = "removed_clients";
	const QString RMO_tb = "removed_orders";
	const QString USR_tb = "users";
	const QString COM_tb = "computers";
    const QString IMG_tb = "images";
    const QString STAT_tb = "statistic";
    const QString DEV_tb = "devices";

    bool SetupConnection(QString databasePassword);
	bool CreateTables();

	bool ValidateUser(QString login, QString password);
	bool SaveUser(User *user);
	bool RemoveUser(User *user);
	bool ModifyUser(User *user);
    QList<User*> ReadUsers();
    int GetUsersCount();

    bool BackupDatabase(QString path);

	QString ReadStringGlobalSettings(QString name);
	int ReadIntGlobalSettings(QString name);
	bool SaveStringGlobalSettings(QString name, QString value);
	bool SaveIntGlobalSettings(QString name, int value);

    bool SaveImageTemp(QString name);
    bool RemoveImageTemp(QString name);
    QString ReadImageTemp(QString name);

    bool SaveIntStatistic(QString name, int value);
    bool SaveDoubleStatistic(QString name, double value);
    bool SaveStringStatistic(QString name, QString value);
    int ReadIntStatistic(QString name);
    double ReadDoubleStatistic(QString name);
    QString ReadStringStatistic(QString name);

	bool SaveClient(Client *client, QString table);
	bool MoveClient(Client *client, QString table);
	bool ModifyClient(Client *client);
	bool RemoveClient(Client *client);
	QList<Client*> ReadClients(QString table);
	QList<Client*> ReadClientsWhere(QString table, QString where);

	bool SaveOrder(Order *order, QString table);
	bool MoveOrder(Order *order, QString table);
	bool ModifyOrder(Order *order);
	bool RemoveOrder(Order *order);
	QList<Order*> ReadOrders(QString table);
	QList<Order*> ReadOrdersWhere(QString table, QString where);
	int GetOrderCountWhere(QString table, QString where);

	bool SaveComputer(Computer *computer);
	bool ModifyComputer(Computer *computer);
	bool RemoveComputer(Computer *computer);
	QList<Computer*> ReadComputers();
	QList<Computer*> ReadComputersWhere(QString where);

    QStringList ReadDevices();
    bool SaveDevices(QStringList devices);

    QString GetDatabasePassword();

private:
	void SaveDatabaseLog(QString info);

};

#endif // DATABASECONNECTOR_H
