#include "recommendationsauthoritem.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include "global.h"

RecommendationsAuthorItem::RecommendationsAuthorItem(QWidget *parent) :
    Widget_Entry(parent)
{
    initWindow();
    initVariabel();
    initConnect();
}

void RecommendationsAuthorItem::update(std::map<QString, QString> & data)
{
    /*
    ** 键值
    ** user_url : 用户主页
    ** user_nickname : 用户名
    ** user_intro : 用户简介
    ** user_notes_x_article_title : 最近文章标题，倒数第x篇
    ** user_notes_x_article_url : 最近文章链接，倒数第x篇
    ** user_avatar_url : 用户头像Url
    ** user_sex : 性别
    */

    setLink( data["user_url"] );

    name->setText(data["user_nickname"]);
    description->setText( data["user_intro"] );

    if(data["user_sex"] != NULL){
        if(data["user_sex"] == "man"){

        }else{

        }
        sex->setVisible(true);
    }else{
        sex->setVisible(false);
    }


    if(data["user_notes_0_article_title"] != NULL){
       article1->setText(data["user_notes_0_article_title"]);
       article1->setLink(data["user_notes_0_article_url"]);
       article1->setVisible(true);
    }else{
       article1->setVisible(false);
    }

    if(data["user_notes_1_article_title"] != NULL){
       article2->setText(data["user_notes_1_article_title"]);
       article2->setLink(data["user_notes_1_article_url"]);
       article2->setVisible(true);
    }else{
       article2->setVisible(false);
    }


    if(data["user_notes_2_article_title"] != NULL){
       article3->setText(data["user_notes_2_article_title"]);
       article3->setLink(data["user_notes_2_article_url"]);
       article3->setVisible(true);
    }else{
       article3->setVisible(false);
    }

    image.loadImage( data["user_avatar_url"] );
    connect( &image, &MyImage::sigLoadFinish, this,
                [=](){
                    avatar->setPixmap( image.radius(avatar->width(), avatar->height(), avatar->width()/2) );
                } );
}

