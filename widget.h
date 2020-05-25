#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "search/widget_search.h"
#include "homepage/widget_homepage.h"
#include "httprequest.h"
#include "../../day0/Image/myimage.h"
#include "books/widget_books.h"
#include "publications/widget_publications.h"
#include "author/widget_authorinfo.h"
#include "shcool/widget_shcool.h"
#include "recommendations/widget_recommendationsauthor.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void initWindow();
    void clearSubWidget();  //回收各个模块占用的占用

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

signals:
    void sigSearch(const QString& content, const QString& page);

private slots:
    void on_pushButton_Search_clicked();
    void on_pushButton_HomePage_clicked();
    void on_pushButton_Books_clicked();
    void on_pushButton_Publications_clicked();

    void on_pushButton_School_clicked();

    void on_pushButton_author_clicked();

private:
    Ui::Widget *ui;

    Widget_HomePage* homePageWidget = 0;    //首页
    Widget_Search* searchWidget = 0;        //搜索
    Widget_Books* booksWidget = 0;          //简书优选
    Widget_Publications* publicationWidget = 0; //简书版权
    Widget_Shcool* shcoolWidget = 0;        //简书大学堂
    Widget_RecommendationsAuthor* recommendationsAuthor = 0; //推荐作者

    int m_nBorderWidth; //m_nBorder表示鼠标位于边框缩放范围的宽度

};

#endif // WIDGET_H
