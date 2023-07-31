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
#include "Dialogs/AddMenu/addmenudialog.h"
#include "Dialogs/FinishOrder/finishorderdialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, User *user)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    currentUser = user;
    loginTime = 0;
	SetupWindow();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::SetupWindow()
{
    ui->label_copyright->setText("©" + QCoreApplication::organizationName() + " 2023\n" + QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion());

	RefreshOrders();
    ui->pushButton_statistics->setVisible(currentUser->admin);

    QTimer *timeTimer = new QTimer(this);
    connect(timeTimer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    timeTimer->start(1000);
}

void MainWindow::UpdateTime()
{
	QString currentDate = QDate::currentDate().toString("dd.MM.yyyy");
	QString currentTime = QTime::currentTime().toString("hh:mm");
	ui->label_clock->setText(currentDate + " " + currentTime);
    CheckLoginTime();
}

QListWidgetItem *MainWindow::PrepareOrderDisplay(Order* order)
{
	QListWidgetItem *item = new QListWidgetItem;
	item->setIcon(QPixmap(":/PNG/IMG/question_mark.png").scaled(64, 64));

	QList<Client*> clientsFound = connector.ReadClientsWhere(connector.CLI_tb, "id = " + QString::number(order->clientID));
	QList<Computer*> computersFound = connector.ReadComputersWhere("id = " + QString::number(order->computerID));

	QString display = order->orderNumber + "\n";

	if(clientsFound.count() > 0)
	{
		Client *client = clientsFound.first();
		display += client->surname + " " + client->name + "\n";
	}
	else display += "CLIENT NOT FOUND\n";

	if(computersFound.count() > 0)
	{
		Computer *computer = computersFound.first();
		display += computer->producer + " " + computer->model + "\n";
		item->setIcon(serviceCore.GetProducerIcon(computer->producer));
	}
	else display += "COMPUTER NOT FOUND\n";

	display += order->serialNumber;

	item->setText(display);

	return item;
}

void MainWindow::RefreshOrders()
{
	ui->listWidget_to_diagnosis->clear();
	ui->listWidget_to_repair->clear();
	ui->listWidget_done->clear();

	currentDiagnosisOrders = connector.ReadOrders(connector.CDO_tb);
	currentRepairOrders = connector.ReadOrders(connector.CRO_tb);
	currentCompletedOrders = connector.ReadOrders(connector.CCO_tb);

	foreach(Order *order, currentDiagnosisOrders)
	{
		QListWidgetItem *item = PrepareOrderDisplay(order);
		item->setToolTip(order->symptoms);
		ui->listWidget_to_diagnosis->addItem(item);
	}
	foreach(Order *order, currentRepairOrders)
	{
		QListWidgetItem *item = PrepareOrderDisplay(order);
		item->setToolTip(order->diagnosis);
		ui->listWidget_to_repair->addItem(item);
	}
	foreach(Order *order, currentCompletedOrders)
	{
		QListWidgetItem *item = PrepareOrderDisplay(order);
		item->setToolTip(order->repairDescription);
		ui->listWidget_done->addItem(item);
	}
}

void MainWindow::on_pushButton_add_clicked()
{
	AddMenuDialog addMenuDialog;
	addMenuDialog.exec();
	RefreshOrders();
}

void MainWindow::on_pushButton_search_clicked()
{
    SearchDialog searchDialog(currentUser);
	searchDialog.exec();
	RefreshOrders();
}

void MainWindow::on_pushButton_statistics_clicked()
{
    StatisticsDialog dialog;
    dialog.exec();
}

void MainWindow::on_listWidget_to_diagnosis_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    OrderMenuDialog orderMenuDialog(this, currentUser->admin);
	orderMenuDialog.exec();
	int selectedOption = orderMenuDialog.selectedOption;
	if(selectedOption == 1)
	{
        OrderInfoDialog orderInfoDialog(currentDiagnosisOrders[ui->listWidget_to_diagnosis->currentRow()], currentUser);
		orderInfoDialog.exec();
	}
	else if(selectedOption == 2)
	{
		MoveToRepairDialog moveToRepairDialog(currentDiagnosisOrders[ui->listWidget_to_diagnosis->currentRow()]);
		moveToRepairDialog.exec();
	}
	else if(selectedOption == 3)
    {
        AskDialog askDialog(tr("Are you sure you want to move the order to cancelled?"));
        askDialog.exec();
        if(askDialog.accepted)
        {
            connector.MoveOrder(currentDiagnosisOrders[ui->listWidget_to_diagnosis->currentRow()], connector.CO_tb);
            stat.AddMonthIntStatistic("canceled", 1);
        }
	}
	else if(selectedOption == 4)
    {
        AskDialog askDialog(tr("Are you sure you want to delete the order?"));
        askDialog.exec();
        if(askDialog.accepted)
        {
            connector.MoveOrder(currentDiagnosisOrders[ui->listWidget_to_diagnosis->currentRow()], connector.RMO_tb);
            stat.AddMonthIntStatistic("removed", 1);
        }
	}
	RefreshOrders();
}

