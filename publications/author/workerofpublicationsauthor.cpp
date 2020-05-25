#include "workerofpublicationsauthor.h"

WorkerOfPublicationsAuthor::WorkerOfPublicationsAuthor(QObject *parent) : QObject(parent)
{
    initConnect();
}

void WorkerOfPublicationsAuthor::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
    httpThread.moveToThread(t);   //用于HTTPS请求
    decodeThread.moveToThread(t);      //用于解析html
}

void WorkerOfPublicationsAuthor::initConnect()
{
    //开始HTTP请求
    connect(this, SIGNAL(sigStartHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)),
            &httpThread, SLOT(sltHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)), Qt::QueuedConnection);

    //获取HTTP响应
    connect(&httpThread, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpRequestResult(std::shared_ptr<QByteArray>)) );

    //开始Json解析
    connect(this, SIGNAL(sigStartDecode(std::shared_ptr<QByteArray>)),
            &decodeThread, SLOT(sltDecodePublcationsAuthor(std::shared_ptr<QByteArray>)));

    //获取Json解析的结果
    connect(&decodeThread, SIGNAL(sigSendResult(std::vector< std::map<QString, QString> >&)),
            this, SLOT(sltGetDecodeResult(std::vector< std::map<QString, QString> >&)));
}

void WorkerOfPublicationsAuthor::sltGetHttpRequestResult(std::shared_ptr<QByteArray> content)
{
    emit sigStartDecode(content);
}

void WorkerOfPublicationsAuthor::sltGetDecodeResult(std::vector<std::map<QString, QString> > & result)
{
    emit sigSendResult(result);
}

void WorkerOfPublicationsAuthor::work(int page)
{
    std::map<QString, QString> header;

    header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
    header["Accept"] =  "application/json";
    header["Accept-Language"] =  "zh-CN,zh;q=0.9";

    emit sigStartHttpRequest("https://www.jianshu.com/publication_recommended_users?",
                             "get", "page="+QString::number(page)+"&count=4" ,
                             header);

}
