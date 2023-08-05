#include "installdialog.h"
#include "ui_installdialog.h"

InstallDialog::InstallDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstallDialog)
{
    ui->setupUi(this);
    RunInstall();
}

InstallDialog::~InstallDialog()
{
    delete ui;
}

void InstallDialog::RunInstall()
{
    this->show();
    if(serviceCore.DownloadNewVersion())
    {
        installOK = true;
        QProcess::startDetached(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/MySqlComputerService_latest_" + QDate::currentDate().toString("dd_MM_yyyy") + ".exe");
        exit(0);
    }
    else
    {
        installOK = false;
        this->close();
    }
}
