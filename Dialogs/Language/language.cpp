#include "language.h"
#include "ui_language.h"

Language::Language(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Language)
{
    ui->setupUi(this);
}

Language::~Language()
{
    delete ui;
}

void Language::on_pushButton_ok_clicked()
{
    QSettings settings;
    settings.setValue("language", ui->comboBox_language->currentIndex()+1);
    this->close();
}

