#include "widget_publications.h"
#include <QGridLayout>
#include "global.h"
#include <QEvent>

Widget_Publications::Widget_Publications(QWidget *parent) : QWidget(parent)
{
    initVariable();
    initWindow();
    initConnect();

    bookWorker.work();
    authorWorker.work(authorsPage);
}

void Widget_Publications::initConnect()
{
    //获取推荐作者请求结果
    connect(&authorWorker, WorkerOfPublicationsAuthor::sigSendResult, this, sltGetResultAuthor);

    //获取书籍请求结果
    connect(&bookWorker, WorkerOfPublicationsBook::sigSendResult, this, sltGetResultBook);
}

void Widget_Publications::initVariable()
{
    authorWorker.moveToThread(GLOBAL::thread);
    bookWorker.moveToThread(GLOBAL::thread);
}

void Widget_Publications::initWindow()
{
    //若干位推荐作者加入到authorsBlockWidget中利于隐藏、显示，管理
    authorsBlockWidget = new QWidget(this);
    QGridLayout* subGLayout = new QGridLayout(authorsBlockWidget);
    subGLayout->setContentsMargins (10, 0, 0, 0);
    subGLayout->setSizeConstraint (QLayout::SetFixedSize);
    for(int i = 0; i < 2; ++i){
        for(int j = 0; j < 2; ++j){
            authors[i*2 + j] = new Widget_EntryPublicationsAuthor(this);
            subGLayout->addWidget( authors[i*2 + j], i, j );

            connect(authors[i*2 + j], Widget_EntryPublicationsAuthor::sigClicked, this, sltOnClickedAuthor);
        }
    }

}

