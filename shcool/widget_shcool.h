#ifndef WIDGET_SHCOOL_H
#define WIDGET_SHCOOL_H

#include <QWidget>
#include <QStackedWidget>
#include "public_widget/article/widget_article.h"
#include "public_widget/entry/articleitem.h"
#include "public_widget/entry/triggermenuitem.h"
#include "public_widget/entry/collectionitem.h"
#include "workerofshcool.h"
#include "author/widget_authorinfo.h"

class Widget_Shcool : public QWidget
{
    Q_OBJECT
public:
    explicit Widget_Shcool(QWidget *parent = nullptr);

private:
    void initWindow();
    void initVariable();
    void initConnect();

signals:

private slots:
    void sltGetResult(std::vector< std::map<QString, QString> >);
    void sltLoadArticle(QString url);
    void sltLoadAuhtor(QString url);

private:
    QUrl link; //当前页面的url

    QStackedWidget* stackWidget;
    QWidget* firstWidget = 0;           //当前界面的第一个Widget
    Widget_Article* article = 0;        //用于显示文章
    Widget_AuthorInfo* author = 0;   //显示作者详情

    QScrollArea* articleListScroll;  //文章列表滑动窗口
    QVBoxLayout* articleFramVLayout; //文章列表布局
    std::vector<Widget_Entry* > articleList; //文章信息块

    CollectionItem* collectionItem;
    TriggerMenuItem* menuComments;  //最新评论
    TriggerMenuItem* menuAdded;     //最新收录
    TriggerMenuItem* menuHot;       //热门

    WorkerOfShcool worker;
};

#endif // WIDGET_SHCOOL_H
