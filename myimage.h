#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <QObject>
#include <QFile>
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "../../jianshu/jianshu/httprequest.h"

class MyImage : public QObject, public QPixmap
{
    Q_OBJECT
public:
    MyImage(const QUrl&, QObject* parent = 0);
    MyImage(QObject* parent = 0);
    ~MyImage();
    void loadImage(const QUrl&);
    void moveToThread(QThread* t);
    QPixmap radius(int width, int height, int r);

private:
    void initConnect();

signals:
    void sigHttpRequest(const QString& url);
    void sigLoadFinish();

private slots:
    void sltGetHttpResult(std::shared_ptr<QByteArray>);


public:
   HttpRequest t;
};

#endif // MYIMAGE_H
