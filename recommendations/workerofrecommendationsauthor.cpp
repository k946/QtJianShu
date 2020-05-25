#include "workerofrecommendationsauthor.h"

WorkerOfRecommendationsAuthor::WorkerOfRecommendationsAuthor(QObject *parent) : QObject(parent)
{
    initConnect();

}

void WorkerOfRecommendationsAuthor::work(int page)
{

    QString url = "https://www.jianshu.com/recommendations/users?";
    QString p = "page=" + QString::number(page);
    std::map<QString, QString> header;

    header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
    header["Accept"] =  "text/html, */*; q=0.01";
    header["Accept-Language"] =  "zh-CN,zh;q=0.9";

    emit sigStartHttpRequest(url, "get", p, header);
}

void WorkerOfRecommendationsAuthor::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
    httpThread.moveToThread(t);
    decodeThread.moveToThread(t);
}

void WorkerOfRecommendationsAuthor::initConnect()
{
    //开始HTTP请求
    connect(this, SIGNAL(sigStartHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)),
            &httpThread, SLOT(sltHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)), Qt::QueuedConnection);

    //获取HTTP响应
    connect(&httpThread, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpRequestResult(std::shared_ptr<QByteArray>)) );

    //开始Json解析
    connect(this, SIGNAL(sigStartDecode(std::shared_ptr<QByteArray>)),
            &decodeThread, SLOT(sltDecodeRecommendationsAuthor(std::shared_ptr<QByteArray>)));

    //获取Json解析的结果
    connect(&decodeThread, SIGNAL(sigSendResult(std::vector< std::map<QString, QString> >&)),
            this, SLOT(sltGetDecodeResult(std::vector< std::map<QString, QString> >&)));
}


void WorkerOfRecommendationsAuthor::sltGetHttpRequestResult(std::shared_ptr<QByteArray> content)
{
    emit sigStartDecode( content );
}

void WorkerOfRecommendationsAuthor::sltGetDecodeResult(std::vector<std::map<QString, QString> >& result)
{
    emit sigSendResult(result);
}
