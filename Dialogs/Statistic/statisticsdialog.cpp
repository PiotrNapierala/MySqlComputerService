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

#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

StatisticsDialog::StatisticsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsDialog)
{
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(0);
    ui->spinBox->setValue(QDate::currentDate().year());
    RefreshDialog(ui->comboBox->currentIndex(), ui->spinBox->value());
}

StatisticsDialog::~StatisticsDialog()
{
    delete ui;
}

QChartView *StatisticsDialog::CreateIntBarChart(QStringList names, QList<int> values, QString barName, QString chartName)
{
    QBarSet* barSet = new QBarSet(barName);

    for (int i = 0; i < names.size(); i++)
    *barSet << values[i];

    QBarSeries* barSeries = new QBarSeries();
    barSeries->append(barSet);

    QChart* chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle(chartName);

    QBarCategoryAxis* xAxis = new QBarCategoryAxis();
    xAxis->append(names);
    chart->addAxis(xAxis, Qt::AlignBottom);
    barSeries->attachAxis(xAxis);

    QValueAxis* yAxis = new QValueAxis();
    chart->addAxis(yAxis, Qt::AlignLeft);
    barSeries->attachAxis(yAxis);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

QChartView *StatisticsDialog::CreateDoubleBarChart(QStringList names, QList<double> values, QString barName, QString chartName)
{
    QBarSet* barSet = new QBarSet(barName);

    for (int i = 0; i < names.size(); i++)
    *barSet << values[i];

    QBarSeries* barSeries = new QBarSeries();
    barSeries->append(barSet);

    QChart* chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle(chartName);

    QBarCategoryAxis* xAxis = new QBarCategoryAxis();
    xAxis->append(names);
    chart->addAxis(xAxis, Qt::AlignBottom);
    barSeries->attachAxis(xAxis);

    QValueAxis* yAxis = new QValueAxis();
    chart->addAxis(yAxis, Qt::AlignLeft);
    barSeries->attachAxis(yAxis);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

QStringList StatisticsDialog::GetNamesFromYear(int year)
{
    QStringList names;
    for(int i = 1; i <= 12; i++)
    {
        if(i < 10) names << "0" + QString::number(i) + "." + QString::number(year);
        else names << QString::number(i) + "." + QString::number(year);
    }
    return names;
}

void StatisticsDialog::RefreshDialog(int index, int year)
{
    ClearLayout();

    switch(index)
    {
    case 0:
        ui->gridLayout_chart->addWidget(CreateIntBarChart(GetNamesFromYear(year), stat.ReadIntStatisticFromYear("done_orders", year), tr("Quantity"), ui->comboBox->currentText()));
    break;
    case 1:
    ui->gridLayout_chart->addWidget(CreateIntBarChart(GetNamesFromYear(year), stat.ReadIntStatisticFromYear("canceled", year), tr("Quantity"), ui->comboBox->currentText()));
    break;
    case 2:
    ui->gridLayout_chart->addWidget(CreateIntBarChart(GetNamesFromYear(year), stat.ReadIntStatisticFromYear("removed", year), tr("Quantity"), ui->comboBox->currentText()));
    break;
    case 3:
    ui->gridLayout_chart->addWidget(CreateDoubleBarChart(GetNamesFromYear(year), stat.ReadDoubleStatisticFromYear("income", year), tr("$"), ui->comboBox->currentText()));
    break;
    }
}

void StatisticsDialog::ClearLayout()
{
    while(ui->gridLayout_chart->count())
    {
        QWidget* widget = ui->gridLayout_chart->itemAt(0)->widget();
        if(widget)
        {
            ui->gridLayout_chart->removeWidget(widget); delete widget;
        }
    }
}

void StatisticsDialog::on_comboBox_currentIndexChanged(int index)
{
    RefreshDialog(index, ui->spinBox->value());
}

void StatisticsDialog::on_spinBox_valueChanged(int arg1)
{
    RefreshDialog(ui->comboBox->currentIndex(), arg1);
}

