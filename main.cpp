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

#include "Dialogs/MainWindow/mainwindow.h"
#include "Dialogs/FirstRun/firstrundialog.h"
#include "Dialogs/Login/logindialog.h"
#include "Dialogs/AddUser/adduserdialog.h"
#include "Dialogs/Language/language.h"

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Libraries/ApplicationLog/applicationlog.h"

#include <QApplication>
#include <QSettings>
#include <QSqlDatabase>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("Piotr Napierala");
	QCoreApplication::setApplicationName("MySqlComputerService");
    QCoreApplication::setApplicationVersion("1.2.1");

    QSettings settings;
    QTranslator translator;
    DatabaseConnector connector;

    if(settings.value("language").toInt() == 0)
    {
        Language languageDialog;
        languageDialog.exec();
    }

    if(settings.value("language").toInt() == 0) exit(0);

    switch(settings.value("language").toInt())
    {
    case 2:
        translator.load(":/translations/Translations/polish.qm");
        a.installTranslator(&translator);
        break;
    }

    while(settings.value("database/database_server_adress").toString().isEmpty() || settings.value("database/database_server_port").toInt() == 0 ||
           settings.value("database/database_name").toString().isEmpty() || settings.value("database/database_user").toString().isEmpty() ||
           settings.value("database/database_password").toString().isEmpty())
    {
        FirstRunDialog dialog(nullptr, 0);
        dialog.exec();
        if(!dialog.accepted) exit(1);
    }

    while(!connector.SetupConnection())
    {
        FirstRunDialog dialog(nullptr, 0);
        dialog.exec();
        if(!dialog.accepted) exit(1);
    }

    connector.CreateTables();

    while(connector.GetUsersCount() == 0)
    {
        AddUserDialog dialog(nullptr, true);
        dialog.exec();
        if(!dialog.accepted) exit(1);
    }

    while(connector.ReadIntGlobalSettings("isSet") == false)
    {
        FirstRunDialog dialog(nullptr, 1);
        dialog.exec();
        if(!dialog.accepted) exit(1);
    }

    LoginDialog loginDialog;
    loginDialog.exec();
    if(loginDialog.loginCorrect == true)
    {
        MainWindow w(nullptr, loginDialog.currentUser);
        w.showMaximized();
        return a.exec();
    }
    else return 1;
}
