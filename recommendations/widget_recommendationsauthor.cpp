#include "widget_recommendationsauthor.h"
#include "global.h"
#include <QHBoxLayout>

Widget_RecommendationsAuthor::Widget_RecommendationsAuthor(QWidget *parent) : QWidget(parent)
{
    initVariable();
    initWindow();
    initConnect();

    worker.work( page );
}

void Widget_RecommendationsAuthor::initConnect()
{
    connect(&worker, WorkerOfRecommendationsAuthor::sigSendResult, this, sltGetResult );
    connect(pushButton_nextpage, &QPushButton::clicked, this, &sltNextpage);
    connect(pushButton_prevpage, &QPushButton::clicked, this, &sltPrevpage);
}

void Widget_RecommendationsAuthor::initVariable()
{
    worker.moveToThread(GLOBAL::thread);
}

void Widget_RecommendationsAuthor::initWindow()
{
    setStyleSheet("background:rgb(237,245,253);"
                  "border:0px;");

    firstWidget = new QWidget(this);
    QVBoxLayout* firstWidgetVLayout = new QVBoxLayout(firstWidget);
    firstWidgetVLayout->setMargin(0);
    firstWidgetVLayout->setSpacing(0);

    authorListArea = new QScrollArea(this);
    authorListArea->setWidgetResizable(true);
    firstWidgetVLayout->addWidget(authorListArea);

    QWidget* authorListWidget = new QWidget(authorListArea);
    QGridLayout* authorListGLayout = new QGridLayout(authorListWidget);

    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 3; ++j){
            authorItems[i*3 + j] = new RecommendationsAuthorItem(this);
            authorListGLayout->addWidget( authorItems[i*3 + j], i, j);

            connect(authorItems[i*3 + j], SIGNAL(sigLoadArticle(QString)),
                    this, SLOT(sltLoadArticle(QString)));
            connect(authorItems[i*3 + j], SIGNAL(sigLoadAuthor(QString)), this, SLOT(sltLoadAuhtor(QString)));
        }
    }

    authorListArea->setWidget(authorListWidget);

    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setFixedHeight(50);
    buttonContainer->setStyleSheet("background:rgb(219,230,244);");
    QHBoxLayout* buttonHLayout = new QHBoxLayout(buttonContainer);
    buttonHLayout->setMargin(0);

    //上一页按钮
    pushButton_prevpage = new QPushButton(this);
    pushButton_prevpage->setFixedHeight(40);
    pushButton_prevpage->setCursor(Qt::PointingHandCursor);
    GLOBAL::setIcon(QChar(0xf0a8), pushButton_prevpage, "", 15, Qt::AlignLeft, 20);
    pushButton_prevpage->setStyleSheet("background:transparent;");
    buttonHLayout->addWidget(pushButton_prevpage);

    //下一页按钮
    pushButton_nextpage = new QPushButton(this);
    pushButton_nextpage->setFixedHeight(40);
    pushButton_nextpage->setCursor(Qt::PointingHandCursor);
    GLOBAL::setIcon(QChar(0xf0a9), pushButton_nextpage, "", 15, Qt::AlignRight, -20);
    pushButton_nextpage->setStyleSheet("background:transparent;");
    buttonHLayout->addWidget(pushButton_nextpage);

    firstWidgetVLayout->addWidget(buttonContainer);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(firstWidget);
    stackedWidget->setCurrentWidget(firstWidget);

    /*返回上一界面管理*/
    stackedWidget->setObjectName("stackedWidget");
    stackedWidget->installEventFilter(GLOBAL::titleBar);

    QVBoxLayout* mainVLayout = new QVBoxLayout(this);
    mainVLayout->setMargin(0);
    mainVLayout->setSpacing(0);
    mainVLayout->addWidget(stackedWidget);


}

void Widget_RecommendationsAuthor::search()
{
    worker.work( page );
}


void Widget_RecommendationsAuthor::sltLoadArticle(QString url)
{
    if(0 == article){
        article = new Widget_Article(this);
        stackedWidget->addWidget(article);
    }
    article->loadArticle(url);

    /*将当前界面添加到标题栏返回按钮的页面堆中同时设置返回按钮为可见，进而在单击标题栏的返回按钮时能返回到当前界面*/
    GLOBAL::titleBar->addStackedWidget(stackedWidget, stackedWidget->currentWidget());

    stackedWidget->setCurrentWidget(article);
}

void Widget_RecommendationsAuthor::sltLoadAuhtor(QString url)
{

    if(0 == author){
        author = new Widget_AuthorInfo(this);
        stackedWidget->addWidget(author);
    }
    author->loadAuthorInfo( url );

    /*将当前界面添加到标题栏返回按钮的页面堆中同时设置返回按钮为可见，进而在单击标题栏的返回按钮时能返回到当前界面*/
    GLOBAL::titleBar->addStackedWidget(stackedWidget, stackedWidget->currentWidget());

    stackedWidget->setCurrentWidget(author);
}

void Widget_RecommendationsAuthor::sltGetResult(std::vector<std::map<QString, QString> > result)
{
    int resultSize = result.size();
    for(int i = 0; i < 24 && i < resultSize; ++i){
        authorItems[i]->update(result[i]);
    }
}

void Widget_RecommendationsAuthor::sltNextpage()
{
    ++page;
    search();
}

void Widget_RecommendationsAuthor::sltPrevpage()
{
    --page;
    search();

}

