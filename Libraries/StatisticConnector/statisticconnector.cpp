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

#include "statisticconnector.h"

void StatisticConnector::AddMonthIntStatistic(QString name, int value)
{
    QString statName = name + "_" + QDate::currentDate().toString("MM_yyyy");
    connector.SaveIntStatistic(statName, connector.ReadIntStatistic(statName) + value);
}

void StatisticConnector::AddIntStatistic(QString name, int value)
{
    connector.SaveIntStatistic(name, connector.ReadIntStatistic(name) + value);
}

void StatisticConnector::AddMonthDoubleStatisitic(QString name, double value)
{
    QString statName = name + "_" + QDate::currentDate().toString("MM_yyyy");
    connector.SaveDoubleStatistic(statName, connector.ReadDoubleStatistic(statName) + value);
}

void StatisticConnector::AddDoubleStatisitic(QString name, double value)
{
    connector.SaveDoubleStatistic(name, connector.ReadDoubleStatistic(name) + value);
}

void StatisticConnector::SetStringStatisitc(QString name, QString value)
{
    connector.SaveStringStatistic(name, value);
}

QList<int> StatisticConnector::ReadIntStatisticFromYear(QString name, int year)
{
    QList<int> toReturn;

    for(int i = 1; i <= 12; i++)
    {
        QString month = QString::number(i);
        if(i < 10) month = "0" + month;

        QString statName = name + "_" + month + "_" + QString::number(year);
        toReturn << connector.ReadIntStatistic(statName);
    }

    return toReturn;
}

QList<double> StatisticConnector::ReadDoubleStatisticFromYear(QString name, int year)
{
    QList<double> toReturn;

    for(int i = 1; i <= 12; i++)
    {
        QString month = QString::number(i);
        if(i < 10) month = "0" + month;

        QString statName = name + "_" + month + "_" + QString::number(year);
        toReturn << connector.ReadDoubleStatistic(statName);
    }

    return toReturn;
}

int StatisticConnector::ReadIntStatistic(QString name)
{
    return connector.ReadIntStatistic(name);
}

double StatisticConnector::ReadDoubleStatisitc(QString name)
{
    return connector.ReadDoubleStatistic(name);
}

QString StatisticConnector::ReadStringStatisitic(QString name)
{
    return connector.ReadStringStatistic(name);
}

