#include "widget_homepage.h"
#include "ui_widget_homepage.h"
#include "global.h"

Widget_HomePage::Widget_HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget_HomePage)
{
    ui->setupUi(this);

    initVariable();
    initWindow(); 
    initConnect();

    worker.search(QString::number(page));
}

void Widget_HomePage::sltGetResult(std::vector<std::map<QString, QString> > result)
{
    int size = result.size();
    for(int i = 0; i < size && i < 8; ++i){
        entries[i]->update(result[i]);
        entries[i]->setVisible(true);
    }
}

Widget_HomePage::~Widget_HomePage()
{
    delete ui;
}

void Widget_HomePage::search()
{
    worker.search(QString::number(page));

    for(int i = 0; i < 8; ++i){
        entries[i]->hide();
    }
}

void Widget_HomePage::initConnect()
{
    connect(&worker, WorkerOfHomePage::sigSendResult, this, sltGetResult );
}

void Widget_HomePage::initVariable()
{
    worker.moveToThread( GLOBAL::thread );

    /*设置返回界面管理*/
    ui->stackedWidget->setObjectName("stackedWidget");
    ui->stackedWidget->installEventFilter(GLOBAL::titleBar);
}

void Widget_HomePage::initWindow()
{

    GLOBAL::setIcon(QChar(0xf0a8), ui->pushButton_prevPage, "", 15, Qt::AlignLeft, 20);
    ui->pushButton_prevPage->setStyleSheet("background:transparent;");
    ui->pushButton_prevPage->setCursor(Qt::PointingHandCursor);

    GLOBAL::setIcon(QChar(0xf0a9), ui->pushButton_nextPage, "", 15, Qt::AlignRight, -20);
    ui->pushButton_nextPage->setStyleSheet("background:transparent;");
    ui->pushButton_nextPage->setCursor(Qt::PointingHandCursor);

    layout = new QVBoxLayout( ui->scrollArea->widget() );     //布局
    for(int i = 0; i < 8; ++i){
        entries[i] = new ArticleItem(this);
        entries[i]->hide();
        layout->addWidget(entries[i]);
        layout->setMargin(0);

        connect(entries[i], SIGNAL(sigLoadArticle(QString)), this, SLOT(sltLoadArticle(QString)));
        connect(entries[i], SIGNAL(sigLoadAuthor(QString)), this, SLOT(sltLoadAuhtor(QString)));
    }

}

void Widget_HomePage::sltLoadArticle(QString url)
{
    if(0 == article){
        article = new Widget_Article(this);
        ui->stackedWidget->addWidget(article);
    }
    article->loadArticle(url);

    /*将当前界面添加到标题栏返回按钮的页面堆中同时设置返回按钮为可见，进而在单击标题栏的返回按钮时能返回到当前界面*/
    GLOBAL::titleBar->addStackedWidget(ui->stackedWidget, ui->stackedWidget->currentWidget());

    ui->stackedWidget->setCurrentWidget(article);
}

void Widget_HomePage::sltLoadAuhtor(QString url)
{

    if(0 == author){
        author = new Widget_AuthorInfo(this);
        ui->stackedWidget->addWidget(author);
    }
    author->loadAuthorInfo( url );

    /*将当前界面添加到标题栏返回按钮的页面堆中同时设置返回按钮为可见，进而在单击标题栏的返回按钮时能返回到当前界面*/
    GLOBAL::titleBar->addStackedWidget(ui->stackedWidget, ui->stackedWidget->currentWidget());

    ui->stackedWidget->setCurrentWidget(author);
}



void Widget_HomePage::on_pushButton_prevPage_clicked()
{
    page = (page - 1 + 16) % 16;
    search();

}

void Widget_HomePage::on_pushButton_nextPage_clicked()
{
    page = (page + 1) % 16;
    search();
}
