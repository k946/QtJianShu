#include "workerofshcool.h"

WorkerOfShcool::WorkerOfShcool(QObject *parent) : QObject(parent)
{
    initConnect();
}


void WorkerOfShcool::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
    httpThread.moveToThread(t);
    decodeThread.moveToThread(t);
}

void WorkerOfShcool::work()
{
    QString url = "https://www.jianshu.com/c/e048f1a72e3d?utm_medium=index-banner-s&utm_source=desktop";
    std::map<QString, QString> header;

    header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
    header["Accept"] =  "text/html, */*; q=0.01";
    header["Accept-Language"] =  "zh-CN,zh;q=0.9";


    //header["X-CSRF-Token"] = "1VPEgvarJC2D22wCQmP+E+P1NzdCfUEecpKJGiYvL42DneDZdC77mUXiXGl5gEukQWdYHagyvB/baw805DdAQw==";
    //header["X-INFINITESCROLL"] = "true";
    //header["If-None-Match"] = "W\/\"16c7d94c4846279b581ba34806b075e0\"";
    //header["X-Requested-With"] = "XMLHttpRequest";

    emit sigStartHttpRequest(url, "get", "", header);
}

void WorkerOfShcool::initConnect()
{
    //开始HTTP请求
    connect(this, SIGNAL(sigStartHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)),
            &httpThread, SLOT(sltHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)), Qt::QueuedConnection);

    //获取HTTP响应
    connect(&httpThread, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpRequestResult(std::shared_ptr<QByteArray>)) );

    //开始Json解析
    connect(this, SIGNAL(sigStartDecode(std::shared_ptr<QByteArray>)),
            &decodeThread, SLOT(sltDecodeShcool(std::shared_ptr<QByteArray>)));

    //获取Json解析的结果
    connect(&decodeThread, SIGNAL(sigSendResult(std::vector< std::map<QString, QString> >&)),
            this, SLOT(sltGetDecodeResult(std::vector< std::map<QString, QString> >&)));
}


void WorkerOfShcool::sltGetHttpRequestResult(std::shared_ptr<QByteArray> content)
{
    emit sigStartDecode( content );
}

void WorkerOfShcool::sltGetDecodeResult(std::vector<std::map<QString, QString> >& result)
{
    emit sigSendResult(result);
}

