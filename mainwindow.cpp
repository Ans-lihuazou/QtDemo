#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // setWindowIcon(QIcon(":/icons/icon.svg"));
    setWindowTitle("无标题");
    setCentralWidget(ui->stackedWidget);
    ui->stackedWidget->setCurrentIndex(0);
    strLastList.push_back("");
    timer = new QTimer(this);
    timer->setInterval(3000);//三秒
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timer_timeout()));
    timer->start();
    font = new QFont();
    ui->actLine->setChecked(true);
    ui->actState->setChecked(true);
    *font = ui->plainTextEdit->font();
    //StateBar
//    hLayout = new QHBoxLayout();
    labelTimer = new QTimer(this);
    labelTimer->setInterval(60000);
    labelTimer->start();
    connect(labelTimer,SIGNAL(timeout()),this,SLOT(on_labelTimer_timeout()));
    labelCountFir = new QLabel("字数:");
    labelCountSec = new QLabel("");
    labelDateFir = new QLabel("时间:");
    labelDateSec = new QLabel(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
    labelCountFir->setMinimumWidth(30);
    labelCountSec->setMinimumWidth(30);
    labelDateFir->setMinimumWidth(30);
    labelDateSec->setMinimumWidth(30);
    labelCountFir->setStyleSheet(QString(" margin-left:10px;"));
    labelDateFir->setStyleSheet(QString(" margin-left:20px;"));
    ui->statusbar->addWidget(labelCountFir);
    ui->statusbar->addWidget(labelCountSec);
    ui->statusbar->addWidget(labelDateFir);
    ui->statusbar->addWidget(labelDateSec);
    ui->statusbar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    listModel = new QStringListModel(this);
    itemModel = new QStandardItemModel(this);
    ui->listView->setModel(itemModel);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);//编辑listView

//    ui->statusbar->setLayout(hLayout);

    //查找
    findDialog = new FindDialog(this);
    connect(findDialog,SIGNAL(findToNext(bool)),this,SLOT(on_findNext_set(bool)));
    connect(findDialog,SIGNAL(findLoop(bool)),this,SLOT(on_findLoop_set(bool)));
    connect(findDialog,SIGNAL(findBigSmall(bool)),this,SLOT(on_findBm_set(bool)));
    connect(findDialog,SIGNAL(findString(QString)),this,SLOT(on_findStr_set(QString)));
    connect(findDialog,SIGNAL(starFind()),this,SLOT(on_startFind()));
    ui->actFindLast->setEnabled(false);
    ui->actFindNext->setEnabled(false);
    //替换
    replaceDialog = new ReplaceDialog(this);
    connect(replaceDialog,SIGNAL(findLoop(bool)),this,SLOT(on_replaceLoop_set(bool)));
    connect(replaceDialog,SIGNAL(findBigSmall(bool)),this,SLOT(on_replaceBm_set(bool)));
    connect(replaceDialog,SIGNAL(findString(QString)),this,SLOT(on_repfindStr_set(QString)));
    connect(replaceDialog,SIGNAL(starFind()),this,SLOT(on_replaceFind()));
    connect(replaceDialog,SIGNAL(replaceString(QString)),this,SLOT(on_starReplace(QString)));
    connect(replaceDialog,SIGNAL(replaceAll(QString)),this,SLOT(on_starReplaceAll(QString)));
}

MainWindow::~MainWindow()
{
    delete listModel;
    delete timer;
    delete font;
    delete findDialog;
    delete replaceDialog;
    delete ui;
}


void MainWindow::on_actNewFile_triggered()//新建newFile
{
    if(ui->plainTextEdit->toPlainText()==initString)
    {
        initString = "";
        nowFileName = "";
        strLastList.clear();
        strLastList.push_back("");
        ui->plainTextEdit->clear();
        timer->start();
    }
    else
    {
        TipDialog *tip = new TipDialog(this);
        tip->setTip(windowTitle());
        int ret = tip->exec();
        delete tip;
        if(ret == QDialog::Accepted)
        {
           on_actSave_triggered();//保存文件
        }
        if(ret == QDialog::Rejected||ret== QDialog::Accepted )
        {
            setWindowTitle("无标题");
            nowFileName="";
            ui->plainTextEdit->clear();
            initString = "";
            strLastList.clear();
            strLastList.push_back("");
            timer->start();
        }
    }
}

