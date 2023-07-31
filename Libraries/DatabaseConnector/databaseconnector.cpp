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

#include "databaseconnector.h"

bool DatabaseConnector::SetupConnection()
{
	QSettings settings;
	QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL", "database");
	database.setHostName(settings.value("database/database_server_adress").toString());
	database.setPort(settings.value("database/database_server_port").toInt());
	database.setDatabaseName(settings.value("database/database_name").toString());
	database.setUserName(settings.value("database/database_user").toString());
	database.setPassword(settings.value("database/database_password").toString());
	if(database.open())
	{
		database.close();
        ApplicationLog::SaveLocalLog("Database connection successfully established");
		return true;
    }
    else
    {
        ApplicationLog::SaveLocalLog("Error creating database connection");
        return false;
    }
}

bool DatabaseConnector::CreateTables()
{
    QStringList files;
	files = {":/txt/Tables/canceled_orders.txt", ":/txt/Tables/clients.txt", ":/txt/Tables/computers.txt",
			":/txt/Tables/current_completed_orders.txt", ":/txt/Tables/current_diagnosis_orders.txt", ":/txt/Tables/current_repair_orders.txt",
			":/txt/Tables/done_orders.txt", ":/txt/Tables/global_settings.txt", ":/txt/Tables/logs.txt", ":/txt/Tables/removed_clients.txt",
            ":/txt/Tables/removed_orders.txt", ":/txt/Tables/users.txt", ":/txt/Tables/statistic.txt", ":/txt/Tables/images.txt", ":/txt/Tables/devices.txt"};

    bool status = true;

    if(QSqlDatabase::database("database").open())
    {
        QSqlQuery query(QSqlDatabase::database("database"));
        foreach(QString filePath, files)
        {
            QFile commandFile(filePath);
            if(commandFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString command = commandFile.readAll();
                if(!query.exec(command))
                {
                    ApplicationLog::SaveLocalLog("Error while executing the SQL command - " + command);
                    status = false;
                }
            }
        }
        QSqlDatabase::database("database").close();
    }

    return status;
}

void DatabaseConnector::SaveDatabaseLog(QString info)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "INSERT INTO $table_name (date, info) VALUES ('$date', '$info');";
        command.replace("$table_name", LOG_tb);
		command.replace("$date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
		command.replace("$info", info);
        QSqlQuery query(QSqlDatabase::database("database"));
        if(!query.exec(command)) ApplicationLog::SaveLocalLog("Error while executing the SQL command - " + command);
		QSqlDatabase::database("database").close();
	}
}

int DatabaseConnector::GetUsersCount()
{
    int toReturn = 0;

    if(QSqlDatabase::database("database").open())
    {
        QString command = "SELECT COUNT(id) FROM $table_name;";
        command.replace("$table_name", USR_tb);
        QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next()) toReturn = query.value(0).toInt();
        QSqlDatabase::database("database").close();
    }

    return toReturn;
}

