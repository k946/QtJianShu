#include "widget.h"
#include <QApplication>
#include <QObject>
#include <QFileDialog>
#include "httprequest.h"
#include "global.h"
#include "recommendations/widget_recommendationsauthor.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Main Thread: " << QThread::currentThreadId();


    qRegisterMetaType<std::vector< std::map<QString, QString> >>("std::vector< std::map<QString, QString> >");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::shared_ptr<QByteArray>>("std::shared_ptr<QByteArray>");


    GLOBAL::thread->start();
    QObject::connect( GLOBAL::thread, &QThread::finished,  GLOBAL::thread, &QThread::deleteLater);
    GLOBAL::http->moveToThread(GLOBAL::thread);


    Widget w;
    w.show();

    return a.exec();
}