void MainWindow::on_actOpenFile_triggered()//打开openFile
{
   // ui->plainTextEdit->currentCharFormat();
    //是否提示
    if(ui->plainTextEdit->toPlainText()==initString)
    {
        initString = "";
        ui->plainTextEdit->clear();
    }else
    {
        TipDialog *tip = new TipDialog(this);
        tip->setTip(windowTitle());
        int ret = tip->exec();
        delete tip;
        if(ret == QDialog::Accepted)
        {
           on_actSave_triggered();
        }else if(ret != QDialog::Rejected)
        {
            return ;
        }
    }
    strLastList.clear();
    strLastList.push_back("");
    timer->start();
    //打开文件
    initString = "";
    ui->plainTextEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this,"openFile","","text(*.txt *.TXT);;all(*.*)");
    nowFileName = fileName;
    QString name = nowFileName.right(fileName.length()-fileName.lastIndexOf('/')-1);
    setWindowTitle(name);
    if(!fileName.isEmpty())
    {
        QFile *file = new QFile(fileName);
        if(file->open(QFile::ReadOnly|QFile::Text))
        {
            QTextStream readStream(file);
            ui->plainTextEdit->setPlainText(readStream.readAll());
            initString = ui->plainTextEdit->toPlainText();
            isReaded = true;
        }
        file->close();
        delete file;
    }
}

void MainWindow::on_actSave_triggered()
{
    QString fileName = (nowFileName.isEmpty())?QFileDialog::getSaveFileName(this,"save","",""):nowFileName;
    if(fileName!=nowFileName)
        nowFileName = fileName;
    QString name = nowFileName.right(fileName.length()-fileName.lastIndexOf('/')-1);
    setWindowTitle(name);
    if(!fileName.isEmpty())
    {
        QFile *file = new  QFile(fileName);
        if(file->open(QFile::WriteOnly))
        {
            QTextStream outStream(file);
            outStream<<ui->plainTextEdit->toPlainText();
        }
        initString = ui->plainTextEdit->toPlainText();
        file->close();
        delete file;
    }
}

void MainWindow::on_actOtherSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,"save","","");
    if(fileName!=nowFileName)
        nowFileName = fileName;
    QString name = nowFileName.right(fileName.length()-fileName.lastIndexOf('/')-1);
    setWindowTitle(name);
    if(!fileName.isEmpty())
    {
        QFile *file = new  QFile(fileName);
        if(file->open(QFile::WriteOnly))
        {
            QTextStream outStream(file);
            outStream<<ui->plainTextEdit->toPlainText();
        }
        initString = ui->plainTextEdit->toPlainText();
        file->close();
        delete file;
    }
}


void MainWindow::on_plainTextEdit_copyAvailable(bool b)//b:是否选中
{
    ui->actCopy->setEnabled(b);
    ui->actCut->setEnabled(b);
    ui->actDelete->setEnabled(b);
    ui->actPaste->setEnabled(ui->plainTextEdit->canPaste());
}


void MainWindow::on_actDelete_triggered()
{
    ui->plainTextEdit->textCursor().removeSelectedText();
}

void MainWindow::on_timer_timeout()
{
//    qDebug()<<"test";
    if(strLastList.empty())
        strLastList.push_back(ui->plainTextEdit->toPlainText());
    else if(strLastList.back()!=ui->plainTextEdit->toPlainText())
        strLastList.push_back(ui->plainTextEdit->toPlainText());
    if(strLastList.count()>100)
        strLastList.pop_front();//做多回收100次
//     qDebug()<<strLastList;
}



void MainWindow::on_actLast_triggered()
{
//    qDebug()<<strLastList;
    ui->plainTextEdit->setPlainText(strLastList.back());
    if(strLastList.count()>1) strLastList.pop_back();
}

void MainWindow::on_actLine_triggered(bool checked)
{
    if(checked)
        ui->plainTextEdit->setWordWrapMode(QTextOption::WrapAnywhere);
    else
        ui->plainTextEdit->setWordWrapMode(QTextOption::NoWrap);
}

