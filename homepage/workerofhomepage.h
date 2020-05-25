#ifndef WORKEROFHOMEPAGE_H
#define WORKEROFHOMEPAGE_H

#include <QObject>
#include "decode.h"
#include "httprequest.h"

class WorkerOfHomePage : public QObject
{
    Q_OBJECT
public:
    explicit WorkerOfHomePage(QObject *parent = nullptr);
    ~WorkerOfHomePage();
    void moveToThread(QThread* t);

private:
    void initConnect();

signals:
    void sigStartHttpRequest(QString, QString, QString);             //开始HTTP请求
    void sigStartDecode(std::shared_ptr<QByteArray>);                  //开始html解析
    void sigSendResult(std::vector< std::map<QString, QString> >);   //返回解析的结果

public slots:
    void sltGetHttpRequestResult(std::shared_ptr<QByteArray>);         //获取HTTP请求的结果
    void sltGetDecodeResult(std::vector< std::map<QString, QString> > &); //获取json的解析结果



public:
    void search(const QString& page = "1");

private:
   HttpRequest httpThread;   //用于HTTPS请求
   Decode decodeThread;      //用于解析html
   std::vector< QString > id;       //已接受的热门文章的ID，搜索时需要用到,详情见search()方法
};

#endif // WORKEROFHOMEPAGE_H
