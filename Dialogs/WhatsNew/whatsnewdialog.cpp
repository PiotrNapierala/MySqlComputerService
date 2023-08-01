#include "whatsnewdialog.h"
#include "ui_whatsnewdialog.h"

WhatsNewDialog::WhatsNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WhatsNewDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("ver" + QCoreApplication::applicationVersion());
    LoadData();
}

WhatsNewDialog::~WhatsNewDialog()
{
    delete ui;
}

void WhatsNewDialog::LoadData()
{
    QSettings whatsnew(qApp->applicationDirPath() + "/whatsnew.ini", QSettings::IniFormat);
    versions = whatsnew.value("versions").toStringList();
    infoList = whatsnew.value("infoList").toStringList();

    if(versions.count() != infoList.count()) this->close();
    if(versions.count() < 1) this->close();

    ui->comboBox_ver->addItems(versions);
    ui->comboBox_ver->setCurrentIndex(ui->comboBox_ver->count()-1);
    ui->textEdit_whats_new->setText(infoList[ui->comboBox_ver->currentIndex()]);
}

void WhatsNewDialog::on_comboBox_ver_currentIndexChanged(int index)
{
    ui->textEdit_whats_new->setText(infoList[index]);
}