void MainWindow::on_actState_triggered(bool checked)
{
    ui->statusbar->setVisible(checked);
}

void MainWindow::on_actFont_triggered()
{
    bool ok = false;
    QFont newFont = QFontDialog::getFont(&ok,this);
    if(!ok)
        return;
    *font = newFont;
    newFont.setPointSizeF(newFont.pointSizeF()*up);
    ui->plainTextEdit->setFont(newFont);
}

void MainWindow::on_actBig_triggered()
{
    up = up*1.3;
    QFont newFont = *font;
    newFont.setPointSizeF(newFont.pointSizeF()*up);
    ui->plainTextEdit->setFont(newFont);
}

void MainWindow::on_actSmall_triggered()
{
    up = up*0.77;
    QFont newFont = *font;
    newFont.setPointSizeF(newFont.pointSizeF()*up);
    ui->plainTextEdit->setFont(newFont);
}

void MainWindow::on_actInit_triggered()
{
    up = 1.0;
    QFont newFont = *font;
    newFont.setPointSizeF(newFont.pointSizeF()*up);
    ui->plainTextEdit->setFont(newFont);
}

void MainWindow::on_actClose_triggered()
{
    if(ui->plainTextEdit->toPlainText()==initString)
    {
        close();
    }
    else
    {
        TipDialog *tip = new TipDialog(this);
        tip->setTip(windowTitle());
        int ret = tip->exec();
        delete tip;
        if(ret == QDialog::Accepted)
        {
           on_actSave_triggered();//保存文件
        }
        if(ret == QDialog::Rejected||ret== QDialog::Accepted )
        {
           close();
        }
    }
}

void MainWindow::on_actSelectAll_triggered()
{
    ui->plainTextEdit->selectAll();
}

void MainWindow::on_actDate_triggered()
{
    ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
}

void MainWindow::on_actFind_triggered()
{
    findDialog->setFindStr(findStr);
    findDialog->show();
}

void MainWindow::on_findNext_set(bool flag)
{
    findNext = flag;
}

void MainWindow::on_findLoop_set(bool flag)
{
    findLoop = flag;
}

void MainWindow::on_findBm_set(bool flag)
{
    findBm = flag;
}

void MainWindow::on_findStr_set(QString str)
{
    findStr = str;
    ui->actFindLast->setEnabled(!findStr.isEmpty());
    ui->actFindNext->setEnabled(!findStr.isEmpty());
}

void MainWindow::on_startFind()
{
    bool flag = false;
    if(findNext)
    {
        if(findBm)//向后区分大小写
        {
           flag = ui->plainTextEdit->find(findStr,QTextDocument::FindCaseSensitively);
           if(findLoop&&!flag)
               while(ui->plainTextEdit->find(findStr,QTextDocument::FindBackward|QTextDocument::FindCaseSensitively))
                   flag = true;
        }
        else
        {
            flag = ui->plainTextEdit->find(findStr);
            if(findLoop&&!flag)
                while(ui->plainTextEdit->find(findStr,QTextDocument::FindBackward))
                    flag = true;
        }
    }
    else
    {
        if(findBm)
        {
           flag = ui->plainTextEdit->find(findStr,QTextDocument::FindCaseSensitively|QTextDocument::FindBackward);
           if(findLoop&&!flag)
               while(ui->plainTextEdit->find(findStr,QTextDocument::FindCaseSensitively))
                   flag = true;
        }
        else
        {
            flag = ui->plainTextEdit->find(findStr,QTextDocument::FindBackward);
            if(findLoop&&!flag)
                while(ui->plainTextEdit->find(findStr))
                    flag = true;
        }
    }
    if(!flag)
    {
       if(!remAll)
       {
           if(ui->plainTextEdit->textCursor().selectedText()!=findStr)
                QMessageBox::warning(this, "Warning",QString("找不到%1").arg(findStr));
       }
       else remAll = false;
    }

}

void MainWindow::on_actFindLast_triggered()
{
    bool flagNext = findNext;
    bool flagLoop = findLoop;
    findNext = false;
    findLoop = true;
    on_startFind();
    findNext = flagNext;
    findLoop = flagLoop;
}

