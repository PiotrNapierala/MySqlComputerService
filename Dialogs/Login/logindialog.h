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

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSettings>

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Libraries/ServiceCore/servicecore.h"
#include "Libraries/MyCrypto/mycrypto.h"
#include "Libraries/Totp/totp.h"

#include "Dialogs/FirstRun/firstrundialog.h"
#include "Dialogs/AddUser/adduserdialog.h"
#include "Dialogs/String/stringdialog.h"

#include "Classes/User/user.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget *parent = nullptr);
	~LoginDialog();

	bool loginCorrect = false;
	User *currentUser;

private slots:
	void on_pushButton_login_clicked();

private:
	Ui::LoginDialog *ui;
	DatabaseConnector connector;
    ServiceCore serviceCore;
    QSettings settings;
    TOTP mytotp;

    void CheckTempDatabasePassword();

};

#endif // LOGINDIALOG_H
