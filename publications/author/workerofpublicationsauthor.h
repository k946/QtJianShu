#ifndef WORKEROFPUBLICATIONSAUTHOR_H
#define WORKEROFPUBLICATIONSAUTHOR_H

#include <QObject>
#include "httprequest.h"
#include "decode.h"

class WorkerOfPublicationsAuthor : public QObject
{
    Q_OBJECT
public:
    explicit WorkerOfPublicationsAuthor(QObject *parent = nullptr);
    void moveToThread(QThread* t);

private:
    void initConnect();

signals:
    void sigStartHttpRequest(const QString& url, const QString& method,
                        const QString& p , const std::map<QString, QString>& header);
    void sigStartDecode(std::shared_ptr<QByteArray>);                  //解析
    void sigSendResult(std::vector< std::map<QString, QString> >);   //返回解析的结果

public slots:
    void sltGetHttpRequestResult(std::shared_ptr<QByteArray> content);         //获取HTTP请求的结果
    void sltGetDecodeResult(std::vector< std::map<QString, QString> > &); //获取json的解析结果

public:
    void work(int page = 1);

private:
   HttpRequest httpThread;   //用于HTTPS请求
   Decode decodeThread;      //用于解析html
};

#endif // WORKEROFPUBLICATIONSAUTHOR_H
