#include "findform.h"
#include "ui_findform.h"

FindForm::FindForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindForm)
{
    ui->setupUi(this);
}

FindForm::~FindForm()
{
    delete ui;
}
