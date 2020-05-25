#include "myimage.h"
#include "global.h"
#include <QPainter>
#include <QBitmap>

MyImage::MyImage(const QUrl & url, QObject *parent) :
    QObject(parent)
{

    initConnect();
    emit sigHttpRequest(url.toString());

}

MyImage::MyImage(QObject *parent) : QObject(parent)
{
    initConnect();
}


MyImage::~MyImage()
{

}

void MyImage::initConnect()
{
    connect(this, SIGNAL(sigHttpRequest(const QString&)),
            &t, SLOT(sltHttpRequest(const QString&)), Qt::QueuedConnection);

    connect(&t, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpResult(std::shared_ptr<QByteArray>)), Qt::QueuedConnection);

}


void MyImage::loadImage(const QUrl & url)
{
    emit sigHttpRequest(url.toString());
}

void MyImage::moveToThread(QThread *t)
{
    this->t.moveToThread(t);
}

QPixmap MyImage::radius(int width, int height, int r)
{

    QBitmap mask(width, height);
    QPainter painter(&mask);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, r*2, r*2, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, r*2, r*2, 99, 99);

    QPixmap image = this->scaled(width, height);
    image.setMask(mask);

    return image;
}


void MyImage::sltGetHttpResult(std::shared_ptr<QByteArray> data)
{
    loadFromData(*data);
    emit sigLoadFinish();
}