void Widget_Publications::initWindow(int novel, int it, int career, int culture, int skill, int collection)
{
    //滑动窗口
    QWidget* scrollFram = new QWidget(this);

    QGridLayout* framLayout = new QGridLayout(scrollFram);
    framLayout->setMargin(0);
    framLayout->setSizeConstraint (QLayout::SetFixedSize);


    //推荐作者加入布局
    authorsBlockLabel = new LabelMouseEvent("推荐作者", this);
    authorsBlockLabel->setObjectName("authorsBlockLabel");
    authorsBlockLabel->setFixedHeight(36);
    authorsBlockLabel->setFont( QFont("黑体", 12, 87) );
    authorsBlockLabel->setStyleSheet("LabelMouseEvent#authorsBlockLabel{border-left: 8px solid #70B6AB;"
                      "margin-left:10px;"
                      "padding-left:12px;"
                      "color:#333333;"
                      "background:transparent;"
                      "}");
    framLayout->addWidget(authorsBlockLabel, 0, 0);
    connect(authorsBlockLabel, LabelMouseEvent::sigClicked, this, [=]{
        bool visible =  !authorsBlockWidget->isVisible();
        authorsBlockWidget->setVisible( visible );
    });

    //换一批
    page_change = new LabelMouseEvent("换一批", this);
    page_change->setCursor(Qt::PointingHandCursor);
    page_change->setAlignment(Qt::AlignRight);
    page_change->setObjectName("page_change");
    page_change->setFixedHeight(24);
    page_change->setStyleSheet("LabelMouseEvent#page_change{color:#41867A; font: 24px \"黑体\";padding-right:20px; }");
    GLOBAL::setIcon(QChar(0xf2f1), page_change, "#41867A", 8, Qt::AlignRight, -80);
    framLayout->addWidget(page_change, 0, 1);
    connect(page_change, LabelMouseEvent::sigClicked, this, [=]{
        authorWorker.work(++authorsPage);
    });

    framLayout->addWidget(authorsBlockWidget, 1, 0, 1, 2);


    /*
    ** 小说、故事模块
    */
    novelBlockLabel = new LabelMouseEvent("小说、故事", this);
    novelBlockLabel->setObjectName("novelBlockLabel");
    novelBlockLabel->setFixedHeight(36);
    novelBlockLabel->setFont( QFont("黑体", 12, 87) );
    novelBlockLabel->setStyleSheet("LabelMouseEvent#novelBlockLabel{"
                                   "border-left: 8px solid #70B6AB;"
                                   "margin-left:10px;"
                                   "padding-left:12px;"
                                   "color:#333333;"
                                   "background:transparent;"
                                   "}");
    framLayout->addWidget(novelBlockLabel, framLayout->rowCount() + 1, 0, 1, 2);
    //显示与隐藏小说、故事
    connect(novelBlockLabel, LabelMouseEvent::sigClicked, this, [=]{
        bool visible =  !novelBlockWidget->isVisible();
        novelBlockWidget->setVisible( visible );
    });

    //将novel本小说加入到novelBlockWidget中利于隐藏、显示，管理
    novelBlockWidget = new QWidget(this);
    QGridLayout* subGLayout = new QGridLayout(novelBlockWidget);
    subGLayout->setContentsMargins(10,0,0,0);
    subGLayout->setSizeConstraint (QLayout::SetFixedSize);
    for(int i = 0; i < novel; ++i){
        Widget_Entry* temp = new Widget_EntryPublicationsBook(this);
        books.push_back( temp );
        subGLayout->addWidget( temp, i/2, i%2 );
    }
    framLayout->addWidget(novelBlockWidget, framLayout->rowCount() + 1, 0, 1, 2);

    //互联网、科普
    itBlockLabel = new LabelMouseEvent("互联网、科普", this);
    itBlockLabel->setObjectName("novelBlockLabel");
    itBlockLabel->setFixedHeight(36);
    itBlockLabel->setFont( QFont("黑体", 12, 87) );
    itBlockLabel->setStyleSheet("LabelMouseEvent#novelBlockLabel{"
                                   "border-left: 8px solid #70B6AB;"
                                   "margin-left:10px;"
                                   "padding-left:12px;"
                                   "color:#333333;"
                                   "background:transparent;"
                                   "}");
    framLayout->addWidget(itBlockLabel, framLayout->rowCount() + 1, 0, 1, 2);
    connect(itBlockLabel, LabelMouseEvent::sigClicked, this, [=]{
        bool visible =  !itBlockWidget->isVisible();
        itBlockWidget->setVisible( visible );
    });

    itBlockWidget = new QWidget(this);
    subGLayout = new QGridLayout(itBlockWidget);
    subGLayout->setContentsMargins(10,0,0,0);
    subGLayout->setSizeConstraint (QLayout::SetFixedSize);
    for(int i = 0; i < it; ++i){
        Widget_Entry* temp = new Widget_EntryPublicationsBook(this);
        books.push_back( temp );
        subGLayout->addWidget( temp, i/2, i%2 );
    }
    framLayout->addWidget(itBlockWidget, framLayout->rowCount() + 1, 0, 1, 2);

    //职场、励志、理财
    careerBlockLabel = new LabelMouseEvent("职场、励志、理财", this);
    careerBlockLabel->setObjectName("novelBlockLabel");
    careerBlockLabel->setFixedHeight(36);
    careerBlockLabel->setFont( QFont("黑体", 12, 87) );
    careerBlockLabel->setStyleSheet("LabelMouseEvent#novelBlockLabel{"
                                   "border-left: 8px solid #70B6AB;"
                                   "margin-left:10px;"
                                   "padding-left:12px;"
                                   "color:#333333;"
                                   "background:transparent;"
                                   "}");
    framLayout->addWidget(careerBlockLabel, framLayout->rowCount() + 1, 0, 1, 2);
    connect(careerBlockLabel, LabelMouseEvent::sigClicked, this, [=]{
        bool visible =  !careerBlockWidget->isVisible();
        careerBlockWidget->setVisible( visible );
    });


    careerBlockWidget = new QWidget(this);
    subGLayout = new QGridLayout(careerBlockWidget);
    subGLayout->setContentsMargins(10,0,0,0);
    subGLayout->setSizeConstraint (QLayout::SetFixedSize);
    for(int i = 0; i < career; ++i){
        Widget_Entry* temp = new Widget_EntryPublicationsBook(this);
        books.push_back( temp );
        subGLayout->addWidget( temp, i/2, i%2 );
    }
    framLayout->addWidget(careerBlockWidget, framLayout->rowCount() + 1, 0, 1, 2);

    //文化、历史
    cultureBlockLabel = new LabelMouseEvent("文化、历史", this);
    cultureBlockLabel->setObjectName("novelBlockLabel");
    cultureBlockLabel->setFixedHeight(36);
    cultureBlockLabel->setFont( QFont("黑体", 12, 87) );
    cultureBlockLabel->setStyleSheet("LabelMouseEvent#novelBlockLabel{"
                                   "border-left: 8px solid #70B6AB;"
                                   "margin-left:10px;"
                                   "padding-left:12px;"
                                   "color:#333333;"
                                   "background:transparent;"
                                   "}");
    framLayout->addWidget(cultureBlockLabel, framLayout->rowCount() + 1, 0, 1, 2);
    connect(cultureBlockLabel, LabelMouseEvent::sigClicked, this, [=]{
        bool visible =  !cultureBlockWidget->isVisible();
        cultureBlockWidget->setVisible( visible );
    });

    cultureBlockWidget = new QWidget(this);
    subGLayout = new QGridLayout(cultureBlockWidget);
    subGLayout->setContentsMargins(10,0,0,0);
    subGLayout->setSizeConstraint (QLayout::SetFixedSize);
    for(int i = 0; i < culture; ++i){
        Widget_Entry* temp = new Widget_EntryPublicationsBook(this);
        books.push_back( temp );
        subGLayout->addWidget( temp, i/2, i%2 );
    }
    framLayout->addWidget(cultureBlockWidget, framLayout->rowCount() + 1, 0, 1, 2);


    //工具、技能
    skillBlockLabel = new LabelMouseEvent("工具、技能", this);
    skillBlockLabel->setObjectName("novelBlockLabel");
    skillBlockLabel->setFixedHeight(36);
    skillBlockLabel->setFont( QFont("黑体", 12, 87) );
    skillBlockLabel->setStyleSheet("LabelMouseEvent#novelBlockLabel{"
                                   "border-left: 8px solid #70B6AB;"
                                   "margin-left:10px;"
                                   "padding-left:12px;"
                                   "color:#333333;"
                                   "background:transparent;"
                                   "}");
    framLayout->addWidget(skillBlockLabel, framLayout->rowCount() + 1, 0, 1, 2);
    connect(skillBlockLabel, LabelMouseEvent::sigClicked, this, [=]{
        bool visible =  !skillBlockWidget->isVisible();
        skillBlockWidget->setVisible( visible );
    });

    skillBlockWidget = new QWidget(this);
    subGLayout = new QGridLayout(skillBlockWidget);
    subGLayout->setContentsMargins(10,0,0,0);
    subGLayout->setSizeConstraint (QLayout::SetFixedSize);
    for(int i = 0; i < skill; ++i){
        Widget_Entry* temp = new Widget_EntryPublicationsBook(this);
        books.push_back( temp );
        subGLayout->addWidget( temp, i/2, i%2 );
    }
    framLayout->addWidget(skillBlockWidget, framLayout->rowCount() + 1, 0, 1, 2);

/*
    //电子书
    collectionBlockLabel = new LabelMouseEvent("电子书", this);
    collectionBlockLabel->setObjectName("novelBlockLabel");
    collectionBlockLabel->setFixedHeight(36);
    collectionBlockLabel->setFont( QFont("黑体", 12, 87) );
    collectionBlockLabel->setStyleSheet("LabelMouseEvent#novelBlockLabel{"
                                   "border-left: 8px solid #70B6AB;"
                                   "margin-left:10px;"
                                   "padding-left:12px;"
                                   "color:#333333;"
                                   "background:transparent;"
                                   "}");
    framLayout->addWidget(collectionBlockLabel, framLayout->rowCount() + 1, 0, 1, 2);

    collectionBlockWidget = new QWidget(this);
    subGLayout = new QGridLayout(collectionBlockWidget);
    subGLayout->setContentsMargins(10,0,0,0);
    subGLayout->setSizeConstraint (QLayout::SetFixedSize);
    for(int i = 0; i < culture; ++i){
        Widget_Entry* temp = new Widget_EntryPublicationsBook(this);
        books.push_back( temp );
        subGLayout->addWidget( temp, i/2, i%2 );
    }
    framLayout->addWidget(collectionBlockWidget, framLayout->rowCount() + 1, 0, 1, 2);

*/
    scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet("border:0px; background:rgb(237,245,253);");
    scrollArea->setWidget(scrollFram);
    authorsBlockWidget->setVisible(false);
    novelBlockWidget->setVisible(false);
    itBlockWidget->setVisible(false);
    careerBlockWidget->setVisible(false);
    cultureBlockWidget->setVisible(false);
    skillBlockWidget->setVisible(false);

    /*界面栈, 安装事件过滤器*/
    stackWidget = new QStackedWidget(this);
    stackWidget->installEventFilter(GLOBAL::titleBar);

    QVBoxLayout* topLayout = new QVBoxLayout(this);
    topLayout->setMargin(0);
    topLayout->addWidget(stackWidget);

    stackWidget->addWidget(scrollArea);
    stackWidget->setCurrentWidget(scrollArea);
}


