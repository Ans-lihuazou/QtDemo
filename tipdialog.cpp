#include "tipdialog.h"
#include "ui_tipdialog.h"

TipDialog::TipDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TipDialog)
{
    ui->setupUi(this);
    ui->plainTextEdit->setReadOnly(true);
}

TipDialog::~TipDialog()
{
    delete ui;
}

void TipDialog::setTip(QString tip)
{
    QString str = "是否将更改保存到:"+tip;
    ui->plainTextEdit->setReadOnly(false);
    ui->plainTextEdit->appendPlainText(str);
    ui->plainTextEdit->setReadOnly(true);
}

void TipDialog::on_pbSave_clicked()
{
    emit accept();
}

void TipDialog::on_pbNotSave_clicked()
{
    emit reject();
}

void TipDialog::on_pbCancle_clicked()
{
    emit close();
}
