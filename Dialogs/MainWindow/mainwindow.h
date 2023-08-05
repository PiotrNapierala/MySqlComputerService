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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QListWidgetItem>
#include <QProcess>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QMouseEvent>
#include <QSettings>

#include "Libraries/DatabaseConnector/databaseconnector.h"
#include "Libraries/ServiceCore/servicecore.h"
#include "Libraries/EmailConnector/emailconnector.h"
#include "Libraries/StatisticConnector/statisticconnector.h"

#include "Classes/Client/client.h"
#include "Classes/Order/order.h"
#include "Classes/Computer/computer.h"
#include "Classes/User/user.h"

#include "Dialogs/AddMenu/addmenudialog.h"
#include "Dialogs/MoveToRepair/movetorepairdialog.h"
#include "Dialogs/MoveToCompleted/movetocompleteddialog.h"
#include "Dialogs/OrderMenu/ordermenudialog.h"
#include "Dialogs/OrderInfo/orderinfodialog.h"
#include "Dialogs/Search/searchdialog.h"
#include "Dialogs/Ask/askdialog.h"
#include "Dialogs/FinishOrder/finishorderdialog.h"
#include "Dialogs/Settings/settingsdialog.h"
#include "Dialogs/Statistic/statisticsdialog.h"
#include "Dialogs/About/aboutdialog.h"
#include "Dialogs/Install/installdialog.h"
#include "Dialogs/Info/infodialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, User* user = nullptr);
	~MainWindow();

private slots:
	void on_pushButton_add_clicked();
	void on_pushButton_search_clicked();
	void on_pushButton_statistics_clicked();
	void on_listWidget_to_diagnosis_itemDoubleClicked(QListWidgetItem *item);
	void on_listWidget_to_repair_itemDoubleClicked(QListWidgetItem *item);
	void on_listWidget_done_itemDoubleClicked(QListWidgetItem *item);
	void on_pushButton_logout_clicked();
	void on_pushButton_full_window_clicked();
    void on_pushButton_settings_clicked();
    void UpdateTime();
    void on_pushButton_info_clicked();
    void on_pushButton_refresh_clicked();
    void on_pushButton_update_clicked();

private:
	Ui::MainWindow *ui;
	DatabaseConnector connector;
	ServiceCore serviceCore;
    StatisticConnector stat;
    QSettings settings;

    User *currentUser;
    int loginTime = 0;
    QPoint mousePosition;

	QList<Order*> currentDiagnosisOrders, currentRepairOrders, currentCompletedOrders;

	void SetupWindow();
	void RefreshOrders();
	QListWidgetItem *PrepareOrderDisplay(Order* order);
    void CheckLoginTime();

};
#endif // MAINWINDOW_H
