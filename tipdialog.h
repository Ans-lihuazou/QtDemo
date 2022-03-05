#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QDialog>

namespace Ui {
class TipDialog;
}

class TipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TipDialog(QWidget *parent = nullptr);
    ~TipDialog(); 
    void setTip(QString tip);

private slots:
    void on_pbSave_clicked();

    void on_pbNotSave_clicked();

    void on_pbCancle_clicked();

private:
    Ui::TipDialog *ui;
};

#endif // TIPDIALOG_H
