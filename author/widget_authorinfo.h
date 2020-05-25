#ifndef WIDGET_AUTHOR_H
#define WIDGET_AUTHOR_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "labelmouseevent.h"
#include "public_widget/entry/articleitem.h"
#include "public_widget/entry/authoritem.h"
#include "workerofauthorinfo.h"
#include "public_widget/article/widget_article.h"


/*
** 用于显示作者信息（文章列表，粉丝数等）
*/

class Widget_AuthorInfo : public QWidget
{
    Q_OBJECT
public:
    explicit Widget_AuthorInfo(QWidget *parent = nullptr);
    void loadAuthorInfo(const QUrl& url);
private:
    void initWindow();
    void initConnect();

private slots:
    void sltGetResult(std::vector< std::map<QString, QString> >);
    void sltLoadArticle(QString url);

private:
    QUrl link; //当前页面的url

    QStackedWidget* stackWidget;    //界面容器，包含作者信息firstWidget和文章article
    QWidget* firstWidget = 0;           //当前界面的第一个Widget，即作者信息
    Widget_Article* article = 0;        //用于显示文章

    AuthorItem* author;     //作者信息块
    QScrollArea* articleListScroll;  //文章列表滑动窗口
    QVBoxLayout* articleFramVLayout; //文章列表布局
    std::vector<Widget_Entry* > articleList; //文章信息块
    LabelMouseEvent* menuArticle;   //文章
    LabelMouseEvent* menuFeed;      //动态
    LabelMouseEvent* menuComments;  //最新评论
    LabelMouseEvent* menuHot;       //热门

    WorkerOfAuthorInfo worker;      //请求作者主页的工人
};

#endif // WIDGET_AUTHOR_H
