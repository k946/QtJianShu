#include "widget_entrypublicationsauthor.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include "global.h"

Widget_EntryPublicationsAuthor::Widget_EntryPublicationsAuthor(QWidget *parent):
    Widget_Entry(parent)
{
    initWindow();
    initVariable();
    initConnect();
}

Widget_EntryPublicationsAuthor::~Widget_EntryPublicationsAuthor()
{

}

void Widget_EntryPublicationsAuthor::update(std::map<QString, QString> &data)
{
    /*
    ** 键值
    ** user_url : 用户主页
    ** user_nickname : 用户名
    ** user_intro : 用户简介
    ** user_notes_x_public_title : 最近文章标题，倒数第x篇
    ** user_notes_x_note_url : 最近文章链接，倒数第x篇
    ** user_avatar_url : 用户头像Url
    */

    setLink( data["user_url"] );

    name->setText(data["user_nickname"]);
    intro->setText( data["user_intro"] );
    keyWork->setText(data["key_work_title"]);
    keyWork->setLink( data["key_work_url"] );

    if(data["user_notes_0_public_title"] != NULL){
       article1->setText(data["user_notes_0_public_title"]);
       article1->setLink(data["user_notes_0_note_url"]);
       article1->setVisible(true);
    }else{
       article1->setVisible(false);
    }

    if(data["user_notes_1_public_title"] != NULL){
       article2->setText(data["user_notes_1_public_title"]);
       article2->setLink(data["user_notes_1_note_url"]);
       article2->setVisible(true);
    }else{
       article2->setVisible(false);
    }

    myImage.loadImage( data["user_avatar_url"] );

}

void Widget_EntryPublicationsAuthor::initWindow()
{
    this->setFixedWidth(600);
    this->setStyleSheet("border:2px;");

    //右侧垂直布局
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setObjectName("vLayout");
    vLayout->setMargin(0);
    vLayout->setSpacing(0);

    //头像
    avatar = new QLabel(this);
    avatar->setMargin(0);
    avatar->setFixedSize(128,128);
    avatar->setStyleSheet("background:transparent");

    //作者名
    name = new QLabel(this);

    name->setFixedSize(450,45);
    name->setText("名字");
    name->setFont(QFont("黑体", 12, 87));
    name->setStyleSheet("color:#333333");
    vLayout->addWidget(name);

    //简介
    intro = new QLabel(this);
    intro->setFixedSize(450,78);
    intro->setAlignment(Qt::AlignLeft);
    intro->setText("描述");
    intro->setFont(QFont("宋体", 9));
    intro->setWordWrap(true);
    intro->setStyleSheet("color:#333333");
    vLayout->addWidget(intro);

    //文集名
    keyWork = new LabelMouseEvent(this);
    keyWork->setFixedSize(450,40);
    keyWork->setCursor(Qt::PointingHandCursor);
    keyWork->setText("文集");
    keyWork->setStyleSheet("color:rgb(99,99,99); font:9pt \"黑体\";");
    vLayout->addWidget(keyWork);

    //最近的文章1
    article1 = new LabelMouseEvent(this);
    article1->setFixedSize(450,40);
    article1->setCursor(Qt::PointingHandCursor);
    article1->setText("2020-02-06");
    article1->setObjectName("article1");
    article1->setStyleSheet("LabelMouseEvent#article1{color:rgb(99,99,99); font:9pt \"黑体\";}");
    GLOBAL::setIcon( QChar(0xf02d), article1, "rgb(99,99,99)", 8, Qt::AlignVCenter );
    article1->setIndent(20);
    vLayout->addWidget(article1);

    //最近的文章2
    article2 = new LabelMouseEvent(this);
    article2->setFixedSize(450,40);
    article2->setCursor(Qt::PointingHandCursor);
    article2->setText("2020-02-06");
    article2->setObjectName("article2");
    article2->setStyleSheet("LabelMouseEvent#article2{color:rgb(99,99,99); font:9pt \"黑体\";}");
    GLOBAL::setIcon( QChar(0xf02d), article2, "rgb(99,99,99)", 8, Qt::AlignVCenter );
    article2->setIndent(20);
    vLayout->addWidget(article2);

    //去作者主页>
    authorpage = new QLabel(this);
    authorpage->setFixedSize(450,40);
    authorpage->setCursor(Qt::PointingHandCursor);
    authorpage->setText("去作者主页>");
    authorpage->setStyleSheet("color:#41867A; font:9pt \"黑体\";");
    vLayout->addWidget(authorpage);
    vLayout->addStretch(0);

    QHBoxLayout* topLayout = new QHBoxLayout(this);
    topLayout->setObjectName("topLayout");
    topLayout->addWidget(avatar);
    topLayout->addSpacing(10);
    topLayout->setAlignment(avatar, Qt::AlignTop);
    topLayout->addLayout(vLayout);
}

void Widget_EntryPublicationsAuthor::initConnect()
{
    connect( &myImage, &MyImage::sigLoadFinish, this,
                [=](){
                    avatar->setPixmap( myImage.radius(avatar->width(), avatar->height(), avatar->width()/2) );
                    myImage.loadFromData("");
                } );
}

void Widget_EntryPublicationsAuthor::initVariable()
{
    myImage.moveToThread(GLOBAL::thread);
}
