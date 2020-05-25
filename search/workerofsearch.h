#ifndef WORKERSEARCH_H
#define WORKERSEARCH_H

#include <QObject>
#include "httprequest.h"
#include "decode.h"

class WorkerOfSearch : public QObject
{
    Q_OBJECT
public:
    explicit WorkerOfSearch(QObject *parent = nullptr);
    void moveToThread(QThread* t);
    //搜索参数：搜索的内容，页码，搜索类型，排序方式，发布时间
    void work(const QString& p,const QString& page = "1",
                const QString& type = "note", const QString& order_by = "default", const QString& time_range = "");

signals:
    void sigStartHttpRequest( QString url, QString method, QString p );             //开始HTTP请求
    void sigStartDecode( std::shared_ptr<QByteArray> );                                 //开始Json解析
    void sigSendResult(std::vector< std::map<QString, QString> >);      //返回最终解析的结果

public slots:
    void sltGetHttpRequestResult(std::shared_ptr<QByteArray> content);         //获取HTTP请求的结果
    void sltGetDecodeResult(std::vector< std::map<QString, QString> >& ); //获取json的解析结果

private:
   HttpRequest httpThread;
   Decode decodeThread;
   unsigned int page;


};

#endif // WORKERSEARCH_H
