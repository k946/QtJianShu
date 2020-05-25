#ifndef DECODEJSONTHREAD_H
#define DECODEJSONTHREAD_H

#include <QObject>
#include <QThread>
#include <map>
#include <vector>
#include <string>
#include <jsoncpp/dist/json/json.h>
#include <thread>
#include <QDomComment>
#include <queue>
#include <htmlcxx/html/ParserDom.h>

typedef std::map<std::string,std::string> MAP_S2S;


class Decode : public QThread
{
    Q_OBJECT
public:
    Decode();
    virtual void run();

signals:
    void sigSendResult(std::vector< std::map<QString, QString> >&);

public slots:
    void sltDecodeSearchNote(std::shared_ptr<QByteArray>);
    void sltDecodeHomePage(std::shared_ptr<QByteArray>);
    void sltDecodeBooks(std::shared_ptr<QByteArray>);
    void sltDecodePublcationsAuthor(std::shared_ptr<QByteArray>);
    void sltDecodePublcationsBook(std::shared_ptr<QByteArray>);
    void sltDecodeAuthorInfo(std::shared_ptr<QByteArray>);
    void sltDecodeArticle(std::shared_ptr<QByteArray>);
    void sltDecodeShcool(std::shared_ptr<QByteArray>);
    void sltDecodeRecommendationsAuthor(std::shared_ptr<QByteArray>);
};

#endif // DECODEJSONTHREAD_H