bool DatabaseConnector::BackupDatabase(QString path)
{
    if(QSqlDatabase::database("database").open())
    {
        QStringList tables;
        QString tableCommand = "SELECT table_name FROM information_schema.tables WHERE table_schema = '$database_name';";
        tableCommand.replace("$database_name", QSqlDatabase::database("database").databaseName());
        QSqlQuery tableQuery(tableCommand, QSqlDatabase::database("database"));
        while(tableQuery.next()) tables << tableQuery.value(0).toString();
        if(tables.isEmpty()) return false;

        QDir backupDir(path + "/MySqlComputerService_backup/" + QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss"));
        if(!backupDir.exists()) backupDir.mkpath(".");
        QStringList backupFiles;

        foreach(QString table, tables)
        {
            QString headersCommand = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = '$database_name' AND TABLE_NAME = '$table_name';";
            headersCommand.replace("$database_name", QSqlDatabase::database("database").databaseName());
            headersCommand.replace("$table_name", table);

            QStringList headers;
            QSqlQuery headersQuery(headersCommand, QSqlDatabase::database("database"));
            while(headersQuery.next()) headers << headersQuery.value(0).toString();
            if(headers.isEmpty()) return false;

            QString dataCommand = "SELECT * FROM $table_name;";
            dataCommand.replace("$table_name", table);

            QStringList data, mainData;
            QSqlQuery dataQuery(dataCommand, QSqlDatabase::database("database"));
            while(dataQuery.next())
            {
                data.clear();
                for(int i = 0; i < headers.count(); i++) data << dataQuery.value(i).toString();
                mainData << data.join(",");
            }

            backupFiles << table + "_backup.csv";
            QFile csvFile(backupDir.path() + "/" + table + "_backup.csv");
            if(csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream s(&csvFile);
                s << headers.join(",") << "\n";
                foreach(QString line, mainData) s << line << "\n";
                csvFile.close();
            }
            else return false;
        }

        if(backupFiles.isEmpty()) return false;

        QSettings backupFile(backupDir.path() + "/backup_info.msqlcs", QSettings::IniFormat);
        backupFile.setValue("tables", tables);
        backupFile.setValue("files", backupFiles);
    }
    else return false;

    return true;
}

QList<User*> DatabaseConnector::ReadUsers()
{
    QList<User*> users;
    if(QSqlDatabase::database("database").open())
    {
        QString command = "SELECT * FROM $table_name;";
        command.replace("$table_name", USR_tb);
        QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next())
        {
            User *user = new User;
            user->id = query.value(0).toInt();
            user->login = query.value(1).toString();
            user->password = query.value(2).toString();
            user->admin = query.value(3).toBool();
            user->active = query.value(4).toBool();
            users << user;
        }
        QSqlDatabase::database("database").close();
    }
    return users;
}

bool DatabaseConnector::ValidateUser(QString login, QString password)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "SELECT * FROM $table_name WHERE login = '$login' LIMIT 1;";
        command.replace("$table_name", USR_tb);
		command.replace("$login", login);
		QSqlQuery query(command, QSqlDatabase::database("database"));
		while(query.next())
		{
            currentUser->id = query.value(0).toInt();
            currentUser->login = query.value(1).toString();
            currentUser->password = query.value(2).toString();
            currentUser->admin = query.value(3).toBool();
            currentUser->active = query.value(4).toBool();
		}
		QSqlDatabase::database("database").close();

        QString passHash = QByteArray(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256)).toHex();

        if(currentUser->login == login && currentUser->password == passHash && currentUser->active == true)
		{
			ApplicationLog::SaveLocalLog("Successful user validation");
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog("User validation error");
			return false;
		}
	}
	else
	{
		ApplicationLog::SaveLocalLog("Database connection error");
		return false;
	}
}

