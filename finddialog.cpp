#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    ui->pbFind->setEnabled(false);
    ui->cbLoop->setChecked(true);
    ui->rbNext->setChecked(true);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::setFindStr(QString str)
{
    ui->lineEdit->setText(str);
}

void FindDialog::on_lineEdit_textChanged(const QString &arg1)
{
    ui->pbFind->setEnabled(!arg1.isEmpty());
    emit findString(ui->lineEdit->text());
}


void FindDialog::on_rbPre_clicked()
{
    emit findToNext(ui->rbNext->isChecked());
}

void FindDialog::on_cbLoop_clicked()
{
    emit findLoop(ui->cbLoop->isChecked());
}

void FindDialog::on_rbNext_clicked()
{
    emit findToNext(ui->rbNext->isChecked());
}

void FindDialog::on_cbBm_clicked()
{
    emit findBigSmall(ui->cbBm->isChecked());
}

void FindDialog::on_pbFind_clicked()
{
    emit starFind();
}
