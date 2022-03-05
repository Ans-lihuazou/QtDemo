#ifndef MAINWINDOWALL_H
#define MAINWINDOWALL_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QAction>
#include <QList>
#include <QTextBlock>
#include <QTextDocument>
#include "mainwindow.h"

#define MAXLENGTH 10000

namespace Ui {
class MainWindowAll;
}

class MainWindowAll : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowAll(QWidget *parent = nullptr);
    ~MainWindowAll();

private slots:


    void on_actAll_new_triggered();

    void on_actAll_open_triggered();

    void on_actAll_initMode_triggered();

    void on_actAll_tile_triggered();

    void on_actAll_MDI_triggered(bool checked);

    void on_actAll_line_triggered(bool checked);

    void on_actAll_compare_triggered(bool checked);

private:
    Ui::MainWindowAll *ui;
    bool isInit = true;
    QList<qint32> LCS(QStringList strList1,QStringList strList2);
    void getCurrList(MainWindow* currWindow,QStringList& currList);
    void reduction();
};

#endif // MAINWINDOWALL_H
