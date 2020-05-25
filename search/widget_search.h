#ifndef WIDGET_SEARCHRESULT_H
#define WIDGET_SEARCHRESULT_H

#include <QWidget>
#include <QVBoxLayout>
#include "widget_entry.h"
#include "workerofsearch.h"


namespace Ui {
class Widget_Search;
}

class Widget_Search : public QWidget
{
    Q_OBJECT

public:
    explicit Widget_Search(QWidget *parent = 0);
    ~Widget_Search();
    void search();
    void search(const QString&, const QString&);

private:
    void initWindow();
    void initVariable();
    void initConnect();

private slots:
    void sltGetResult( std::vector< std::map<QString, QString> > result);

    void on_pushButton_nextPage_clicked();

    void on_pushButton_prevPage_clicked();

    void on_pushButton_note_clicked();

    void on_pushButton_user_clicked();

    void on_pushButton_collection_clicked();

    void on_pushButton_notebook_clicked();

private:
    Ui::Widget_Search *ui;
    WorkerOfSearch worker;
    Widget_Entry* entries[10];    //widget，用于显示搜索的结果
    QVBoxLayout* layout;

    //以下4个为搜索时使用的参数
    QString searchContent;              //要搜索的内容
    unsigned short page = 1;            //当前搜索的页数
    QString type = "note";              //类型：note（文章）,user（用户）,collection（专题）,notebook（文集）
    QString order_by = "default";       //排序类型：default（综合），top（热门），published_at（最新发布），commented_at（最新评论）
    QString time_range;                 //发布时间：a_week（最近一周），a_day（最近一天），three_months（最近三天），空(时间不限)
};

#endif // WIDGET_SEARCHRESULT_H
