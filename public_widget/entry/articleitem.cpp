#include "articleitem.h"
#include "ui_articleitem.h"
#include <QDebug>
#include <QFontDatabase>
#include "global.h"
#include "labelmouseevent.h"

ArticleItem::ArticleItem(QWidget *parent) :
    Widget_Entry(parent),
    ui(new Ui::ArticleItem)
{
    ui->setupUi(this);
    initWindow();
    initVariable();
    initConnect();
}

ArticleItem::~ArticleItem()
{
    delete ui;

}

void ArticleItem::update(std::map<QString, QString> &data)
{
    setLink(data["link"]);
    ui->title->setLink( data["link"] );

    //setAvatar(data["avatar"]);
    /*
    *属性列表：
    * title : 文章标题
    * content : 大致内容
    * nickname : 作者名字
    * comments : 文章评论数量
    * likes : 文章被喜欢数
    * link : 文章链接
    * userHomePage : 文章作者个人主页链接
    * clicks : 文章点击量
    * money : 被打赏量
    * time : 文章发布时间
    * diamonds : 钻石
    * image : 展示图片的url
    * paid : 付费文章
    */
    ui->title->setText(data["title"]);
    ui->content->setText( data["content"]);

    /*用户名标签*/
    if ( data["nickname"] != NULL){
        if(nickname == 0){
            nickname = new LabelMouseEvent(ui->widget);
            nickname->setCursor(Qt::PointingHandCursor);
            nickname->setContentsMargins(0,0,40,0);
            bottomHLayout->insertWidget(bottomHLayout->count() - 1, nickname);
            connect(nickname, SIGNAL(sigClicked()), this, SLOT(sltClickedAuthor()));
        }

        /*设置用户个人主页Url*/
        if ( data["userHomePage"] != NULL ){
            nickname->setLink( data["userHomePage"] );
        }

        nickname->setText(data["nickname"]);
        nickname->setVisible(true);
    }else if(nickname != 0){
        nickname->clear();
        nickname->setVisible(false);
    }

    /*点击量*/
    if( data["clicks"] != NULL ){
       if(clicks == 0){
           clicks = new QLabel(ui->widget);
           bottomHLayout->insertWidget(bottomHLayout->count() - 1, clicks);
           /*眼睛图标*/
           GLOBAL::setIcon(QChar(0xf06e), clicks, "#999999", 8);
           clicks->setFixedWidth(120);
           clicks->setIndent(23);
           clicks->setAlignment(Qt::AlignLeft);
       }
       clicks->setText( data["clicks"] );
       clicks->setVisible(true);
    }else if(clicks != 0){
        clicks->clear();
        clicks->setVisible(false);
    }


    /*时间*/
    if( data["time"] != NULL ){
       if(time == 0){
           time = new QLabel(ui->widget);
           bottomHLayout->insertWidget(bottomHLayout->count() - 1, time);

           GLOBAL::setIcon(QChar(0xf017), time, "#999999", 8);
           time->setFixedWidth(310);
           time->setIndent(23);
           time->setAlignment(Qt::AlignLeft);
       }

       time->setText( data["time"] );
       time->setVisible(true);
    }else if(time != 0){
        time->clear();
        time->setVisible(false);
    }


    /*钻石标签*/
    if( data["diamonds"] != NULL ){
        if(diamonds == 0){
            diamonds = new QLabel(ui->widget);
            bottomHLayout->insertWidget(bottomHLayout->count() - 1 > 0 ? 1 : 0, diamonds);
            /**/
            GLOBAL::setIcon(QChar(0xf3a5),diamonds, "red", 8);
            diamonds->setFixedWidth(120);
            diamonds->setIndent(24);
            diamonds->setAlignment(Qt::AlignLeft);

        }

        diamonds->setText(data["diamonds"]);
        diamonds->setVisible(true);
    }else if(diamonds != 0){
        diamonds->clear();
        diamonds->hide();
    }

    /*付费文章*/
    if ( data["paid"] != NULL){
        if(paid == 0){
            paid = new QLabel(ui->widget);
            bottomHLayout->insertWidget(bottomHLayout->count() - 1, paid);
            /*评论图标*/
            GLOBAL::setIcon(QChar(0xf51e), paid, "gold", 8);
            paid->setFixedWidth(120);
            paid->setIndent(23);
            paid->setStyleSheet("color:gold;");
            paid->setAlignment(Qt::AlignLeft);
        }
        paid->setText(data["paid"]);
        paid->setVisible(true);
    }else if(paid != 0){
        paid->clear();
        paid->setVisible(false);
    }


    /*评论数量*/
    if ( data["comments"] != NULL){
        if(comments == 0){
            comments = new LabelMouseEvent(ui->widget);
            bottomHLayout->insertWidget(bottomHLayout->count() - 1, comments);
            /*评论图标*/
            GLOBAL::setIcon(QChar(0xf27a), comments, "", 8);
            comments->setFixedWidth(120);
            comments->setIndent(21);
            comments->setAlignment(Qt::AlignLeft);
        }
        comments->setText(data["comments"]);
        comments->setVisible(true);
    }else if(comments != 0){
        comments->clear();
        comments->setVisible(false);
    }

    /*喜欢数量*/
    if ( data["likes"] != NULL){
        if(likes == 0){
            likes = new QLabel(ui->widget);
            bottomHLayout->insertWidget(bottomHLayout->count() - 1, likes);
            /*喜欢图标*/
            GLOBAL::setIcon(QChar(0xf004), likes, "rgb(238,119,102)", 8);
            likes->setFixedWidth(120);
            likes->setIndent(20);
            likes->setAlignment(Qt::AlignLeft);
        }
        likes->setText(data["likes"]);
        likes->setVisible(true);
    }else if(likes != 0){
        likes->clear();
        likes->setVisible(false);
    }

    /*打赏次数*/
    if( data["money"] != NULL ){
        if(money == 0){
            money = new QLabel(ui->widget);
            bottomHLayout->insertWidget(bottomHLayout->count() - 1, money);
            /*打赏图标*/
            GLOBAL::setIcon(QChar(0xf3d1), money, "gold", 8);
            money->setFixedWidth(120);
            money->setIndent(20);
            money->setAlignment(Qt::AlignLeft);
        }

        money->setText(data["money"]);
        money->setVisible(true);
    }else if(money != 0){
        money->clear();
        money->hide();
    }


    if(data["image"] != NULL){
        image.loadImage( data["image"] );
    }else{
        ui->image->clear();
        ui->image->setVisible(false);
    }
}

void ArticleItem::initWindow()
{
    /*底部用户名、时间、点击量等内容的布局*/
    bottomHLayout = new QHBoxLayout(ui->widget);
    bottomHLayout->setContentsMargins(20,5,0,9);
    bottomHLayout->addStretch();
    ui->widget->setFixedHeight(40);

    ui->image->setStyleSheet("QLabel#image{ padding-top:20px; padding-right:20px; }");
    ui->image->setVisible(false);
}

void ArticleItem::initVariable()
{
    image.moveToThread( GLOBAL::thread );
}

void ArticleItem::initConnect()
{
   connect(ui->title, SIGNAL(sigClicked()), this, SLOT(sltClickedTitle()));


   connect( &image, &MyImage::sigLoadFinish, this,
               [=](){
                   ui->image->setPixmap( image.scaled(350,250) );
                   image.loadFromData("");
                   ui->image->setVisible(true);
               } );
}


void ArticleItem::sltClickedTitle()
{
    QString url = ((LabelMouseEvent*)sender())->getLink();
    emit sigLoadArticle(url);
}

void ArticleItem::sltClickedAuthor()
{
    QString url = ((LabelMouseEvent*)sender())->getLink();
    emit sigLoadAuthor(url);
}