bool DatabaseConnector::SaveUser(User *user)
{
	if(QSqlDatabase::database("database").open())
	{
        QString passHash = QByteArray(QCryptographicHash::hash(user->password.toUtf8(), QCryptographicHash::Sha256)).toHex();

        QString command = "INSERT IGNORE INTO $table_name (login, password, admin, active) VALUES ('$login', '$password', '$admin', '$active');";
        command.replace("$table_name", USR_tb);
		command.replace("$login", user->login);
        command.replace("$password", passHash);
		command.replace("$admin", QString::number(user->admin));
		command.replace("$active", QString::number(user->active));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("User saved");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::RemoveUser(User *user)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "DELETE FROM $table_name WHERE login = '$login';";
        command.replace("$table_name", USR_tb);
		command.replace("$login", user->login);
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("User removed from table");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::ModifyUser(User *user)
{
	if(QSqlDatabase::database("database").open())
	{
        QString passHash = QByteArray(QCryptographicHash::hash(user->password.toUtf8(), QCryptographicHash::Sha256)).toHex();

        QString command = "UPDATE $table_name SET login = '$login', password = '$password', admin = $admin, active = $active WHERE login = '$login';";
        command.replace("$table_name", USR_tb);
        command.replace("$login", user->login);
        command.replace("$password", passHash);
		command.replace("$admin", QString::number(user->admin));
		command.replace("$active", QString::number(user->active));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("User modified");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::SaveClient(Client *client, QString table)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "INSERT INTO $table (name, surname, phone, email, nip, gdpr_accepted, system_data_accepted) VALUES ('$name', '$surname', '$phone', '$email', '$nip', '$gdpr_accepted', '$system_data_accepted');";
		command.replace("$table", table);
		command.replace("$name", client->name);
		command.replace("$surname", client->surname);
		command.replace("$phone", client->phone);
		command.replace("$email", client->email);
		command.replace("$nip", client->nip);
        command.replace("$gdpr_accepted", QString::number(client->gdpr));
        command.replace("$system_data_accepted", QString::number(client->systemData));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Client saved");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::RemoveClient(Client *client)
{
	if(QSqlDatabase::database("database").open())
	{
		QString command = "DELETE FROM $table WHERE id = '$id';";
		command.replace("$table", client->currentTable);
		command.replace("$id", QString::number(client->id));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Client removed from table " + client->currentTable);
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::MoveClient(Client *client, QString table)
{
	if(SaveClient(client, table))
	{
		if(RemoveClient(client)) return true;
		else return false;
	}
	else return false;
}

bool DatabaseConnector::ModifyClient(Client *client)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "UPDATE $table SET name = '$name', surname = '$surname', phone = '$phone', email = '$email', nip = '$nip', gdpr_accepted = '$gdpr_accepted', system_data_accepted = '$system_data_accepted' WHERE $table.id = $id;";
		command.replace("$table", client->currentTable);
		command.replace("$id", QString::number(client->id));
		command.replace("$name", client->name);
		command.replace("$surname", client->surname);
		command.replace("$phone", client->phone);
		command.replace("$email", client->email);
		command.replace("$nip", client->nip);
        command.replace("$gdpr_accepted", QString::number(client->gdpr));
        command.replace("$system_data_accepted", QString::number(client->systemData));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Client modified");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

QList<Client*> DatabaseConnector::ReadClients(QString table)
{
	QList<Client*> clients;
	if(QSqlDatabase::database("database").open())
	{
		QString command = "SELECT * FROM $table;";
		command.replace("$table", table);
		QSqlQuery query(command, QSqlDatabase::database("database"));
		while(query.next())
		{
			Client *client = new Client;
			client->id = query.value(0).toInt();
			client->name = query.value(1).toString();
			client->surname = query.value(2).toString();
			client->phone = query.value(3).toString();
			client->email = query.value(4).toString();
			client->nip = query.value(5).toString();
            client->gdpr = query.value(6).toBool();
            client->systemData = query.value(7).toBool();
			client->currentTable = table;
			clients << client;
		}
		QSqlDatabase::database("database").close();
	}
	return clients;
}

QList<Client*> DatabaseConnector::ReadClientsWhere(QString table, QString where)
{
	QList<Client*> clients;
	if(QSqlDatabase::database("database").open())
	{
		QString command = "SELECT * FROM $table WHERE " + where + ";";
		command.replace("$table", table);
		QSqlQuery query(command, QSqlDatabase::database("database"));
		while(query.next())
		{
			Client *client = new Client;
			client->id = query.value(0).toInt();
			client->name = query.value(1).toString();
			client->surname = query.value(2).toString();
			client->phone = query.value(3).toString();
			client->email = query.value(4).toString();
			client->nip = query.value(5).toString();
            client->gdpr = query.value(6).toBool();
            client->systemData = query.value(7).toBool();
			client->currentTable = table;
			clients << client;
		}
		QSqlDatabase::database("database").close();
	}
	return clients;
}



int DatabaseConnector::GetOrderCountWhere(QString table, QString where)
{
	int toReturn = 0;

	if(QSqlDatabase::database("database").open())
	{
		QString command = "SELECT COUNT(id) FROM $table WHERE " + where + ";";
		command.replace("$table", table);
		QSqlQuery query(command, QSqlDatabase::database("database"));
		while(query.next()) toReturn = query.value(0).toInt();
		QSqlDatabase::database("database").close();
	}

    return toReturn;
}

bool DatabaseConnector::SaveOrder(Order *order, QString table)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "INSERT INTO $table (client_id, order_number, date_of_admission, release_date, diagnosis, repair_description, remarks, valuation, paid, seals, system_password, receiptFV, serial_number, computer_id, symptoms, items, pickup_code, img) VALUES ('$client_id', '$order_number', '$date_of_admission', '$release_date', '$diagnosis', '$repair_description', '$remarks', '$valuation', '$paid', '$seals', '$system_password', '$receiptFV', '$serial_number', '$computer_id', '$symptoms', '$items', '$pickup_code', '$img');";
		command.replace("$table", table);
		command.replace("$client_id", QString::number(order->clientID));
		command.replace("$order_number", order->orderNumber);
		command.replace("$date_of_admission", order->dateOfAdmission.toString("yyyy-MM-dd hh:mm:ss"));
		command.replace("$release_date", order->releaseDate.toString("yyyy-MM-dd hh:mm:ss"));
		command.replace("$diagnosis", order->diagnosis);
		command.replace("$repair_description", order->repairDescription);
		command.replace("$remarks", order->remarks);
		command.replace("$valuation", QString::number(order->valuation));
		command.replace("$paid", QString::number(order->paid));
		command.replace("$seals", order->seals);
		command.replace("$system_password", order->systemPassword);
		command.replace("$receiptFV", QString::number(order->receiptFV));
		command.replace("$serial_number", order->serialNumber);
		command.replace("$computer_id", QString::number(order->computerID));
		command.replace("$symptoms", order->symptoms);
		command.replace("$items", order->items);
		command.replace("$pickup_code", order->pickupCode);
        command.replace("$img", order->photos.join(";"));

		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Order saved");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::RemoveOrder(Order *order)
{
	if(QSqlDatabase::database("database").open())
	{
		QString command = "DELETE FROM $table WHERE id = $id;";
		command.replace("$table", order->currentTable);
		command.replace("$id", QString::number(order->id));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Order removed from table " + order->currentTable);
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::MoveOrder(Order *order, QString table)
{
	if(SaveOrder(order, table))
	{
		if(RemoveOrder(order)) return true;
		else return false;
	}
	else return false;
}

bool DatabaseConnector::ModifyOrder(Order *order)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "UPDATE $table SET client_id = '$client_id', order_number = '$order_number', date_of_admission = '$date_of_admission', release_date = '$release_date', diagnosis = '$diagnosis', repair_description = '$repair_description', remarks = '$remarks', valuation = '$valuation', paid = '$paid', seals = '$seals', system_password = '$system_password', receiptFV = '$receiptFV', serial_number = '$serial_number', computer_id = '$computer_id', symptoms = '$symptoms', items = '$items', pickup_code = '$pickup_code', img = '$img' WHERE $table.id = $id;";
		command.replace("$table", order->currentTable);
		command.replace("$id", QString::number(order->id));
		command.replace("$client_id", QString::number(order->clientID));
		command.replace("$order_number", order->orderNumber);
		command.replace("$date_of_admission", order->dateOfAdmission.toString("yyyy-MM-dd hh:mm:ss"));
		command.replace("$release_date", order->releaseDate.toString("yyyy-MM-dd hh:mm:ss"));
		command.replace("$diagnosis", order->diagnosis);
		command.replace("$repair_description", order->repairDescription);
		command.replace("$remarks", order->remarks);
		command.replace("$valuation", QString::number(order->valuation));
		command.replace("$paid", QString::number(order->paid));
		command.replace("$seals", order->seals);
		command.replace("$system_password", order->systemPassword);
		command.replace("$receiptFV", QString::number(order->receiptFV));
		command.replace("$serial_number", order->serialNumber);
		command.replace("$computer_id", QString::number(order->computerID));
		command.replace("$symptoms", order->symptoms);
		command.replace("$items", order->items);
		command.replace("$pickup_code", order->pickupCode);
        command.replace("$img", order->photos.join(";"));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Order modified");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

QList<Order*> DatabaseConnector::ReadOrders(QString table)
{
	QList<Order*> orders;
	if(QSqlDatabase::database("database").open())
	{
		QString command = "SELECT * FROM $table;";
		command.replace("$table", table);
		QSqlQuery query(command, QSqlDatabase::database("database"));
		while(query.next())
		{
			Order *order = new Order;
			order->id = query.value(0).toInt();
			order->clientID = query.value(1).toInt();
			order->orderNumber = query.value(2).toString();
			order->dateOfAdmission = query.value(3).toDateTime();
			order->releaseDate = query.value(4).toDateTime();
			order->diagnosis = query.value(5).toString();
			order->repairDescription = query.value(6).toString();
			order->remarks = query.value(7).toString();
			order->valuation = query.value(8).toDouble();
			order->paid = query.value(9).toDouble();
			order->seals = query.value(10).toString();
			order->systemPassword = query.value(11).toString();
			order->receiptFV = query.value(12).toInt();
			order->serialNumber = query.value(13).toString();
			order->computerID = query.value(14).toInt();
			order->symptoms = query.value(15).toString();
			order->items = query.value(16).toString();
			order->pickupCode = query.value(17).toString();
            order->photos = query.value(18).toString().split(";");
			order->currentTable = table;
			orders << order;
		}
		QSqlDatabase::database("database").close();
	}
	return orders;
}

QList<Order*> DatabaseConnector::ReadOrdersWhere(QString table, QString where)
{
	QList<Order*> orders;
	if(QSqlDatabase::database("database").open())
	{
		QString command = "SELECT * FROM $table WHERE " + where + ";";
		command.replace("$table", table);
		QSqlQuery query(command, QSqlDatabase::database("database"));
		while(query.next())
		{
			Order *order = new Order;
			order->id = query.value(0).toInt();
			order->clientID = query.value(1).toInt();
			order->orderNumber = query.value(2).toString();
			order->dateOfAdmission = query.value(3).toDateTime();
			order->releaseDate = query.value(4).toDateTime();
			order->diagnosis = query.value(5).toString();
			order->repairDescription = query.value(6).toString();
			order->remarks = query.value(7).toString();
			order->valuation = query.value(8).toDouble();
			order->paid = query.value(9).toDouble();
			order->seals = query.value(10).toString();
			order->systemPassword = query.value(11).toString();
			order->receiptFV = query.value(12).toInt();
			order->serialNumber = query.value(13).toString();
			order->computerID = query.value(14).toInt();
			order->symptoms = query.value(15).toString();
			order->items = query.value(16).toString();
			order->pickupCode = query.value(17).toString();
            order->photos = query.value(18).toString().split(";");
			order->currentTable = table;
			orders << order;
		}
		QSqlDatabase::database("database").close();
	}
	return orders;
}

bool DatabaseConnector::SaveComputer(Computer *computer)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "INSERT INTO $table_name (producer, model) VALUES ('$producer', '$model');";
        command.replace("$table_name", COM_tb);
		command.replace("$producer", computer->producer);
		command.replace("$model", computer->model);
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Computer saved");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::ModifyComputer(Computer *computer)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "UPDATE $table_name SET producer = '$producer', model = '$model' WHERE id = $id;";
        command.replace("$table_name", COM_tb);
		command.replace("$id", QString::number(computer->id));
		command.replace("$producer", computer->producer);
		command.replace("$model", computer->model);
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Computer modified");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::RemoveComputer(Computer *computer)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "DELETE FROM $table_name WHERE id = $id;";
        command.replace("$table_name", COM_tb);
		command.replace("$id", QString::number(computer->id));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Order removed from table computers");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

QList<Computer*> DatabaseConnector::ReadComputers()
{
	QList<Computer*> computers;
	if(QSqlDatabase::database("database").open())
	{
        QString command = "SELECT * FROM $table_name;";
        command.replace("$table_name", COM_tb);
		QSqlQuery query(command, QSqlDatabase::database("database"));
		while(query.next())
		{
			Computer *computer = new Computer;
			computer->id = query.value(0).toInt();
			computer->producer = query.value(1).toString();
			computer->model = query.value(2).toString();
			computers << computer;
		}
		QSqlDatabase::database("database").close();
	}
	return computers;
}

QList<Computer*> DatabaseConnector::ReadComputersWhere(QString where)
{
	QList<Computer*> computers;
	if(QSqlDatabase::database("database").open())
	{
        QString command = "SELECT * FROM $table_name WHERE " + where + ";";
        command.replace("$table_name", COM_tb);
		QSqlQuery query(command, QSqlDatabase::database("database"));
		while(query.next())
		{
			Computer *computer = new Computer;
			computer->id = query.value(0).toInt();
			computer->producer = query.value(1).toString();
			computer->model = query.value(2).toString();
			computers << computer;
		}
		QSqlDatabase::database("database").close();
    }
    return computers;
}

QStringList DatabaseConnector::ReadDevices()
{
    QStringList devices;
    if(QSqlDatabase::database("database").open())
    {
        QString command = "SELECT * FROM $table_name;";
        command.replace("$table_name", DEV_tb);
        QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next())
        {
            devices << query.value(0).toString();
        }
        QSqlDatabase::database("database").close();
    }
    return devices;
}

bool DatabaseConnector::SaveDevices(QStringList devices)
{
    bool status = true;

    if(QSqlDatabase::database("database").open())
    {
        QString clearCommand = "TRUNCATE TABLE $table_name;";
        clearCommand.replace("$table_name", DEV_tb);
        QSqlQuery query(QSqlDatabase::database("database"));
        if(!query.exec(clearCommand)) return false;
        foreach(QString device, devices)
        {
            QString command = "INSERT IGNORE INTO $table_name (IP) VALUES ('$IP');";
            command.replace("$table_name", DEV_tb);
            command.replace("$IP", device);
            if(!query.exec(command)) status = false;
        }
        QSqlDatabase::database("database").close();
    }
    else return false;

    return status;
}

QString DatabaseConnector::ReadStringGlobalSettings(QString name)
{
	if(QSqlDatabase::database("database").open())
	{
		QString value = "";
        QString command = "SELECT * FROM $table_name WHERE name = '$name';";
        command.replace("$table_name", GSET_tb);
		command.replace("$name", name);
		QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next()) value = query.value(1).toString();
		QSqlDatabase::database("database").close();
		return value;
	}
	else return "";
}

int DatabaseConnector::ReadIntGlobalSettings(QString name)
{
	if(QSqlDatabase::database("database").open())
	{
		int value = 0;
        QString command = "SELECT * FROM $table_name WHERE name = '$name';";
        command.replace("$table_name", GSET_tb);
		command.replace("$name", name);
		QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next()) value = query.value(1).toInt();
		QSqlDatabase::database("database").close();
		return value;
	}
	else return 0;
}

bool DatabaseConnector::SaveStringGlobalSettings(QString name, QString value)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "INSERT INTO $table_name (name, value) VALUES ('$name', '$value') ON DUPLICATE KEY UPDATE value='$value';";
        command.replace("$table_name", GSET_tb);
		command.replace("$name", name);
		command.replace("$value", value);
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Global settings saved");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
	}
	else return false;
}

bool DatabaseConnector::SaveIntGlobalSettings(QString name, int value)
{
	if(QSqlDatabase::database("database").open())
	{
        QString command = "INSERT INTO $table_name (name, value) VALUES ('$name', $value) ON DUPLICATE KEY UPDATE value=$value;";
        command.replace("$table_name", GSET_tb);
		command.replace("$name", name);
		command.replace("$value", QString::number(value));
		QSqlQuery query(QSqlDatabase::database("database"));
		if(query.exec(command))
		{
			SaveDatabaseLog("Global settings saved");
			QSqlDatabase::database("database").close();
			return true;
		}
		else
		{
			ApplicationLog::SaveLocalLog(query.lastError().text());
			QSqlDatabase::database("database").close();
			return false;
		}
    }
    else return false;
}

bool DatabaseConnector::SaveIntStatistic(QString name, int value)
{
    if(QSqlDatabase::database("database").open())
    {
        QString command = "INSERT INTO $table_name (name, value) VALUES ('$name', $value) ON DUPLICATE KEY UPDATE value=$value;";
        command.replace("$table_name", STAT_tb);
        command.replace("$name", name);
        command.replace("$value", QString::number(value));
        QSqlQuery query(QSqlDatabase::database("database"));
        if(query.exec(command))
        {
            SaveDatabaseLog("Stat saved");
            QSqlDatabase::database("database").close();
            return true;
        }
        else
        {
            ApplicationLog::SaveLocalLog(query.lastError().text());
            QSqlDatabase::database("database").close();
            return false;
        }
    }
    else return false;
}

bool DatabaseConnector::SaveDoubleStatistic(QString name, double value)
{
    if(QSqlDatabase::database("database").open())
    {
        QString command = "INSERT INTO $table_name (name, value) VALUES ('$name', $value) ON DUPLICATE KEY UPDATE value=$value;";
        command.replace("$table_name", STAT_tb);
        command.replace("$name", name);
        command.replace("$value", QString::number(value));
        QSqlQuery query(QSqlDatabase::database("database"));
        if(query.exec(command))
        {
            SaveDatabaseLog("Stat saved");
            QSqlDatabase::database("database").close();
            return true;
        }
        else
        {
            ApplicationLog::SaveLocalLog(query.lastError().text());
            QSqlDatabase::database("database").close();
            return false;
        }
    }
    else return false;
}

bool DatabaseConnector::SaveStringStatistic(QString name, QString value)
{
    if(QSqlDatabase::database("database").open())
    {
        QString command = "INSERT INTO $table_name (name, value) VALUES ('$name', $value) ON DUPLICATE KEY UPDATE value=$value;";
        command.replace("$table_name", STAT_tb);
        command.replace("$name", name);
        command.replace("$value", value);
        QSqlQuery query(QSqlDatabase::database("database"));
        if(query.exec(command))
        {
            SaveDatabaseLog("Stat saved");
            QSqlDatabase::database("database").close();
            return true;
        }
        else
        {
            ApplicationLog::SaveLocalLog(query.lastError().text());
            QSqlDatabase::database("database").close();
            return false;
        }
    }
    else return false;
}

int DatabaseConnector::ReadIntStatistic(QString name)
{
    if(QSqlDatabase::database("database").open())
    {
        int value = 0;
        QString command = "SELECT * FROM $table_name WHERE name = '$name';";
        command.replace("$table_name", STAT_tb);
        command.replace("$name", name);
        QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next()) value = query.value(2).toInt();
        QSqlDatabase::database("database").close();
        return value;
    }
    else return 0;
}

double DatabaseConnector::ReadDoubleStatistic(QString name)
{
    if(QSqlDatabase::database("database").open())
    {
        double value = 0.0;
        QString command = "SELECT * FROM $table_name WHERE name = '$name';";
        command.replace("$table_name", STAT_tb);
        command.replace("$name", name);
        QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next()) value = query.value(2).toDouble();
        QSqlDatabase::database("database").close();
        return value;
    }
    else return 0.0;
}

QString DatabaseConnector::ReadStringStatistic(QString name)
{
    if(QSqlDatabase::database("database").open())
    {
        QString value = "";
        QString command = "SELECT * FROM $table_name WHERE name = '$name';";
        command.replace("$table_name", STAT_tb);
        command.replace("$name", name);
        QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next()) value = query.value(2).toString();
        QSqlDatabase::database("database").close();
        return value;
    }
    else return "";
}

