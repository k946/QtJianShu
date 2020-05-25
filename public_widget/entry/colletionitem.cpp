#include "collectionitem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "global.h"

CollectionItem::CollectionItem(QWidget *parent, double sizeRatio) :
    Widget_Entry(parent),
    sizeRatio(sizeRatio)
{
    initWindow();
    initVariable();
}

void CollectionItem::update(std::map<QString, QString> & m)
{
    title->setText(m["title"]);

    info->setText(m["info"]);

    myImage.loadImage( m["image"] );
    connect( &myImage, &MyImage::sigLoadFinish, this,
                [=](){
                    img->setPixmap( myImage.scaled(img->width(),img->height()) );
                } );
}

void CollectionItem::initWindow()
{
    QHBoxLayout* mainHLayout = new QHBoxLayout(this);
    mainHLayout->setMargin(0);
    mainHLayout->setSpacing(15);

    img = new QLabel(this);
    img->setFixedSize(180 * sizeRatio ,180 * sizeRatio);
    img->setStyleSheet("background:red;");
    mainHLayout->addSpacing(10 * sizeRatio);
    mainHLayout->addWidget(img);

    QVBoxLayout* rightVLayout = new QVBoxLayout;
    rightVLayout->setMargin(0);

    title = new QLabel(this);
    title->setFont( QFont("黑体",13 * sizeRatio,87) );
    rightVLayout->addWidget(title);

    info = new QLabel(this);
    info->setStyleSheet("color:#646464;");
    rightVLayout->addWidget(info);
    mainHLayout->addLayout(rightVLayout);
}

void CollectionItem::initVariable()
{
    myImage.moveToThread(GLOBAL::thread);
}
