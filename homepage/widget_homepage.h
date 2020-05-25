#ifndef WIDGET_HOMEPAGE_H
#define WIDGET_HOMEPAGE_H

#include <QWidget>
#include <map>
#include <QBoxLayout>
#include <QStackedWidget>
#include "workerofhomepage.h"
#include "widget_entry.h"
#include "public_widget/entry/articleitem.h"
#include "public_widget/article/widget_article.h"
#include "author/widget_authorinfo.h"

namespace Ui {
class Widget_HomePage;
}

class Widget_HomePage : public QWidget
{
    Q_OBJECT

signals:


public:
    explicit Widget_HomePage(QWidget *parent = 0);
    ~Widget_HomePage();


private:
    void initConnect();
    void initVariable();
    void initWindow();
    void search();


private slots:
    void sltLoadArticle(QString url);
    void sltLoadAuhtor(QString url);
    void sltGetResult( std::vector< std::map<QString, QString> > result);
    void on_pushButton_prevPage_clicked();
    void on_pushButton_nextPage_clicked();

private:
    Ui::Widget_HomePage *ui;
    Widget_Article* article = 0;    //显示文章
    Widget_AuthorInfo* author = 0;   //显示作者详情

    WorkerOfHomePage worker;
    QVBoxLayout* layout;
    ArticleItem* entries[10];    //widget，用于显示搜索的结果

    int page = 1;
};

#endif // WIDGET_HOMEPAGE_H
