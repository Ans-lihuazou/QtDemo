#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    ui->pbFind->setEnabled(false);
    ui->pbReplace->setEnabled(false);
    ui->pbReplaceAll->setEnabled(false);
    ui->cbLoop->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_lineEdit_textChanged(const QString &arg1)
{
    ui->pbFind->setEnabled(!arg1.isEmpty());
    ui->pbReplace->setEnabled(!arg1.isEmpty()&&!ui->lineEditReplace->text().isEmpty());
    ui->pbReplaceAll->setEnabled(!arg1.isEmpty()&&!ui->lineEditReplace->text().isEmpty());
    emit findString(arg1);
}


void ReplaceDialog::on_lineEditReplace_textChanged(const QString &arg1)
{
    ui->pbReplace->setEnabled(!arg1.isEmpty()&&!ui->lineEditReplace->text().isEmpty());
    ui->pbReplaceAll->setEnabled(!arg1.isEmpty()&&!ui->lineEditReplace->text().isEmpty());
}

void ReplaceDialog::on_cbLoop_clicked()
{
    emit findLoop(ui->cbLoop->isChecked());
}

void ReplaceDialog::on_cbBm_clicked()
{
    emit findBigSmall(ui->cbBm->isChecked());
}

void ReplaceDialog::on_pbFind_clicked()
{
    emit starFind();
}

void ReplaceDialog::on_pbReplace_clicked()
{
    emit replaceString(ui->lineEditReplace->text());
}

void ReplaceDialog::on_pbReplaceAll_clicked()
{
    emit replaceAll(ui->lineEditReplace->text());
}

void ReplaceDialog::on_pbClose_clicked()
{
    close();
}
