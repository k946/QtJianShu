#ifndef WORKEROFPUBLICATIONSBOOK_H
#define WORKEROFPUBLICATIONSBOOK_H

#include <QObject>
#include <httprequest.h>
#include "decode.h"

class WorkerOfPublicationsBook : public QObject
{
    Q_OBJECT
public:
    explicit WorkerOfPublicationsBook(QObject *parent = nullptr);
    void moveToThread(QThread* t);

private:
    void initConnect();

signals:
    void sigStartHttpRequest(const QString& url, const QString& method,
                        const QString& p , const std::map<QString, QString>& header);

    void sigStartDecode(std::shared_ptr<QByteArray>);                  //开始html解析
    void sigSendResult(std::vector< std::map<QString, QString> >);   //返回解析的结果

public slots:
    void sltGetHttpRequestResult(std::shared_ptr<QByteArray> content);         //获取HTTP请求的结果
    void sltGetDecodeResult(std::vector< std::map<QString, QString> > &); //获取json的解析结果

public:
    void work();

private:
   HttpRequest httpThread;   //用于HTTPS请求
   Decode decodeThread;      //用于解析html
};

#endif // WORKEROFPUBLICATIONSBOOK_H
