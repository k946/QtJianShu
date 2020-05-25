#include "widget_entrypublicationsbook.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "global.h"
#include <QDebug>

Widget_EntryPublicationsBook::Widget_EntryPublicationsBook(QWidget *parent):
    Widget_Entry(parent)
{
    initWindow();
    initVariable();
    initConnect();
}

Widget_EntryPublicationsBook::~Widget_EntryPublicationsBook()
{
}

void Widget_EntryPublicationsBook::update(std::map<QString, QString> & data)
{
    std::map<QString, QString>::iterator it = data.begin();
    std::map<QString, QString>::iterator end = data.end();

    for(;it != end; ++it){
        QString key = it->first;
        QString attr = key.section("_", 2, 2);
        if( "name" == attr){
            name->setText( it->second );    //标题
        }else if( "author" == attr ){
            author->setText( it->second );  //作者
        }else if( "intros" == attr ){
            intros->setText( it->second );  //简介
        }else if( "price" == attr ){
            price->setText( it->second );  //价格
        }else if( "url" == attr){
            setLink(it->second);         //购买链接
        }else if( "cover" == attr){
            myImage.loadImage( it->second );
        }
    }
}

void Widget_EntryPublicationsBook::initWindow()
{
    this->setFixedWidth(670);

    //整体水平布局
    QHBoxLayout* topLayout = new QHBoxLayout(this);

    //书籍封面
    img = new QLabel(this);
    img->setMargin(0);
    img->setFixedSize(250,320);
    img->setStyleSheet("background:transparent;");
    topLayout->addWidget(img);
    topLayout->addSpacing(20);
    topLayout->setAlignment(img, Qt::AlignTop);

    //右侧垂直布局
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setMargin(0);
    vLayout->setSpacing(5);

    //书名
    name = new QLabel(this);
    name->setFixedSize(380,88);
    name->setText("书名");
    name->setIndent(0);
    name->setAlignment(Qt::AlignLeft);
    name->setWordWrap(true);
    name->setFont(QFont("黑体", 12, 87));
    name->setStyleSheet("color:#333333; margin-top:20px;");
    vLayout->addWidget(name);

    //作者
    author = new QLabel(this);
    author->setFixedSize(380,38);
    author->setIndent(0);
    author->setText("作者");

    author->setFont(QFont("宋体", 9));
    author->setStyleSheet("color:#999999; margin-top:5px;");
    vLayout->addWidget(author);

    //简介
    intros = new QLabel(this);
    intros->setWordWrap(true);
    intros->setFixedSize(380,78);
    intros->setText("简介");
    intros->setAlignment(Qt::AlignLeft);
    intros->setStyleSheet("color:#999999; font:9pt \"黑体\"; ");
    vLayout->addWidget(intros);


    //价格
    price = new QLabel(this);
    price->setFixedSize(380,38);
    price->setText("￥ 39.0");
    price->setStyleSheet("color:#333333; font:9pt \"黑体\";");
    vLayout->addSpacing(20);
    vLayout->addWidget(price);
    vLayout->addStretch();

    topLayout->addLayout(vLayout);
}

void Widget_EntryPublicationsBook::initConnect()
{
    connect( &myImage, &MyImage::sigLoadFinish, this,
                [=](){
                    img->setPixmap( myImage.scaled(img->width(), img->height()) );
                    myImage.loadFromData("");
                } );
}

void Widget_EntryPublicationsBook::initVariable()
{
    myImage.moveToThread(GLOBAL::thread);
}
