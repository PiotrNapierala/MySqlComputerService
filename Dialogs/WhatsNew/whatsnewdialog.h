#ifndef WHATSNEWDIALOG_H
#define WHATSNEWDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class WhatsNewDialog;
}

class WhatsNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WhatsNewDialog(QWidget *parent = nullptr);
    ~WhatsNewDialog();

private slots:
    void on_comboBox_ver_currentIndexChanged(int index);

private:
    Ui::WhatsNewDialog *ui;
    QStringList versions, infoList;

    void LoadData();
};

#endif // WHATSNEWDIALOG_H
