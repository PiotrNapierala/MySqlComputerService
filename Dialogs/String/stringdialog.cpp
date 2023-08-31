#include "stringdialog.h"
#include "ui_stringdialog.h"

StringDialog::StringDialog(QString info, QLineEdit::EchoMode echo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringDialog)
{
    ui->setupUi(this);
    ui->label_info->setText(info);
    ui->lineEdit_input->setEchoMode(echo);
    ui->lineEdit_input->setFocus();
    if(ui->lineEdit_input->echoMode() == QLineEdit::Password) ui->lineEdit_input->setAlignment(Qt::AlignCenter);
}

StringDialog::~StringDialog()
{
    delete ui;
}

void StringDialog::on_pushButton_ok_clicked()
{
    text = ui->lineEdit_input->text();
    accepted = true;
    this->close();
}

