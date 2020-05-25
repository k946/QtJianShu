#include "widget_books.h"
#include "widget_entry.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include "widget_entrybooks.h"
#include "global.h"

Widget_Books::Widget_Books(QWidget *parent) : QWidget(parent)
{
    initWindow();
    initVariable();
    initConnect();

    worker.work(page);
}

void Widget_Books::initConnect()
{
    connect(&worker, WorkerOfBooks::sigSendResult, this, sltGetResult );
    connect(pushButton_nextpage, QPushButton::clicked, this, sltNextpage);
    connect(pushButton_prevpage, QPushButton::clicked, this, sltPrevpage);
}

void Widget_Books::initVariable()
{
    worker.moveToThread( GLOBAL::thread );
}

void Widget_Books::initWindow()
{

    setStyleSheet("QWidget{ background:rgb(237,245,253) }");

    //滑动窗口
    scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(" border:0px; ");

    QWidget* widget = new QWidget(this);


    //主窗口布局
    QVBoxLayout* mainVLayout = new QVBoxLayout(this);
    mainVLayout->setMargin(0);
    mainVLayout->setSpacing(0);
    mainVLayout->addWidget(scrollArea);

    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setFixedHeight(50);
    buttonContainer->setStyleSheet("background:rgb(219,230,244);");
    QHBoxLayout* buttonHLayout = new QHBoxLayout(buttonContainer);
    buttonHLayout->setMargin(0);

    //上一页按钮
    pushButton_prevpage = new QPushButton(this);
    pushButton_prevpage->setCursor(Qt::PointingHandCursor);
    GLOBAL::setIcon(QChar(0xf0a8), pushButton_prevpage, "", 15, Qt::AlignLeft, 20);
    pushButton_prevpage->setStyleSheet("background:transparent;");
    buttonHLayout->addWidget(pushButton_prevpage);

    //下一页按钮
    pushButton_nextpage = new QPushButton(this);
    pushButton_nextpage->setCursor(Qt::PointingHandCursor);
    GLOBAL::setIcon(QChar(0xf0a9), pushButton_nextpage, "", 15, Qt::AlignRight, -20);
    pushButton_nextpage->setStyleSheet("background:transparent;");
    buttonHLayout->addWidget(pushButton_nextpage);

    mainVLayout->addWidget(buttonContainer);

    //滑动窗口
    QGridLayout* gScrollLayout =  new QGridLayout(widget);
    gScrollLayout->setHorizontalSpacing(40);

    for(int i = 0; i  < 3; ++i){
        for(int j = 0; j < 4; ++j){
            entries[i*4 + j] = new Widget_EntryBooks(this);
            gScrollLayout->addWidget(entries[i*4 + j], i, j);
        }
    }

    scrollArea->setWidget(widget);

}

void Widget_Books::sltGetResult(std::vector<std::map<QString, QString> > result)
{
    int size = result.size();

    for(int i = 0;i < size && i < 12 ; ++i){
        entries[i]->update(result[i]);
    }
}

void Widget_Books::sltNextpage()
{
    page++;
    worker.work(page);
}

void Widget_Books::sltPrevpage()
{
    page--;
    worker.work(page);
}
