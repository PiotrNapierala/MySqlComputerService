#ifndef INSTALLDIALOG_H
#define INSTALLDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QDate>
#include <QStandardPaths>

#include "Libraries/ServiceCore/servicecore.h"

namespace Ui {
class InstallDialog;
}

class InstallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InstallDialog(QWidget *parent = nullptr);
    ~InstallDialog();

    bool installOK = true;

private:
    Ui::InstallDialog *ui;
    ServiceCore serviceCore;

    void RunInstall();

};

#endif // INSTALLDIALOG_H
