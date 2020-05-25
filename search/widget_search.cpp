#include "widget_search.h"
#include "ui_widget_search.h"
#include "global.h"
#include <QGraphicsDropShadowEffect>
#include "public_widget/entry/articleitem.h"

Widget_Search::Widget_Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget_Search)
{
    ui->setupUi(this);

    initVariable();
    initWindow();
    initConnect();

}

Widget_Search::~Widget_Search()
{
    delete ui;
}

void Widget_Search::search()
{
    worker.work(searchContent, QString::number(page), type, order_by, time_range);
}


void Widget_Search::search(const QString & content, const QString &page)
{
    this->searchContent = content;
    this->page = page.toInt();
    search();
}

void Widget_Search::initWindow()
{
    ui->pushButton_note->setFocus();
    layout = new QVBoxLayout( ui->scrollArea->widget() );
    layout->setMargin(0);

    for(int i = 0; i < 10; ++i){
        entries[i] = new ArticleItem(this);
        entries[i]->hide();
        layout->addWidget(entries[i]);
    }

    GLOBAL::setIcon(QChar(0xf0a9), ui->pushButton_nextPage, "", 15, Qt::AlignLeft, 20);
    ui->pushButton_nextPage->setStyleSheet("background:transparent;");
    ui->pushButton_nextPage->setCursor(Qt::PointingHandCursor);

    GLOBAL::setIcon(QChar(0xf0a8), ui->pushButton_prevPage, "", 15, Qt::AlignRight, 20);
    ui->pushButton_prevPage->setStyleSheet("background:transparent;");
    ui->pushButton_prevPage->setCursor(Qt::PointingHandCursor);
}

void Widget_Search::initVariable()
{
    worker.moveToThread( GLOBAL::thread );
}

void Widget_Search::initConnect()
{
    connect(&worker, SIGNAL( sigSendResult(std::vector< std::map<QString, QString> >) ),
            this, SLOT( sltGetResult(std::vector< std::map<QString, QString> >) ));
}

void Widget_Search::sltGetResult( std::vector< std::map<QString, QString> > result)
{

    int size = result.size();

    for(int i = 0;i < size && i < 10 ; ++i){
        entries[i]->update(result[i]);
        entries[i]->setVisible(true);
    }

}

//下一页
void Widget_Search::on_pushButton_nextPage_clicked()
{

    page = (page + 1) % 101;        //最大页数为100
    search();
}

//上一页
void Widget_Search::on_pushButton_prevPage_clicked()
{

    page = (page - 1 + 101) % 101;
    search();

}

//搜索文章
void Widget_Search::on_pushButton_note_clicked()
{

    for(int i = 0; i < 10; ++i){
        //entries[i]->deleteLater();
    }

    this->type = "note";
    search();
}

//搜索用户
void Widget_Search::on_pushButton_user_clicked()
{
    for(int i = 0; i < 10; ++i){
        //entries[i]->deleteLater();
    }

    this->type = "user";
    search();
}

//搜索专题
void Widget_Search::on_pushButton_collection_clicked()
{
    for(int i = 0; i < 10; ++i){
        //layout->removeWidget(entries[i]);
    }

    this->type = "collection";
    search();
}

//搜索文集
void Widget_Search::on_pushButton_notebook_clicked()
{
    for(int i = 0; i < 10; ++i){
        //layout->removeWidget(entries[i]);
    }

    this->type = "notebook";
    search();
}