bool DatabaseConnector::RemoveImageTemp(QString name)
{
    if(QSqlDatabase::database("database").open())
    {
        QString command = "DELETE FROM $table_name WHERE name = '$name';";
        command.replace("$table_name", IMG_tb);
        command.replace("$name", name);
        QSqlQuery query(QSqlDatabase::database("database"));
        if(query.exec(command))
        {
            SaveDatabaseLog("Setting removed from table global_settings");
            QSqlDatabase::database("database").close();
            return true;
        }
        else
        {
            ApplicationLog::SaveLocalLog(query.lastError().text());
            QSqlDatabase::database("database").close();
            return false;
        }
    }
    else return false;
}

bool DatabaseConnector::SaveImageTemp(QString name)
{
    if(QSqlDatabase::database("database").open())
    {
        QString command = "INSERT INTO $table_name (name, value) VALUES ('$name', '$value') ON DUPLICATE KEY UPDATE value='$value';";
        command.replace("$table_name", IMG_tb);
        command.replace("$name", name);
        command.replace("$value", "");
        QSqlQuery query(QSqlDatabase::database("database"));
        if(query.exec(command))
        {
            SaveDatabaseLog("Global settings saved");
            QSqlDatabase::database("database").close();
            return true;
        }
        else
        {
            ApplicationLog::SaveLocalLog(query.lastError().text());
            QSqlDatabase::database("database").close();
            return false;
        }
    }
    else return false;
}

QString DatabaseConnector::ReadImageTemp(QString name)
{
    if(QSqlDatabase::database("database").open())
    {
        QString value = "";
        QString command = "SELECT * FROM $table_name WHERE name = '$name';";
        command.replace("$table_name", IMG_tb);
        command.replace("$name", name);
        QSqlQuery query(command, QSqlDatabase::database("database"));
        while(query.next()) value = query.value(1).toString();
        QSqlDatabase::database("database").close();
        return value;
    }
    else return "";
}


