#include "widget_entrybooks.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "global.h"

Widget_EntryBooks::Widget_EntryBooks(QWidget* parent):
    Widget_Entry(parent)
{
    initWindow();
    initVariable();
    initConnect();
}

void Widget_EntryBooks::update(std::map<QString, QString> &data)
{
    title->setText(data["name"]);
    nickname->setText(data["user_nickname"]);
    int n = data["views_count"].toInt();
    if(n > 1000){
        readNumber->setText( QString::number( n/1000 ) + "." +
                             QString::number( (n%1000)/100 )+ "k阅读" );
    }else{
        readNumber->setText( data["views_count"] + "阅读" );
    }

    user_url = data["user_url"];
    book_url = data["book_url"];


    myImage1.loadImage( data["image_url"] );
    myImage2.loadImage( data["user_avatar_url"] );



}

void Widget_EntryBooks::initWindow()
{
    this->setFixedWidth(300);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setMargin(0);

    //图书封面
    image = new QLabel(this);
    image->setFixedSize(300,400);
    image->setMargin(0);
    image->setStyleSheet("QLabel#image{ }");
    vLayout->addWidget(image);
    vLayout->addStretch();

    //图示标题
    title = new LabelMouseEvent(this);
    title->setFont( QFont("宋体", 10, 75) );
    title->setFixedWidth(300);
    title->setFixedHeight(65);
    title->setCursor(Qt::PointingHandCursor);
    title->setWordWrap(true);
    title->setStyleSheet("QLabel{ }");
    vLayout->addWidget(title);

    headImage = new QLabel(this);
    headImage->setFixedSize(32,32);
    headImage->setCursor(Qt::PointingHandCursor);


    nickname = new LabelMouseEvent(this);
    nickname->setFixedSize(250,32);
    nickname->setCursor(Qt::PointingHandCursor);
    nickname->setFont( QFont("宋体", 8) );
    nickname->setStyleSheet("QLabel{ color:rgb(99,99,99)  }");
    nickname->setIndent(35);

    QHBoxLayout* hLayout = new QHBoxLayout(nickname);
    hLayout->setMargin(0);
    hLayout->addWidget(headImage);
    hLayout->addStretch();

    vLayout->addWidget(nickname);
    vLayout->addStretch(0);

    readNumber = new QLabel(this);
    readNumber->setFixedHeight(32);
    readNumber->setFont( QFont("宋体", 8) );
    readNumber->setStyleSheet("QLabel{ color:rgb(99,99,99)  }");
    vLayout->addWidget(readNumber);
    vLayout->addStretch(0);

}

void Widget_EntryBooks::initConnect()
{
    connect( &myImage1, &MyImage::sigLoadFinish, this,
                [=](){
                    image->setPixmap( myImage1.scaled(image->width(), image->height() ) );
                    myImage1.loadFromData("");
                }, Qt::QueuedConnection );


    connect( &myImage2, &MyImage::sigLoadFinish, this,
                [=](){
                    headImage->setPixmap( myImage2.scaled(32,32) );
                    myImage2.loadFromData("");
                }, Qt::QueuedConnection );
}

void Widget_EntryBooks::initVariable()
{
    myImage1.moveToThread(GLOBAL::thread);
    myImage2.moveToThread(GLOBAL::thread);
}
