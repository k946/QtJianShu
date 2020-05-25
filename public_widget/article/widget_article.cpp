#include "widget_article.h"
#include <QDebug>
#include <QTextEdit>
#include "global.h"
#include "labelmouseevent.h"
#include <QScrollBar>

Widget_Article::Widget_Article(QWidget *parent) : QWidget(parent)
{

    initWindow();
    initVariable();
    initConnect();
}

void Widget_Article::loadArticle(const QString &url)
{
    worker.work(url);
    clearWidget();
}

void Widget_Article::initWindow()
{
    /*主窗口设置*/
    setStyleSheet("background:rgb(237,245,253);border:0px;");

    /*滑动窗口*/
    scrollArea = new QScrollArea(this);

    /*滑动窗口的主窗口*/
    scrollWidget = new QWidget(this);
    scrollVLayout = new QVBoxLayout(scrollWidget);
    scrollVLayout->setContentsMargins(10,10,0,10);
    scrollVLayout->setSpacing(20);

    /*设置滑动窗口主窗口，且主窗口的大小可变*/
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);

    /*主窗口布局*/
    QVBoxLayout* mainVLayout = new QVBoxLayout(this);
    mainVLayout->setMargin(0);
    mainVLayout->setSpacing(0);
    mainVLayout->addWidget(scrollArea);
}

void Widget_Article::initConnect()
{
    connect(&worker, SIGNAL(sigSendResult(std::vector<std::map<QString, QString> >)),
            this, SLOT(sltGetResult(std::vector<std::map<QString, QString> >)));
}

void Widget_Article::initVariable()
{
    worker.moveToThread(GLOBAL::thread);
}

void Widget_Article::clearWidget()
{
    QObjectList list = scrollWidget->children();
    int listSize = list.size();
    for(int i = 1; i < listSize; ++i){
        list[i]->deleteLater();
    }
}

void Widget_Article::sltGetResult(std::vector<std::map<QString, QString> > data)
{

    /*文章标题*/
    QLabel* title = new QLabel(scrollWidget);
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("黑体", 13, 87));
    title->setText(data[0]["title"]);
    scrollVLayout->addWidget(title);

    /*作者姓名*/
    LabelMouseEvent* author = new LabelMouseEvent(scrollWidget);
    author->setAlignment(Qt::AlignHCenter);
    author->setIndent(150);
    author->setText(data[0]["author_nickname"]);
    author->setLink(data[0]["author_url"]);
    scrollVLayout->addWidget(author);

    int size = data.size();
    for(int i = 1; i < size; ++i){

        if( data[i]["article"] != NULL ){
            /*文字显示*/
            QTextEdit* text = new QTextEdit(scrollWidget);
            text->setReadOnly(true);
            text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            text->setContentsMargins(5,5,5,5);
            text->setAlignment(Qt::AlignVCenter);
            text->setFont(QFont("黑体", 10, QFont::ExtraLight));
            text->setText(data[i]["article"]);
            text->document()->adjustSize();     //调整QTextEdit高度
            text->setFixedHeight( text->document()->size().rheight() ); //调整QTextEdit高度
            scrollVLayout->addWidget(text);


        }else if(data[i]["image"] != NULL){
            /*图片显示*/
            QLabel* img = new QLabel(scrollWidget);
            img->setAlignment(Qt::AlignHCenter);
            scrollVLayout->addWidget(img);
            int width = 0;
            int height = 0;

            width = data[i]["width"].toInt();
            height = data[i]["height"].toInt();

            double areaWidth = this->width() - 400;

            //根据窗口大小,缩放图片
            if(width > areaWidth){
                double r = width/areaWidth;
                width = width/r;
                height = height/r;
            }

            /*如果图片有名称，则设置图片名称*/
            if( data[i]["image_name"] != NULL ){
                QLabel* imageName = new QLabel(scrollWidget);
                imageName->setAlignment(Qt::AlignHCenter);
                imageName->setText(data[i]["image_name"]);
                imageName->setStyleSheet("color:#777777;"
                                         "font: 9pt \"黑体\";");

                scrollVLayout->addWidget(imageName);
                scrollVLayout->setAlignment(imageName, Qt::AlignCenter);
            }

            /*通过url发送http请求获取图片*/
            MyImage* myImage = new MyImage(this);
            myImage->moveToThread(GLOBAL::thread);
            myImage->loadImage( data[i]["image"] );
            connect( myImage, &MyImage::sigLoadFinish, this,
                        [=](){
                            if(width != NULL){
                                img->setPixmap(myImage->scaled(width, height) );
                            }else{
                                img->setPixmap( myImage->copy() );
                            }
                            img->setVisible(true);
                            myImage->deleteLater();
                        } );

        }
    }

}
