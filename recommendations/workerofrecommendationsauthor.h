#ifndef WORKEROFRECOMMENDATIONSAUTHOR_H
#define WORKEROFRECOMMENDATIONSAUTHOR_H

#include <QObject>
#include "decode.h"
#include "httprequest.h"

class WorkerOfRecommendationsAuthor : public QObject
{
    Q_OBJECT
public:
    explicit WorkerOfRecommendationsAuthor(QObject *parent = nullptr);
    void work(int page);
    void moveToThread(QThread* t);

private:
    void initConnect();

signals:
    void sigStartHttpRequest(const QString&, const QString&, const QString&, const std::map<QString, QString>&);//开始HTTP请求
    void sigStartDecode(std::shared_ptr<QByteArray>);                              //开始html解析
    void sigSendResult(std::vector< std::map<QString, QString> >);   //返回解析的结果

public slots:
    void sltGetHttpRequestResult(std::shared_ptr<QByteArray> content);         //获取HTTP请求的结果
    void sltGetDecodeResult(std::vector< std::map<QString, QString> >& ); //获取json的解析结果

private:
    HttpRequest httpThread;   //用于HTTPS请求
    Decode decodeThread;      //用于解析html

};

#endif // WORKEROFRECOMMENDATIONSAUTHOR_H