void MainWindow::on_actFindNext_triggered()
{
    bool flagNext = findNext;
    bool flagLoop = findLoop;
    findNext = true;
    findLoop = true;
    on_startFind();
    findNext = flagNext;
    findLoop = flagLoop;
}

void MainWindow::on_replaceLoop_set(bool flag)
{
    replaceLoop = flag;
}

void MainWindow::on_replaceBm_set(bool flag)
{
    replaceBm = flag;
}

void MainWindow::on_repfindStr_set(QString str)
{
    repFindStr = str;
}

void MainWindow::on_replaceFind()
{
    QString str = findStr;

    findStr = repFindStr;

    bool flagNext = findNext;
    bool flagLoop = findLoop;
    bool flagBm = findBm;

    findNext = true;
    findLoop = replaceLoop;
    findBm = replaceBm;

    on_startFind();

    findNext = flagNext;
    findLoop = flagLoop;
    findBm = flagBm;

    findStr = str;

}

void MainWindow::on_starReplace(QString replaceStr)
{
    if(ui->plainTextEdit->textCursor().selectedText()!=repFindStr)
        on_replaceFind();
    else
    {
        ui->plainTextEdit->textCursor().removeSelectedText();
        ui->plainTextEdit->textCursor().insertText(replaceStr);
    }

}

void MainWindow::on_starReplaceAll(QString replaceStr)
{
    bool flag = replaceLoop;
    replaceLoop = true;
    remAll = true;
    while(remAll)
    {
        if(ui->plainTextEdit->textCursor().selectedText()!=repFindStr)
            on_replaceFind();
        else
        {
            ui->plainTextEdit->textCursor().removeSelectedText();
            ui->plainTextEdit->textCursor().insertText(replaceStr);
        }
    }
    replaceLoop = flag;
}

void MainWindow::on_labelTimer_timeout()
{
    labelDateSec->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
}

void MainWindow::on_actReplace_triggered()
{
    replaceDialog->show();
}

void MainWindow::on_plainTextEdit_textChanged()
{
    labelCountSec->setText(QString::number(ui->plainTextEdit->toPlainText().count()));
}

void MainWindow::readFile(QString fileName)
{
    QFile *file = new QFile(fileName);
    if(file->open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream stream(file);
        ui->plainTextEdit->setPlainText(stream.readAll());
        QFileInfo fileInfo(*file);
        setWindowTitle(fileInfo.fileName());
        isReaded = true;
    }
    file->close();
    delete file;
}

QTextDocument* MainWindow::getPlainText()
{
    return ui->plainTextEdit->document();
}

void MainWindow::setStackPage(int id)
{
    ui->stackedWidget->setCurrentIndex(id);
}

void MainWindow::setDiffText(QList<qint32> diffList)
{
      QTextDocument *doc = ui->plainTextEdit->document();
      qDebug()<<"diffList count = "<<diffList;
      QBrush brush;
      brush.setColor(Qt::red);
      brush.setStyle(Qt::SolidPattern);
      setStackPage(1);
      itemModel->removeRows(0,itemModel->rowCount());
      for(qint32 i=0;i<=doc->blockCount();i++)
      {
          QStandardItem *item =new QStandardItem;
          if(doc->findBlockByNumber(i).text().isEmpty())
              continue;
          item->setData(doc->findBlockByNumber(i).text(),0);
          if(diffList.count()&&itemModel->rowCount()==diffList.front())
          {
              diffList.pop_front();
              item->setBackground(brush);
          }
          itemModel->appendRow(item);
      }
//      while(itemModel->rowCount()>0)
//      {
//         QStandardItem *item = itemModel->item(itemModel->rowCount()-1,0);
//         if( item->text().isEmpty())
//             itemModel->removeRow(itemModel->rowCount()-1);
//         else break;
//      }
      for(int i=0;i<itemModel->rowCount();i++)
          itemModel->item(i)->setText
                  (QString::asprintf("#%d: ",i+1)+itemModel->item(i)->text());
}

bool MainWindow::getisReaded()
{
    return isReaded;
}
