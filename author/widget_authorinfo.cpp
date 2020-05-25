#include "widget_authorinfo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "global.h"

Widget_AuthorInfo::Widget_AuthorInfo(QWidget *parent) : QWidget(parent)
{
    initWindow();
    initConnect();
    worker.moveToThread(GLOBAL::thread);
}

void Widget_AuthorInfo::loadAuthorInfo(const QUrl &url)
{
    worker.work(url);
    link = url;

    menuArticle->setLink(link.toString() + "?order_by=shared_at");
    menuFeed->setLink(QString(link.toString()).replace("u","users") + "/timeline");
    menuComments->setLink(link.toString() + "?order_by=commented_at");
    menuHot->setLink(link.toString() + "?order_by=top");
}

void Widget_AuthorInfo::initWindow()
{
    setStyleSheet("background:rgb(237,245,253);");

    /*作者信息*/
    firstWidget = new QWidget(this);
    QVBoxLayout* firstWidgetVLayout = new QVBoxLayout(firstWidget);
    firstWidgetVLayout->setMargin(0);
    firstWidgetVLayout->setSpacing(0);

    //作者信息块
    author = new AuthorItem(this);
    author->setFixedHeight(200);
    firstWidgetVLayout->addWidget(author);


    /*文章、动态、最新评论、热门*/
    QHBoxLayout* triggerMenuHLayout = new QHBoxLayout();
    triggerMenuHLayout->setMargin(0);
    triggerMenuHLayout->setSpacing(0);
    triggerMenuHLayout->addSpacing(10);

    //文章
    menuArticle = new LabelMouseEvent("文章", this);
    menuArticle->setMargin(0);
    menuArticle->setObjectName("menuArticle");
    menuArticle->setStyleSheet("LabelMouseEvent#menuArticle{"
                               "color:#969696;}"
                               "QLabel:hover{"
                               "border-bottom:4px solid;}"
                               "QLabel#menuArticle:hover{"
                               "color:#646464;}"
                              );
    menuArticle->setFixedSize(200,80);
    menuArticle->setAlignment(Qt::AlignVCenter);
    menuArticle->setIndent(90);
    menuArticle->setFont( QFont("黑体", 12, 87) );
    GLOBAL::setIcon( QChar(0xf52d), menuArticle, "#969696", 13, Qt::AlignVCenter, 40);
    GLOBAL::hoverIconColor(menuArticle, "#646464");
    triggerMenuHLayout->addWidget(menuArticle);


    //动态
    menuFeed = new LabelMouseEvent("动态", this);
    menuFeed->setObjectName("menuFeed");
    menuFeed->setStyleSheet("LabelMouseEvent#menuFeed{"
                            "color:#969696;}"
                            "QLabel:hover{"
                            "border-bottom:4px solid;}"
                            "QLabel#menuFeed:hover{"
                            "color:#646464;}"
                            );
    menuFeed->setFixedSize(200,80);
    menuFeed->setAlignment(Qt::AlignVCenter);
    menuFeed->setIndent(90);
    menuFeed->setFont( QFont("黑体", 12, 87) );
    GLOBAL::setIcon( QChar(0xf0f3), menuFeed, "#969696", 13, Qt::AlignVCenter, 40);
    GLOBAL::hoverIconColor(menuFeed, "#646464");
    triggerMenuHLayout->addWidget(menuFeed);


    //评论
    menuComments = new LabelMouseEvent("最新评论", this);
    menuComments->setObjectName("menuComments");
    menuComments->setStyleSheet("LabelMouseEvent#menuComments{"
                            "color:#969696;}"
                            "QLabel:hover{"
                            "border-bottom:4px solid;}"
                            "QLabel#menuComments:hover{"
                            "color:#646464;}"
                            );
    menuComments->setFixedSize(260,80);
    menuComments->setAlignment(Qt::AlignVCenter);
    menuComments->setIndent(90);
    menuComments->setFont( QFont("黑体", 12, 87) );
    GLOBAL::setIcon( QChar(0xf086), menuComments, "#969696", 13, Qt::AlignVCenter, 40);
    GLOBAL::hoverIconColor(menuComments, "#646464");
    triggerMenuHLayout->addWidget(menuComments);


    //热门
    menuHot = new LabelMouseEvent("热门", this);
    menuHot->setObjectName("menuComments");
    menuHot->setStyleSheet("LabelMouseEvent#menuComments{"
                            "color:#969696;}"
                            "QLabel:hover{"
                            "border-bottom:4px solid;}"
                            "QLabel#menuComments:hover{"
                            "color:#646464;}"
                            );
    menuHot->setFixedSize(200,80);
    menuHot->setAlignment(Qt::AlignVCenter);
    menuHot->setIndent(90);
    menuHot->setFont( QFont("黑体", 12, 87) );
    GLOBAL::setIcon( QChar(0xf06d), menuHot, "#969696", 13, Qt::AlignVCenter, 40);
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

    /*滑动窗口内的控件*/
    articleListScroll->setWidget(articleFram);

    /*作者信息窗口添加文章列表*/
    firstWidgetVLayout->addWidget(articleListScroll);

    /*将作者信息添加进界面容器*/
    stackWidget = new QStackedWidget(this);
    stackWidget->addWidget(firstWidget);
    stackWidget->setCurrentWidget(firstWidget);

    /*顶层界面显示stackWidget*/
    QVBoxLayout* topVLayout = new QVBoxLayout(this);
    topVLayout->setMargin(0);
    topVLayout->setSpacing(0);
    topVLayout->addWidget(stackWidget);
}

void Widget_AuthorInfo::initConnect()
{
    connect(&worker, SIGNAL(sigSendResult(std::vector<std::map<QString,QString> >)),
                            this, SLOT(sltGetResult(std::vector<std::map<QString,QString> >)));
}

/*result的第0个元素为作者信息，其余元素为文章信息*/
void Widget_AuthorInfo::sltGetResult(std::vector<std::map<QString, QString> > result)
{
    /*更新要显示的作者信息*/
    author->update(result[0]);

    int articleNumber = result.size() - 1;

    for(int i = 0; i < articleNumber; ++i){
        /*当文章数量过多，列表无法显示时，创建新的widget_enrtyArticle显示*/
        if(articleList.size() < articleNumber){
            articleList.push_back( new ArticleItem(this) );
            articleFramVLayout->addWidget( articleList[i] );

            /*点击文章标题则触发sltLoadArticLe槽函数*/
            connect(articleList[i], SIGNAL(sigLoadArticle(QString)),
                    this, SLOT(sltLoadArticle(QString)));
        }
        /*更新显示的文章信息*/
        articleList[i]->update(result[i+1]);
    }
}

/*创建新界面显示文章*/
void Widget_AuthorInfo::sltLoadArticle(QString url)
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

