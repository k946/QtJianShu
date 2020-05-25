#ifndef WIDGET_RECOMMENDATIONSAUTHOR_H
#define WIDGET_RECOMMENDATIONSAUTHOR_H

#include <QWidget>
#include <map>
#include <QBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include "public_widget/article/widget_article.h"
#include "author/widget_authorinfo.h"
#include "item/recommendationsauthoritem.h"
#include "workerofrecommendationsauthor.h"

class Widget_RecommendationsAuthor : public QWidget
{
    Q_OBJECT
public:
    explicit Widget_RecommendationsAuthor(QWidget *parent = nullptr);


private:
    void initConnect();
    void initVariable();
    void initWindow();
    void search();


private slots:
    void sltLoadArticle(QString url);
    void sltLoadAuhtor(QString url);
    void sltGetResult( std::vector< std::map<QString, QString> > result);
    void sltNextpage();
    void sltPrevpage();

private:

    QWidget* firstWidget = 0;
    Widget_Article* article = 0;    //显示文章
    Widget_AuthorInfo* author = 0;   //显示作者详情


    QScrollArea* authorListArea;
    QStackedWidget* stackedWidget;
    RecommendationsAuthorItem* authorItems[24];    //widget，用于显示搜索的结果

    QPushButton* pushButton_prevpage;
    QPushButton* pushButton_nextpage;

    int page = 1;

    WorkerOfRecommendationsAuthor worker;

};

#endif // WIDGET_RECOMMENDATIONSAUTHOR_H
