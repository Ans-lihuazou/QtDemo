#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

signals:
    void findLoop(bool);//循环
    void findBigSmall(bool);//区分大小写
    void findString(QString);//查找的内容
    void replaceString(QString);//替换
    void replaceAll(QString);//替换所有
    void starFind();//搜索

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEditReplace_textChanged(const QString &arg1);

    void on_cbLoop_clicked();

    void on_cbBm_clicked();

    void on_pbFind_clicked();

    void on_pbReplace_clicked();

    void on_pbReplaceAll_clicked();

    void on_pbClose_clicked();

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACEDIALOG_H
