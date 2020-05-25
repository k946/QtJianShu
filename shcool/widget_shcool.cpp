#include "widget_shcool.h"
#include "global.h"

Widget_Shcool::Widget_Shcool(QWidget *parent) : QWidget(parent)
{
    initWindow();
    initVariable();
    initConnect();
    worker.work();
}

void Widget_Shcool::initWindow()
{
    setStyleSheet("background:rgb(237,245,253);");

    /*进入简书学堂后显示的第一个窗口*/
    firstWidget = new QWidget(this);
    QVBoxLayout* firstWidgetVLayout = new QVBoxLayout(firstWidget);
    firstWidgetVLayout->setMargin(0);
    firstWidgetVLayout->setSpacing(0);

    //专题信息块
    collectionItem = new CollectionItem(this);
    firstWidgetVLayout->addSpacing(10);
    firstWidgetVLayout->addWidget(collectionItem);


    /*文章、动态、最新评论、热门*/
    QHBoxLayout* triggerMenuHLayout = new QHBoxLayout();
    triggerMenuHLayout->setMargin(0);
    triggerMenuHLayout->setSpacing(0);
    triggerMenuHLayout->addSpacing(10);

    //最新评论
    menuComments = new TriggerMenuItem("最新评论", this);
    GLOBAL::setIcon( QChar(0xf52d), menuComments, "#969696", 13, Qt::AlignVCenter, 40);
    GLOBAL::hoverIconColor(menuComments, "#646464");
    triggerMenuHLayout->addWidget(menuComments);


    //最新收录
    menuAdded = new TriggerMenuItem("最新收录", this);
    GLOBAL::setIcon( QChar(0xf0f3), menuAdded, "#969696", 13, Qt::AlignVCenter, 40);
    GLOBAL::hoverIconColor(menuAdded, "#646464");
    triggerMenuHLayout->addWidget(menuAdded);


    //评论
    menuHot = new TriggerMenuItem("评论", this);
    GLOBAL::setIcon( QChar(0xf086), menuHot, "#969696", 13, Qt::AlignVCenter, 40);
    GLOBAL::hoverIconColor(menuHot, "#646464");
    triggerMenuHLayout->addWidget(menuHot);


    /*用于填充，以设置背景*/
    triggerMenuHLayout->addWidget(new QWidget(this));

    /*将菜单加入顶层布局*/
    firstWidgetVLayout->addLayout(triggerMenuHLayout);


    /*可滑动窗口，用于显示文章列表*/
    articleListScroll = new QScrollArea(this);
    articleListScroll->setStyleSheet("border:0px;");
    /*滑动窗口内的控件可缩放大小*/
    articleListScroll->setWidgetResizable(true);

    //scrollArea内的widget
    QWidget* articleFram = new QWidget(articleListScroll);
    //scrollArea内的Widget的布局
    articleFramVLayout = new QVBoxLayout(articleFram);
    articleFramVLayout->setMargin(0);
    articleFramVLayout->setSpacing(0);

    /*设置滑动窗口内的widget*/
    articleListScroll->setWidget(articleFram);

    /*作者信息窗口添加文章列表*/
    firstWidgetVLayout->addWidget(articleListScroll);

    /*将作者信息添加进界面容器*/
    stackWidget = new QStackedWidget(this);
    stackWidget->addWidget(firstWidget);
    stackWidget->setCurrentWidget(firstWidget);

    /*安装过滤器，管理返回界面*/
    stackWidget->setObjectName("stackedWidget");
    stackWidget->installEventFilter(GLOBAL::titleBar);

    /*顶层界面显示stackWidget*/
    QVBoxLayout* topVLayout = new QVBoxLayout(this);
    topVLayout->setMargin(0);
    topVLayout->setSpacing(0);
    topVLayout->addWidget(stackWidget);
}

void Widget_Shcool::initVariable()
{
    worker.moveToThread(GLOBAL::thread);
}

void Widget_Shcool::initConnect()
{
    connect(&worker, SIGNAL(sigSendResult(std::vector<std::map<QString,QString> >)),
                            this, SLOT(sltGetResult(std::vector<std::map<QString,QString> >)));
}

void Widget_Shcool::sltGetResult(std::vector<std::map<QString, QString> > result)
{
    /*更新专题信息块*/
    collectionItem->update(result[0]);

    int articleNumber = result.size() - 1;

    for(int i = 0; i < articleNumber; ++i){
        /*当文章数量过多，列表无法显示时，创建新的widget_enrtyArticle显示*/
        if(articleList.size() < articleNumber){
            articleList.push_back( new ArticleItem(this) );
            articleFramVLayout->addWidget( articleList[i] );

            /*点击文章标题，触发sltLoadArticLe槽函数，加载文章*/
            connect(articleList[i], SIGNAL(sigLoadArticle(QString)),this, SLOT(sltLoadArticle(QString)));
            /*点击作者名称，加载作者详情*/
            connect(articleList[i], SIGNAL(sigLoadAuthor(QString)), this, SLOT(sltLoadAuhtor(QString)));
        }
        /*更新显示的文章信息*/
        articleList[i]->update(result[i+1]);
    }
}

void Widget_Shcool::sltLoadArticle(QString url)
{
    if(0 == article){
        article = new Widget_Article(this);
        stackWidget->addWidget(article);
    }
    article->loadArticle(url);

    /*将当前界面添加到标题栏返回按钮的页面堆中同时设置返回按钮为可见，进而在单击标题栏的返回按钮时能返回到当前界面*/
    GLOBAL::titleBar->addStackedWidget(stackWidget, stackWidget->currentWidget());

    stackWidget->setCurrentWidget(article);
}

void Widget_Shcool::sltLoadAuhtor(QString url)
{
    if(0 == author){
        author = new Widget_AuthorInfo(this);
        stackWidget->addWidget(author);
    }
    author->loadAuthorInfo( url );

    /*将当前界面添加到标题栏返回按钮的页面堆中同时设置返回按钮为可见，进而在单击标题栏的返回按钮时能返回到当前界面*/
    GLOBAL::titleBar->addStackedWidget(stackWidget, stackWidget->currentWidget());

    stackWidget->setCurrentWidget(author);
}
