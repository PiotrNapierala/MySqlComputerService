#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Language;
}

class Language : public QDialog
{
    Q_OBJECT

public:
    explicit Language(QWidget *parent = nullptr);
    ~Language();

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::Language *ui;
};

#endif // LANGUAGE_H