void MainWindow::on_listWidget_to_repair_itemDoubleClicked(QListWidgetItem *item)
{
	Q_UNUSED(item);
    OrderMenuDialog orderMenuDialog(this, currentUser->admin);
    orderMenuDialog.exec();
	int selectedOption = orderMenuDialog.selectedOption;
	if(selectedOption == 1)
	{
        OrderInfoDialog orderInfoDialog(currentRepairOrders[ui->listWidget_to_repair->currentRow()], currentUser);
		orderInfoDialog.exec();
	}
	else if(selectedOption == 2)
	{
		MoveToCompletedDialog moveToCompletedDialog(currentRepairOrders[ui->listWidget_to_repair->currentRow()]);
		moveToCompletedDialog.exec();
	}
	else if(selectedOption == 3)
    {
        AskDialog askDialog(tr("Are you sure you want to move the order to cancelled?"));
        askDialog.exec();
        if(askDialog.accepted)
        {
            connector.MoveOrder(currentRepairOrders[ui->listWidget_to_repair->currentRow()], connector.CO_tb);
            stat.AddMonthIntStatistic("canceled", 1);
        }
	}
	else if(selectedOption == 4)
    {
        AskDialog askDialog(tr("Are you sure you want to delete the order?"));
        askDialog.exec();
        if(askDialog.accepted)
        {
            connector.MoveOrder(currentRepairOrders[ui->listWidget_to_repair->currentRow()], connector.RMO_tb);
            stat.AddMonthIntStatistic("removed", 1);
        }
	}
	RefreshOrders();
}


void MainWindow::on_listWidget_done_itemDoubleClicked(QListWidgetItem *item)
{
	Q_UNUSED(item);
    OrderMenuDialog orderMenuDialog(this, currentUser->admin);
	orderMenuDialog.exec();
	int selectedOption = orderMenuDialog.selectedOption;
	if(selectedOption == 1)
	{
        OrderInfoDialog orderInfoDialog(currentCompletedOrders[ui->listWidget_done->currentRow()], currentUser);
		orderInfoDialog.exec();
	}
	else if(selectedOption == 2)
	{
		FinishOrderDialog finishOrderDialog(currentCompletedOrders[ui->listWidget_done->currentRow()]);
		finishOrderDialog.exec();
	}
	else if(selectedOption == 3)
    {
        AskDialog askDialog(tr("Are you sure you want to move the order to cancelled?"));
        askDialog.exec();
        if(askDialog.accepted)
        {
            connector.MoveOrder(currentCompletedOrders[ui->listWidget_done->currentRow()], connector.CO_tb);
            stat.AddMonthIntStatistic("canceled", 1);
        }
	}
	else if(selectedOption == 4)
    {
        AskDialog askDialog(tr("Are you sure you want to delete the order?"));
        askDialog.exec();
        if(askDialog.accepted)
        {
            connector.MoveOrder(currentCompletedOrders[ui->listWidget_done->currentRow()], connector.RMO_tb);
            stat.AddMonthIntStatistic("removed", 1);
        }
	}
	RefreshOrders();
}

void MainWindow::on_pushButton_logout_clicked()
{
	qApp->quit();
	QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_pushButton_full_window_clicked()
{
	if(this->windowState() == Qt::WindowFullScreen)
	{
		this->setWindowState(Qt::WindowMaximized);
		ui->pushButton_full_window->setIcon(QPixmap(":/PNG/IMG/expand.png"));
	}
	else
	{
		this->setWindowState(Qt::WindowFullScreen);
		ui->pushButton_full_window->setIcon(QPixmap(":/PNG/IMG/collapse.png"));
	}
}

void MainWindow::on_pushButton_settings_clicked()
{
    SettingsDialog dialog(this, currentUser);
    dialog.exec();
}

void MainWindow::CheckLoginTime()
{
    loginTime++;
    int logoutTime = settings.value("logout_time").toInt() * 60;

    QPoint currentMousePosition = mapFromGlobal(QCursor::pos());
    if(currentMousePosition.x() != mousePosition.x() || currentMousePosition.y() != mousePosition.y())
    {
        mousePosition = currentMousePosition;
        loginTime = 0;
    }

    if(loginTime > logoutTime && logoutTime != 0)
    {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void MainWindow::on_pushButton_info_clicked()
{
    AboutDialog about;
    about.exec();
}

void MainWindow::on_pushButton_refresh_clicked()
{
    RefreshOrders();
}

