#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QFontDialog>
#include <QFont>
#include <QDate>
#include <QDateTime>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QList>
#include <QListView>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QBrush>
#include <QPlainTextEdit>
#include "tipdialog.h"
#include "finddialog.h"
#include "replacedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool getisReaded();
    void readFile(QString fileName);
    QTextDocument* getPlainText();
    void setStackPage(int id);
    void setDiffText(QList<qint32> diffList);


private slots:
    void on_actNewFile_triggered();

    void on_actOpenFile_triggered();

    void on_actSave_triggered();

    void on_actOtherSave_triggered();

    void on_plainTextEdit_copyAvailable(bool b);

    void on_actDelete_triggered();

    void on_timer_timeout();

    void on_actLast_triggered();

    void on_actLine_triggered(bool checked);

    void on_actState_triggered(bool checked);

    void on_actFont_triggered();

    void on_actBig_triggered();

    void on_actSmall_triggered();

    void on_actInit_triggered();

    void on_actClose_triggered();

    void on_actSelectAll_triggered();

    void on_actDate_triggered();

    void on_actFind_triggered();

    void on_findNext_set(bool flag);

    void on_findLoop_set(bool flag);

    void on_findBm_set(bool flag);

    void on_findStr_set(QString str);

    void on_startFind();

    void on_actFindLast_triggered();

    void on_actFindNext_triggered();

    void on_replaceLoop_set(bool flag);

    void on_replaceBm_set(bool flag);

    void on_repfindStr_set(QString str);

    void on_replaceFind();

    void on_starReplace(QString replaceStr);

    void on_starReplaceAll(QString replaceStr);

    void on_labelTimer_timeout();

    void on_actReplace_triggered();

    void on_plainTextEdit_textChanged();

private:
    Ui::MainWindow *ui;

    QString initString="";
    QString nowFileName;

    QStringList strLastList;
    QTimer *timer;

    QFont *font;
    qreal up=1;

    //find
    FindDialog *findDialog;
    bool findNext=true;//向后找
    bool findLoop=true;//循环找
    bool findBm=false;//区分大小写
    QString findStr;//查找的内容

    //replace
    ReplaceDialog *replaceDialog;
    bool replaceLoop=true;//循环替换
    bool replaceBm=false;
    QString repFindStr;//查找的内容
    bool remAll = false;

    //StateBar
    QLabel *labelCountFir;
    QLabel *labelCountSec;
    QLabel *labelDateFir;
    QLabel *labelDateSec;
    QTimer *labelTimer;

//    QHBoxLayout *hLayout;
//    QWidget *widget;
    bool isReaded=false;
    QStringListModel *listModel;
    QStandardItemModel *itemModel;

};
#endif // MAINWINDOW_H
