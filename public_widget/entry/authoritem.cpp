#include "authoritem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "global.h"

AuthorItem::AuthorItem(QWidget *parent) : Widget_Entry(parent)
{
    iniWindow();
    initVariable();
    iniConnect();
}


void AuthorItem::update(std::map<QString, QString> & data)
{
    QString att[] = {"following","follower", "article", "words", "likes",
                     "property",  "article_url", "follower_url", "following_url"};

    /*
    * data键的意义：
    * name : 用户昵称
    * avatar_url : 用户头像
    * following : 关注的人的数量
    * following_url : 关注的人的列表url
    * follower  : 粉丝数
    * follower_url  : 粉丝列表Url
    * article : 发布的文章数
    * article_url : 文章列表url
    * words : 字数
    * likes : 收获的喜欢
    * property : 总资产
    */
    myImage.loadImage( data["avatar_url"] );


    name->setText(data["name"]);
    following->setText(data["following"] + " 关注");
    following->setLink( data["following_url"] );

    followers->setText(data["follower"] + " 粉丝");
    followers->setLink(data["follower_url"]);

    articles->setText(data["article"] + " 文章");
    articles->setLink(data["article_url"]);

    words->setText(data["words"] + " 字数");
    likes->setText(data["likes"] + " 收获喜欢");
    property->setText(data["property"] + " 总资产");
}


void AuthorItem::iniWindow()
{
    QHBoxLayout* topHLayout = new QHBoxLayout(this);

    avatar = new QLabel(this);
    avatar->setFixedSize(140,140);
    avatar->setStyleSheet("background:transparent;");
    topHLayout->addWidget(avatar);
    topHLayout->setAlignment(avatar, Qt::AlignTop);
    topHLayout->addSpacing(20);

    QVBoxLayout* rightVLayout = new QVBoxLayout();

    name = new QLabel(this);
    name->setText("六十四");
    name->setFont( QFont("黑体", 13, 87) );
    rightVLayout->addWidget(name);

    QHBoxLayout* hLayout = new QHBoxLayout();

    QString sty = "QLabel{border:0px solid; color:#646464; border-right:2px solid #dddddd;}";

    following = new LabelMouseEvent(this);
    following->setContentsMargins(0, 0, 8, 0);
    following->setStyleSheet(sty);
    hLayout->addWidget(following);

    followers = new LabelMouseEvent(this);
    followers->setContentsMargins(0, 0, 8, 0);
    followers->setStyleSheet(sty);
    followers->setText("1323 粉丝");
    hLayout->addWidget(followers);

    articles = new LabelMouseEvent(this);
    articles->setText("123 文章");
    articles->setContentsMargins(0, 0, 8, 0);
    articles->setStyleSheet(sty);
    articles->setStyleSheet(sty);
    hLayout->addWidget(articles);

    hLayout->addStretch();
    rightVLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();
    words = new QLabel(this);
    words->setContentsMargins(0, 0, 8, 0);
    words->setStyleSheet(sty);
    words->setText("81923 字数");
    words->setStyleSheet(sty);
    hLayout->addWidget(words);

    likes = new QLabel(this);
    likes->setContentsMargins(0, 0, 8, 0);
    likes->setStyleSheet(sty);
    likes->setText("123 喜欢");
    hLayout->addWidget(likes);

    property = new QLabel(this);
    property->setContentsMargins(0, 0, 8, 0);
    property->setStyleSheet(sty);
    property->setText("123 总资产");
    hLayout->addWidget(property);

    hLayout->addStretch();
    rightVLayout->addLayout(hLayout);
    rightVLayout->addStretch();
    rightVLayout->setSpacing(20);

    topHLayout->addLayout(rightVLayout);


}

void AuthorItem::initVariable()
{
    myImage.moveToThread(GLOBAL::thread);
}

void AuthorItem::iniConnect()
{
    connect( &myImage, &MyImage::sigLoadFinish, this,
                [=](){
                    avatar->setPixmap( myImage.radius(avatar->width(), avatar->height(), avatar->width()/2) );
                    myImage.loadFromData("");
                } );
}
