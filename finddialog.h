#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();
    void setFindStr(QString str);

signals:
    void findToNext(bool);//向后找
    void findLoop(bool);//循环
    void findBigSmall(bool);//区分大小写
    void findString(QString);//查找的内容
    void starFind();//搜索

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_rbPre_clicked();

    void on_rbNext_clicked();

    void on_cbLoop_clicked();

    void on_cbBm_clicked();

    void on_pbFind_clicked();

private:
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
