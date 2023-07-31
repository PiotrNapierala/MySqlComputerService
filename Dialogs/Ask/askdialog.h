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

#ifndef ASKDIALOG_H
#define ASKDIALOG_H

#include <QDialog>

namespace Ui {
class AskDialog;
}

class AskDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AskDialog(QString text, QWidget *parent = nullptr);
	~AskDialog();

	bool accepted = false;

private slots:
	void on_pushButton_accept_clicked();
	void on_pushButton_cancel_clicked();

private:
	Ui::AskDialog *ui;
};

#endif // ASKDIALOG_H
