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

#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>
#include <QDate>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include "Libraries/StatisticConnector/statisticconnector.h"

namespace Ui {
class StatisticsDialog;
}

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsDialog(QWidget *parent = nullptr);
    ~StatisticsDialog();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::StatisticsDialog *ui;
    StatisticConnector stat;

    QChartView *CreateIntBarChart(QStringList names, QList<int> values, QString barName, QString chartName);
    QChartView *CreateDoubleBarChart(QStringList names, QList<double> values, QString barName, QString chartName);
    QStringList GetNamesFromYear(int year);
    void RefreshDialog(int index, int year);
    void ClearLayout();

};

#endif // STATISTICSDIALOG_H
