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

#ifndef ORDER_H
#define ORDER_H

#include <QObject>
#include <QDateTime>

class Order
{
public:
	QString orderNumber, diagnosis, repairDescription, remarks, systemPassword, currentTable, serialNumber, symptoms, items, pickupCode, seals;
    QStringList photos;
	QDateTime dateOfAdmission = QDateTime::currentDateTime();
	QDateTime releaseDate = QDateTime::currentDateTime();
	int id = 0;
	int clientID = 0;
	double valuation = 0;
	double paid = 0;
	int receiptFV = 0;
	int computerID = 0;
};

#endif // ORDER_H