//推荐作者请求结果
void Widget_Publications::sltGetResultAuthor(std::vector<std::map<QString, QString> > result)
{
    //将结果分发给各个WidgetEntry显示
    for(int i = 0; i < result.size() && i < 4; ++i){
        authors[i]->update(result[i]);
    }

    //没有数据，则从头开始请求
    if(result.size() == 0){
        authorsPage = 1;
    }
}

/*书籍请求结果*/
void Widget_Publications::sltGetResultBook(std::vector<std::map<QString, QString> > result)
{
    int novel = 0;
    int it = 0;
    int career = 0;
    int culture = 0;
    int skill = 0;
    int collection = 0;

    //统计每类书籍的数量
    for(int i = 0; i < result.size(); ++i){
        std::map<QString, QString>::iterator now =  result[i].begin();
        QString key = now->first;
        QString category = key.section("_", 0, 0);

        if( category == "novel"){
            ++novel;
        }else if( category == "it"){
            ++it;
        }else if( category == "career"){
            ++career;
        }else if( category == "culture"){
            ++culture;
        }else if( category == "skill"){
            ++skill;
        }else if( category == "collection"){
            ++collection;
        }
    }


    if( !finishInit ){
        initWindow(novel, it, career, culture, skill, collection);
        finishInit = true;
    }

    //将结果分发给各个WidgetEntry显示
    for(int i = 0; i < books.size(); ++i){
        books[i]->update( result[i] );
    }
}

/*点击某个作者，进入作者的主页*/
void Widget_Publications::sltOnClickedAuthor()
{

    QString url = ((Widget_Entry*)sender())->getLink();

    if(0 == authorInfoWidget){
        authorInfoWidget = new Widget_AuthorInfo(this);
        stackWidget->addWidget(authorInfoWidget);
    }
    authorInfoWidget->loadAuthorInfo( url );

    /*将当前界面添加到标题栏返回按钮的页面堆中同时设置返回按钮为可见，进而在单击标题栏的返回按钮时能返回到当前界面*/
    GLOBAL::titleBar->addStackedWidget(stackWidget, stackWidget->currentWidget());

    stackWidget->setCurrentWidget(authorInfoWidget);
}

