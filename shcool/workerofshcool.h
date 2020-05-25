#ifndef WORKEROFSHCOOL_H
#define WORKEROFSHCOOL_H

#include <QObject>
#include "httprequest.h"
#include "decode.h"

class WorkerOfShcool : public QObject
{
    Q_OBJECT
public:
    explicit WorkerOfShcool(QObject *parent = nullptr);
    void moveToThread(QThread* t);
    void work();

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

#endif // WORKEROFSHCOOL_H
