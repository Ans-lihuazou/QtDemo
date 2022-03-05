#include "mainwindowall.h"
#include "ui_mainwindowall.h"

MainWindowAll::MainWindowAll(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowAll)
{
    ui->setupUi(this);
    setCentralWidget(ui->mdiArea);
    setWindowTitle("notepad");

}

MainWindowAll::~MainWindowAll()
{
    delete ui;
}



void MainWindowAll::on_actAll_new_triggered()
{
    MainWindow *w = new MainWindow(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(w);
    w->show();
}

void MainWindowAll::on_actAll_open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"open",QDir::currentPath(),"文本文件(*.txt *.TXT);;所有文件(*.*)");
    if(!fileName.isEmpty())
    {
        bool needNew = true;
        MainWindow *form;
        if(ui->mdiArea->subWindowList().count()>0)
        {
            foreach(auto win,ui->mdiArea->subWindowList())
            {

                form = qobject_cast<MainWindow*>(win->widget());
                needNew = form->getisReaded();
                if(!needNew)
                    break;
            }
        }
        qDebug()<<"needNew:"<<needNew;
        if(needNew)
        {
            form = new MainWindow(this);
            form->setAttribute(Qt::WA_DeleteOnClose);
            ui->mdiArea->addSubWindow(form);
        }
        form->readFile(fileName);
        form->show();
    }
}

void MainWindowAll::on_actAll_initMode_triggered()
{
      ui->mdiArea->setViewMode(QMdiArea::SubWindowView);//默认
      isInit = true;
}

void MainWindowAll::on_actAll_tile_triggered()
{
     ui->mdiArea->tileSubWindows();
     isInit = false;
}

void MainWindowAll::on_actAll_MDI_triggered(bool checked)
{
    if(checked)
    {
        ui->actAll_tile->setEnabled(false);
        ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    }
    else
    {
        ui->actAll_tile->setEnabled(true);
        on_actAll_initMode_triggered();
        if(!isInit)
            on_actAll_tile_triggered();
    }
}

void MainWindowAll::on_actAll_line_triggered(bool checked)
{
    if(checked)
    {
        if(ui->mdiArea->currentSubWindow()==0)
        {
            ui->actAll_line->setChecked(false);
            return;
        }
        MainWindow *currWindow = qobject_cast<MainWindow*>(ui->mdiArea->currentSubWindow()->widget());
        QStringList currList;
        getCurrList(currWindow,currList);
          ui->actAll_compare->setEnabled(false);
        if(ui->mdiArea->subWindowList().count()>1)
        {
            foreach(QMdiSubWindow* win,ui->mdiArea->subWindowList())
            {

                MainWindow *form = qobject_cast<MainWindow*>(win->widget());
                if(form==currWindow) continue;
                QTextDocument* fdoc = form->getPlainText();
                QStringList compList;
                QList<qint32> diffList;
                if(fdoc->blockCount()==0)
                    continue;
                for(int i=0;i<fdoc->blockCount();i++)
                {
                    QString str = fdoc->findBlockByNumber(i).text();
                    if(!str.isEmpty()) compList<<str;
                }
                for(int i=0;i<qMin(currList.count(),compList.count());i++)
                    if(currList[i]!=compList[i])
                        diffList.append(i);
                for(int i =qMin(currList.count(),compList.count());i<compList.count();i++)
                    diffList.append(i);
                form->setDiffText(diffList);
            }
        }
    }
    else
    {
       reduction();
    }
}

void MainWindowAll::on_actAll_compare_triggered(bool checked)
{
    if(checked)
    {
        if(ui->mdiArea->currentSubWindow()==0)
        {
            ui->actAll_line->setChecked(false);
            return;
        }
        ui->actAll_line->setEnabled(false);
        MainWindow *currWindow = qobject_cast<MainWindow*>(ui->mdiArea->currentSubWindow()->widget());
        QStringList currList;
        getCurrList(currWindow,currList);
        if(ui->mdiArea->subWindowList().count()>1)
        {
            foreach(QMdiSubWindow* win,ui->mdiArea->subWindowList())
            {

                MainWindow *form = qobject_cast<MainWindow*>(win->widget());
                if(form==currWindow)
                    continue;
                QTextDocument* fdoc = form->getPlainText();
                QStringList compList;
                QList<qint32> diffList;
                if(fdoc->blockCount()==0)
                    continue;
                for(int i=0;i<fdoc->blockCount();i++)
                {
                    QString str = fdoc->findBlockByNumber(i).text();
                    if(!str.isEmpty()) compList<<str;
                }
                diffList = LCS(compList,currList);
                qDebug()<<"Main test";
                form->setDiffText(diffList);
                qDebug()<<"testEnd";
            }
        }
    }
    else
    {
        reduction();
    }
}

QList<qint32> MainWindowAll::LCS(QStringList strList1, QStringList strList2)
{
    QMap<QString,qint32> mp;
    mp.clear();
    QList<qint32> ret,sign;
    qint32 arr[MAXLENGTH]={0};
    qint32 ans[MAXLENGTH]={0};

    qDebug()<<strList1;
    qDebug()<<strList2;

    qint32 num = 0;

    for(int i=0;i<strList1.count();i++)
        mp[strList1.at(i)]=i;
    for(int i=0;i<strList2.count();i++)
        if(mp.contains(strList2.at(i)))
            arr[num++]=mp[strList2.at(i)];
    if(!num)
    {
        for(int i=0;i<strList1.count();i++)
            ret.append(i);
        return ret;
    }
    int len=0;
    ans[++len]=arr[0];
   // qDebug()<<"is Mid";
    for (int i = 1; i < num; i++)
    {
        if (arr[i] > ans[len])
        {
            ans[++len] = arr[i];
        }
        else
        {
            int l = 0, r = len;
            while (l <= r)
            {
                int mid = (l + r) >> 1;
                if (ans[mid] > arr[i])
                    r = mid-1;
                else
                    l = mid + 1;
            }
            ans[l] = qMin(ans[l], arr[i]);
        }
    }
   qDebug()<<"len is"<<len;
    for(int i=1;i<=len;i++)
        sign.append(ans[i]);
    qDebug()<<sign;
    for(int i=0;i<strList1.count();i++)
    {
        if(!sign.isEmpty()&&sign.front()==i)
            sign.pop_front();
        else
            ret.append(i);
    }
    qDebug()<<"is Retrun"<<ret;
    return ret;
}

void MainWindowAll::getCurrList(MainWindow *currWindow, QStringList& currList)
{
    ui->actAll_new->setEnabled(false);
    ui->actAll_open->setEnabled(false);
    QTextDocument* doc = currWindow->getPlainText();
    currWindow->setDiffText(QList<qint32>());
    for(int index=0;index<doc->blockCount();index++)
    {
        QString str = doc->findBlockByNumber(index).text();
        if(!str.isEmpty())
             currList<<str;
    }

}



void MainWindowAll::reduction()
{
    ui->actAll_new->setEnabled(true);
    ui->actAll_open->setEnabled(true);
    ui->actAll_line->setEnabled(true);
    ui->actAll_compare->setEnabled(true);
    if(ui->mdiArea->subWindowList().count()>0)
    {
        foreach(QMdiSubWindow* win,ui->mdiArea->subWindowList())
        {

            MainWindow *form = qobject_cast<MainWindow*>(win->widget());
            form->setStackPage(0);
        }
    }
}
