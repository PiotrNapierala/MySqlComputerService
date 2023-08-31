#ifndef STRINGDIALOG_H
#define STRINGDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class StringDialog;
}

class StringDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StringDialog(QString info, QLineEdit::EchoMode echo, QWidget *parent = nullptr);
    ~StringDialog();

    QString text;
    bool accepted = false;

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::StringDialog *ui;
};

#endif // STRINGDIALOG_H