void RecommendationsAuthorItem::initWindow()
{
    setFixedSize(560,650);


    /*头像*/
    avatar = new QLabel(this);
    avatar->setFixedSize(150,150);


    /*信息块--包含昵称、描述、文章等信息的方块*/
    infoItem = new Widget_Entry(this);
    infoItem->setFixedSize(530,565);
    infoItem->setObjectName("infoItem");
    infoItem->setCursor(Qt::PointingHandCursor);
    infoItem->setStyleSheet("QWidget#infoItem{"
                            "border:2px solid #d6d6d6;"
                            "border-radius:15px;}");


    QVBoxLayout* infoVLayout = new QVBoxLayout(infoItem);
    infoVLayout->setMargin(0);
    infoVLayout->setSpacing(0);
    infoVLayout->addSpacing(110);

    /*姓名与性别水平布局*/
    QHBoxLayout* name_sexHLayout = new QHBoxLayout();

    /*姓名*/
    name = new QLabel(infoItem);
    name->setFont( QFont("黑体", 14, 87) );
    name->setText("123");
    name_sexHLayout->addStretch();
    name_sexHLayout->addWidget(name);


    /*性别*/
    sex = new QLabel(infoItem);
    name_sexHLayout->addWidget(sex);
    name_sexHLayout->addStretch();

    infoVLayout->addLayout(name_sexHLayout);


    /*描述*/
    description = new QLabel(infoItem);
    description->setStyleSheet("color:#333333;");
    description->setAlignment(Qt::AlignHCenter);
    description->setMinimumHeight(100);
    description->setFixedWidth(350);
    description->setWordWrap(true);
    infoVLayout->addSpacing(20);
    infoVLayout->addWidget(description);
    infoVLayout->setAlignment(description, Qt::AlignCenter);


    //关注
    follow = new LabelMouseEvent(infoItem);
    follow->setFixedSize(170,65);
    follow->setFont( QFont("黑体", 13, 87) );
    follow->setAlignment(Qt::AlignCenter);
    follow->setObjectName("follow");
    follow->setStyleSheet("QLabel{"
                          "background:#42C02E;"
                          "color:white;"
                          "border-radius:32px;}");
    follow->setText("+ 关注");
    infoVLayout->addSpacing(20);
    infoVLayout->addWidget(follow);
    infoVLayout->setAlignment(follow, Qt::AlignCenter);


    /*最近更新水平布局*/
    QHBoxLayout* recentUpdateHLayout = new QHBoxLayout();
    recentUpdateHLayout->setMargin(0);
    recentUpdateHLayout->setSpacing(0);

    /*文字--最近更新*/
    QLabel* tip_recentUpdate = new QLabel(infoItem);
    tip_recentUpdate->setText("最近更新");
    tip_recentUpdate->setStyleSheet("color:#969696;");
    recentUpdateHLayout->addSpacing(50);
    recentUpdateHLayout->addWidget(tip_recentUpdate);

    /*淡灰色分割线*/
    QLabel* tip_line = new QLabel(infoItem);
    tip_line->setFixedWidth(380);
    tip_line->setStyleSheet("border-top:1px solid #bebebe;"
                            "margin-right:50px;"
                            "margin-top:28px;");
    tip_line->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    recentUpdateHLayout->addWidget(tip_line);

    infoVLayout->addLayout(recentUpdateHLayout);


    QString hoverUnderLine = "QLabel:hover{"
                             "border-bottom:2px solid;}";

    /*文章1*/
    article1 = new LabelMouseEvent(infoItem);
    article1->setMaximumWidth(450);
    article1->setStyleSheet(hoverUnderLine);
    article1->setCursor(Qt::PointingHandCursor);
    infoVLayout->addSpacing(20);
    infoVLayout->addWidget(article1);
    infoVLayout->setAlignment(article1, Qt::AlignCenter);


    //文章2
    article2 = new LabelMouseEvent(infoItem);
    article2->setMaximumWidth(450);
    article2->setStyleSheet(hoverUnderLine);
    article2->setCursor(Qt::PointingHandCursor);
    infoVLayout->addSpacing(20);
    infoVLayout->addWidget(article2);
    infoVLayout->setAlignment(article2, Qt::AlignCenter);



    /*文章3*/
    article3 = new LabelMouseEvent(infoItem);
    article3->setMaximumWidth(450);
    article3->setStyleSheet(hoverUnderLine);
    article3->setCursor(Qt::PointingHandCursor);
    infoVLayout->addSpacing(20);
    infoVLayout->addWidget(article3);
    infoVLayout->setAlignment(article3, Qt::AlignCenter);


    infoVLayout->addStretch();


    avatar->setGeometry(200,0,150,150);
    infoItem->setGeometry(10,75,0,0);
    avatar->raise();

    /*设置*/
    shadow = new QGraphicsDropShadowEffect(infoItem);
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor("white"));
    //设置阴影圆角
    shadow->setBlurRadius(30);
    infoItem->setGraphicsEffect(shadow);
}


void RecommendationsAuthorItem::initVariabel()
{
    image.moveToThread(GLOBAL::thread);
}

void RecommendationsAuthorItem::initConnect()
{
    connect(article1, &LabelMouseEvent::sigClicked, this, [=]{
        emit sigLoadArticle(article1->getLink());
    } );

    connect(article2, &LabelMouseEvent::sigClicked, this, [=]{
        emit sigLoadArticle(article2->getLink());
    } );

    connect(article3, &LabelMouseEvent::sigClicked, this, [=]{
        emit sigLoadArticle(article3->getLink());
    } );

    connect(infoItem, &Widget_Entry::sigEnter, this, [=]{
        shadow->setColor("#969696");
    });

    connect(infoItem, &Widget_Entry::sigLeave, this, [=]{
        shadow->setColor("white");
    });

    connect(infoItem, &Widget_Entry::sigClicked, this, [=]{
        emit sigLoadAuthor( this->getLink() );
    });

}


