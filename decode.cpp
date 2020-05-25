#include "decode.h"

#include <QFile>
#include <QDebug>

Decode::Decode()
{

}

void Decode::run()
{
    exec();
}

//优选连载解析
void Decode::sltDecodeBooks(std::shared_ptr<QByteArray> content)
{
    Json::Reader reader;
    Json::Value root;
    std::vector< std::map<QString, QString> > entries;

    if (reader.parse(content->toStdString(), root))
    {

            // 获取json数组内容
            if (root["books"].isArray())
            {

                int size = root["books"].size();
                std::map<QString, QString> entry;
                //QString clear[] = {"<em class='search-result-highlight'>", "</em>"};  //要清除的多余内容

                for (int i = 0; i < size; i++)
                {

                    entry["name"] = root["books"][i]["name"].asString().data();      //标题
                    entry["image_url"] = root["books"][i]["image_url"].asString().data();
                    entry["book_url"] = ("https://www.jianshu.com/nb/" + root["books"][i]["notebook_id"].asString()).data();
                    entry["views_count"] = QString::number( root["books"][i]["views_count"].asInt() );

                    entry["user_nickname"] = root["books"][i]["user"]["nickname"].asString().data();
                    entry["user_url"] = ("http://www.jianshu.com/u/" + root["books"][i]["user"]["slug"].asString()).data();
                    entry["user_avatar_url"] = root["books"][i]["user"]["avatar"].asString().data();

                    entries.push_back(entry);
                    entry.clear();
                }

                emit sigSendResult(entries);

            }
    }
}

//优选版权-推荐作者
void Decode::sltDecodePublcationsAuthor(std::shared_ptr<QByteArray> content)
{

    Json::Reader reader;
    Json::Value root;
    std::vector< std::map<QString, QString> > entries;

    if (reader.parse(content->toStdString(), root))
    {

            // 获取json数组内容
            if (root.isArray())
            {

                int size = root.size();
                std::map<QString, QString> entry;

                for (int i = 0; i < size; i++)
                {
                    entry["key_work_title"] = root[i]["key_work_title"].asString().data();
                    entry["key_work_url"] = root[i]["key_work_url"].asString().data();

                    entry["user_nickname"] = root[i]["user"]["nickname"].asString().data();      //标题
                    entry["user_avatar_url"] = root[i]["user"]["avatar_source"].asString().data();
                    entry["user_intro"] = root[i]["user"]["intro"].asString().data();

                    if( root[i]["user"]["notes"].isArray() ){
                        for(int j = 0; j < root[i]["user"]["notes"].size(); ++j){
                            entry["user_notes_" + QString::number(j) + "_note_url"] =
                                   root[i]["user"]["notes"][j]["note_url"].asString().data();

                            entry["user_notes_" + QString::number(j) + "_public_title"] =
                                   root[i]["user"]["notes"][j]["public_title"].asString().data();
                        }
                    }

                    entry["user_url"] = root[i]["user"]["url"].asString().data();

                    entries.push_back(entry);
                    entry.clear();
                }

                emit sigSendResult(entries);
            }
    }

}

//优选版权-图书
void Decode::sltDecodePublcationsBook(std::shared_ptr<QByteArray> content)
{
    std::vector< std::map<QString, QString> > result;   //最终结果
    std::map<QString, QString> m;


    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(content->toStdString());
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();

    //进入书籍域
    for(;it != end; ++it){
        if( it->tagName().compare("div") == 0 ){   //匹配div标签
             it->parseAttributes();

             //如果div的class属性为category 且 存在id属性则已表示进入书籍域
             if(it->attribute("class").second.compare("category") != 0 || !it->attribute("id").first )
                  continue;

             QString category = it->attribute("id").second.data();   //获取书籍的类型

             //进入书籍
             for(int i = 0;it != end; ++it){
                 if( it->tagName().compare("a") == 0 ){
                      it->parseAttributes();
                      if(it->attribute("class").second.compare("book") != 0)    //如果a标签的class属性为book则表示a标签的内容为书籍信息
                           continue;

                      QString bookNo = QString::number(i++); //图书序号

                      //书籍购买url
                      m[category + "_" + bookNo + "_url"] =
                              it->attribute("href").second.data();

                      it += 4;

                      for(;it != end; ++it){
                           //书籍封面url
                           if( it->tagName().compare("img") == 0 ){
                               it->parseAttributes();
                               m[category + "_" + bookNo + "_cover"] =
                                       it->attribute("src").second.data();
                               ++it;
                           }
                           it->parseAttributes();

                           //书名、作者、简介、价格
                           if( it->tagName().compare("div") == 0 ){
                               it->parseAttributes();
                               QString attr =  it->attribute("class").second.data();

                               //当匹配到价格时，说明该本书籍已经匹配结束，跳出循环匹配另一本书籍
                               if( "price" == attr ){
                                   m[category + "_" + bookNo + "_price" ] = (++it)->text().data();  //获取文本
                                   result.push_back(m);
                                   m.clear();
                                   break;
                               }else if( "wechat-tag" == attr){
                                   continue;
                               }

                               m[category + "_" + bookNo + "_" + attr ] = (++it)->text().data();  //获取文本
                           }
                      }

                      it += 4;
                      it->parseAttributes();
                      //判断是否进入下一类书籍
                      if( it->attribute("class").second.compare("category") == 0 ){
                           category = it->attribute("id").second.data(); //修改书籍类型
                           i = 0;   //重置图书编号
                      }else{
                          it -= 4;
                      }

                 }
             }
             break;
        }
    }

    emit sigSendResult(result);
}

//个人主页
void Decode::sltDecodeAuthorInfo(std::shared_ptr<QByteArray> content)
{

    std::vector< std::map<QString, QString> > result;   //最终结果
    std::map<QString, QString> m;


    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(content->toStdString());
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();

    for(; it != end; ++it){
        if(it->tagName().compare("a") == 0){
            it->parseAttributes();
            /*如果a标签的class属性为avatar，则将要匹配作者的一些信息*/
            if(it->attribute("class").second.compare("avatar") != 0)
                 continue;

            /*用户头像url*/
            it += 2;
            it->parseAttributes();
            m["avatar_url"] = it->attribute("src").second.data();

            /*作者名字*/
            it += 11;
            m["name"] = it->text().data();

            /*匹配作者详情：关注的人的数量、粉丝量、文章量、字数、收获喜欢数、总资产*/
            {
            QString att[] = {"following","follower", "article", "words", "likes",
                             "property",  "article_url", "follower_url", "following_url"};
            for(int i = 0; it != end && i < 6; ++it){
                if(it->tagName().compare("div") == 0){
                    it->parseAttributes();
                    if(it->attribute("class").second.compare("meta-block") != 0)
                        continue;
                    it += 2;
                    if( it->tagName().compare("a") == 0){
                        it->parseAttributes();
                         m[ att[8-i] ] = ("https://www.jianshu.com" + it->attribute("href").second).data();
                    }else{
                        it -= 2;
                    }
                    it += 3;
                    m[ att[i] ] = it->text().data();
                    /*只有6个标签要匹配，因此匹配完6个后就退出当前循环*/
                    ++i;
                }
            }
            }

            /*保存作者信息至返回结果*/
            result.push_back(m);
            m.clear();

            /*
            *属性列表：
            * title : 文章标题
            * content : 大致内容
            * nickname : 作者名字
            * comments : 文章评论数量
            * likes : 文章被喜欢数
            * link : 文章链接
            * userHomePage : 文章作者个人主页链接
            * clicks : 文章点击量
            * money : 被打赏量
            * time : 文章发布时间
            * diamons : 钻石
            * image : 展示图片的url
            */


            /*匹配作者文章列表*/
            for(; it != end; ++it){
                if(it->tagName().compare("li") == 0){
                    it->parseAttributes();
                    /*li标签的data-note-id属性存在,则表示已经进入某篇文章*/
                    if( it->attribute("data-note-id").first ){

                        /*文章的封面图片，注:该元素不一定的存在*/
                        it += 4;
                        if( it->tagName().compare("img") == 0){
                            it->parseAttributes();

                            /*有些图片的url的开头没有 'https:' 因此需要加上'https:' 才能请求成功 */
                            QString t = it->attribute("data-echo").second.data();
                            if( !t.contains("https:") ){
                                t = "https:" + t;
                            }
                            m["image"] = t;

                            it++;
                        }else{
                            it -=4;
                        }


                        it += 4;
                        it->parseAttributes();
                        /*文章标题及文章链接*/
                        m["link"] = ("https://jianshu.com" + it->attribute("href").second).data();
                        m["title"] = (++it)->text().data();

                        /*文章大致内容*/
                        it += 3;
                        m["content"] = it->text().data();

                        /*匹配钻石, 它不一定存在*/
                        it += 6;
                        it->parseAttributes();
                        if(it->attribute("class").second.compare("iconfont ic-paid1") == 0){
                            m["diamons"] = (++it)->text().data();
                            it -= 2;
                        }else{
                            it -= 6;
                        }

                        /*点击量*/
                        it += 7;
                        m["clicks"] = it->text().data();

                        /*评论数量*/
                        it += 5;
                        m["comments"] = it->text().data();

                        /*喜欢数量*/
                        it += 4;
                        m["likes"] = it->text().data();

                        /*匹配打赏, 注：该元素不一定存在*/
                        it += 3;
                        it->parseAttributes();
                        if(it->attribute("class").second.compare("iconfont ic-list-money") == 0){
                            m["money"] = (++it)->text().data();
                        }else{
                            it -= 3;
                        }

                        /*时间*/
                        it += 2;
                        it->parseAttributes();
                        QString time = it->attribute("data-shared-at").second.data();
                        time.replace("T", " ");
                        time.replace(time.indexOf('+'),6, "");
                        m["time"] = time;

                        result.push_back(m);
                        m.clear();
                    }
                }
            }


            break;
        }
    }

    emit sigSendResult(result);

}

void Decode::sltDecodeArticle(std::shared_ptr<QByteArray> content)
{
    /*
    QFile file("C:\\Users\\user\\Desktop\\简书.txt");
    file.open(QIODevice::WriteOnly);
    file.write(content->data());
    file.close();
    */

    std::vector< std::map<QString, QString> > result;
    std::map<QString, QString> m;

    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(content->toStdString());
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    htmlcxx::HTML::Node s;



    for(; it != end; ++it){
        /*匹配直到标签为h1*/
        if(it->tagName().compare("h1") != 0)
            continue;

        it->parseAttributes();
        /*匹配直到h1标签包含title属性*/
        if(it->attribute("title").first == false)
            continue;

        /*标题*/
        m["title"] = (++it)->text().data();
        //qDebug() << m["title"];

        /*作者主页url*/
        it += 2;
        it->parseAttributes();
        m["author_url"] = ("https://www.jianshu.com" + it->attribute("href").second).data();

        /*作者昵称*/
        it += 2;
        m["author_nickname"] = it->text().data();

        result.push_back(m);
        m.clear();

        for(; it != end; ++it){
            if(it->tagName().compare("article") == 0){
                ++it;
                for(; it != end; ++it){

                    /*文章结束标志：div的class属性为_19DgIp*/
                    if(it->tagName().compare("div") == 0){
                        it->parseAttributes();
                        if(it->attribute("class").second.compare("_19DgIp") == 0)
                            break;
                    }

                    /*提取图片*/
                    if(it->tagName().compare("img") == 0){
                        it->parseAttributes();
                        result.push_back(m);
                        m.clear();

                        /*有2种不同的图片，因此根据标签属性的不同，分别进行提取*/
                        if(it->attribute("class").second.compare("uploaded-img") == 0){

                            /*有些图片的url的开头没有 'https:' 因此需要加上'https:' 才能请求成功 */
                            QString t = it->attribute("src").second.data();
                            if( !t.contains("https:") ){
                                t = "https:" + t;
                            }
                            m["image"] = t;
                            //qDebug() << "image" << m["image"];

                        }else{
                            QString t = it->attribute("data-original-src").second.data();
                            if( !t.contains("https:") ){
                                t = "https:" + t;
                            }
                            m["image"] = t;
                            //qDebug() << "image" << m["image"];

                            /*图片宽度*/
                            m["width"] = it->attribute("data-original-width").second.data();
                            qDebug() << "width" << m["width"];

                            /*图片高度*/
                            m["height"] = it->attribute("data-original-height").second.data();
                            //qDebug() << "height" << m["height"];

                            /*图片名字*/
                            it += 4;
                            QString imageName = it->text().data();
                            if( imageName.replace("\n", "").size() > 0 ){
                                m["image_name"] = imageName;
                            }

                            result.push_back(m);
                            m.clear();

                            continue;
                        }
                    }


                    /*提取标签及文本*/
                    if( it->isTag() && it->tagName().compare("div") != 0 && it->tagName().compare("img") != 0){
                        /*标签栈*/
                        std::vector<QString> labels;
                        bool hasText = false;
                        int brNumber = 0;

                        for(;it != end; ++it){
                            /*遇到文本，退出循环*/
                            if( !it->isComment() && !it->isTag() && it->text().compare("\n") ){
                                hasText = true;
                                break;
                            }

                            /*遇到div标签，退出循环*/
                            if( it->tagName().compare("div") == 0)
                                break;

                            /*遇到img标签，退出循环，匹配图片*/
                            if( it->tagName().compare("img") == 0 ){
                                --it;
                                break;
                            }

                            /*遇到br标签，统计br的数量，因为没有</br>标签*/
                            if( it->tagName().compare("br") == 0 ){
                                brNumber++;
                                continue;
                            }

                            labels.push_back( it->tagName().data() );
                        }

                        if(!hasText)
                            continue;

                        for(int i = 0; i < labels.size(); ++i){
                            qDebug() << "<" + labels[i] + ">";
                            m["article"] += "<" + labels[i] + ">";
                        }

                        for(int i = 0; i < brNumber; ++i){
                            m["article"] += "<br>";
                            qDebug() << "<br>";
                        }

                        qDebug()  << it->text().data();
                        m["article"] += it->text().data();

                        for(int i = labels.size() - 1; i >= 0; --i){
                            qDebug() << "</" + labels[i] + ">";
                            m["article"] += "</" + labels[i] + ">";
                        }
                    }
                }
                break;
            }
        }

    }

    result.push_back(m);
    emit sigSendResult(result);

}

void Decode::sltDecodeShcool(std::shared_ptr<QByteArray> content)
{


    std::vector< std::map<QString, QString> > result;   //最终结果
    std::map<QString, QString> m;


    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(content->toStdString());
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    htmlcxx::HTML::Node s;

    for(; it != end; ++it){
        if(it->tagName().compare("div") != 0)
            continue;

        it->parseAttributes();
        /*当div的class属性为main-top，则开始匹配专题信息*/
        if(it->attribute("class").second.compare("main-top") == 0){
            it += 4;
            it->parseAttributes();
            m["image"] = it->attribute("src").second.data();

            it += 11;
            m["title"] = it->text().data();

            it += 4;
            QString info = it->text().data();
            info.replace(" ", "");
            info.replace("\n","");
            m["info"] = info;

            result.push_back(m);
            m.clear();

        }else{
            continue;
        }

        /*
        *属性列表：
        * title : 文章标题
        * content : 大致内容
        * nickname : 作者名字
        * comments : 文章评论数量
        * likes : 文章被喜欢数
        * link : 文章链接
        * userHomePage : 文章作者个人主页链接
        * clicks : 文章点击量
        * money : 被打赏量
        * time : 文章发布时间
        * diamons : 钻石
        * image : 展示图片的url
        * paid : 付费文章
        */

        /*匹配文章列表*/
        for(; it != end; ++it){
            it->parseAttributes();
            if(it->tagName().compare("li") == 0){
                it->parseAttributes();
                /*li标签的data-note-id属性存在,则表示已经进入某篇文章*/
                if( it->attribute("data-note-id").first ){
                    int li_end = it->offset() + it->length();

                    /*文章的封面图片，注:该元素不一定的存在*/
                    it += 4;
                    if( it->tagName().compare("img") == 0){
                        it->parseAttributes();

                        /*有些图片的url的开头没有 'https:' 因此需要加上'https:' 才能请求成功 */
                        QString t = it->attribute("data-echo").second.data();
                        if( !t.contains("https:") ){
                            t = "https:" + t;
                        }
                        m["image"] = t;

                        ++it;
                    }else{
                        it -=4;
                    }

                    it += 4;
                    it->parseAttributes();
                    /*文章标题及文章链接*/
                    m["link"] = ("https://jianshu.com" + it->attribute("href").second).data();
                    m["title"] = (++it)->text().data();
                    /*文章大致内容*/
                    it += 3;
                    m["content"] = it->text().data();

                    for(; it != end; ++it){

                        /*li标签结束，表示当前文章解析完成，退出循环解析下一篇*/
                        if( it->offset() >= li_end ){
                            result.push_back(m);
                            m.clear();
                            --it;   //<li> 和 </li>在同一个节点，因此需要前移
                            break;
                        }

                        it->parseAttributes();
                        QString classValue = it->attribute("class").second.data();

                        if(classValue == "iconfont ic-paid1"){
                            m["diamons"] = (++it)->text().data();
                        }
                        else if(classValue == "nickname"){
                            it->parseAttributes();
                            m["userHomePage"] = ("http://www.jianshu.com" + it->attribute("href").second).data();
                            m["nickname"] = (++it)->text().data();

                        }
                        else if(classValue == "iconfont ic-list-comments"){
                            m["comments"] = (++it)->text().data();
                        }
                        else if(classValue == "iconfont ic-list-like"){
                            m["likes"] = (++it)->text().data();
                        }else if(classValue == "iconfont ic-list-money"){
                            m["money"] = (++it)->text().data();
                        }else if( classValue == "iconfont ic-paid" ){
                            QString paid = (++it)->text().data();
                            paid.replace(" ", "");
                            paid.replace("\n","");
                            m["paid"] = paid;
                        }

                    }
                }
            }
        }
        break;
    }

    emit sigSendResult(result);
}


//推荐作者模块
void Decode::sltDecodeRecommendationsAuthor(std::shared_ptr<QByteArray> content)
{
    QFile file("C:\\Users\\user\\Desktop\\简书.txt");
    file.open(QIODevice::WriteOnly);
    file.write(content->data());
    file.close();


    std::vector< std::map<QString, QString> > result;   //结果
    std::map<QString, QString> m;

    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(content->toStdString());
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();



    for(;it != end; ++it){
        if(it->tagName().compare("div") != 0)
            continue;

        it->parseAttributes();
        /*div的class属性为col-xs-8 则开始匹配作者信息*/
        if( it->attribute("class").second.compare("col-xs-8") == 0 ){

            /*
            ** 键值
            ** user_url : 用户主页
            ** user_nickname : 用户名
            ** user_intro : 用户简介
            ** user_notes_x_article_title : 最近文章标题，倒数第x篇
            ** user_notes_x_article_url : 最近文章链接，倒数第x篇
            ** user_avatar_url : 用户头像Url
            ** user_sex : 性别
            */

            int divEnd = it->offset() + it->length();
            int recentUpdate = 0;

            for(; it != end; ++it){

                if(it->offset() >= divEnd){
                    result.push_back(m);
                    m.clear();
                    break;
                }

                if(it->isTag()){
                    it->parseAttributes();

                    /*class属性不存在，target属性的值为_blank，表示匹配到用户主页*/
                    if( it->attribute("class").first == false && it->attribute("target").second.compare("_blank") == 0 ){
                        m["user_url"] = ( "https://www.jianshu.com" + it->attribute("href").second ).data();
                    }
                    else if(it->attribute("class").second.compare("avatar") == 0 ){
                        m["user_avatar_url"] = it->attribute("src").second.data();
                    }
                    else if(it->attribute("class").second.compare("name") == 0 ){
                        QString t =  (++it)->text().data();
                        t.replace("\n", "");
                        t.replace(" ", "");
                        m["user_nickname"] = t;
                    }
                    else if(it->attribute("class").second.compare("iconfont ic-man") == 0 ){
                        m["user_sex"] = "man";
                    }
                    else if(it->attribute("class").second.compare("iconfont ic-woman") == 0 ){
                        m["user_sex"] = "woman";
                    }
                    else if(it->attribute("class").second.compare("description") == 0 ){
                        m["user_intro"] = (++it)->text().data();
                    }
                    if( it->attribute("class").first == true && it->attribute("target").second.compare("_blank") == 0 ){
                        m["user_notes_"+ QString::number(recentUpdate) +"_article_url"] =  ( "https://www.jianshu.com" + it->attribute("href").second ).data();
                        m["user_notes_"+ QString::number(recentUpdate) +"_article_title"] =  (++it)->text().data();
                        ++recentUpdate;
                    }

                }

            }


        }

    }

    emit sigSendResult(result);

}

//搜索-文章
void Decode::sltDecodeSearchNote(std::shared_ptr<QByteArray> content)
{


    Json::Reader reader;
    Json::Value root;
    std::vector< std::map<QString, QString> > entries;

    if (reader.parse(content->toStdString(), root))
    {

            // 获取json数组内容
            if (root["entries"].isArray())
            {
                int size = root["entries"].size();
                std::map<QString, QString> entry;
                QString clear[] = {"<em class='search-result-highlight'>", "</em>"};  //要清除的多余内容

                for (int i = 0; i < size; i++)
                {

                    entry["title"] = root["entries"][i]["title"].asString().data();            //标题
                    entry["title"].replace(clear[0], "");
                    entry["title"].replace(clear[1], "");

                    entry["content"] = root["entries"][i]["content"].asString().data();        //大致内容
                    entry["content"].replace(clear[0], "");
                    entry["content"].replace(clear[1], "");


                    entry["time"] = root["entries"][i]["first_shared_at"].asString().data();   //发布时间
                    entry["time"].replace("T", "  ");
                    entry["time"].replace(".000Z", "");

                    entry["likes"] = QString::number(root["entries"][i]["likes_count"].asInt());         //喜欢数（点赞数）
                    entry["clicks"] = QString::number(root["entries"][i]["views_count"].asInt());        //点击量
                    entry["comments"] = QString::number(root["entries"][i]["public_comments_count"].asInt());    //评论数

                    entry["link"] = ("https://www.jianshu.com/p/" + root["entries"][i]["slug"].asString()).data();    //文章的url


                    entry["avatar"] = root["entries"][i]["user"]["avatar_url"].asString().data();  //用户头像的url
                    entry["nickname"] = root["entries"][i]["user"]["nickname"].asString().data();  //用户昵称
                    entry["nickname"].replace(clear[0], "");
                    entry["nickname"].replace(clear[1], "");

                    entry["userHomePage"] = ("http://www.jianshu.com/u/" + root["entries"][i]["user"]["slug"].asString()).data(); //用户主页的url
                    entries.push_back(entry);
                }

                emit sigSendResult(entries);
                entry.clear();
            }
    }

}

//主页热门文章
void Decode::sltDecodeHomePage(std::shared_ptr<QByteArray> content)
{
    std::vector< std::map<QString, QString> > result;   //结果
    std::map<QString, QString> m;
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(content->toStdString());
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();



    for(;it != end; ++it){
        if( it->tagName().compare("li") == 0 ){//匹配li标签
             it->parseAttributes();

             if(it->attribute("data-note-id").second.compare("") == 0)
                 continue;      //如果li标签的data-note-id属性不存在，则继续匹配下一个li标签
             m["id"] = it->attribute("data-note-id").second.data();
             int li_end = it->offset() + it->length();

             it += 4;
             if( it->tagName().compare("img") == 0 ){ //匹配图片
                  it->parseAttributes();
                  m["image"] = it->attribute("src").second.data();
             }else{
                it -= 4;
             }

             for(;it != end; ++it){
                 if( it->tagName().compare("a") == 0 ){//匹配li里的a标签:url与标题
                      it->parseAttributes();
                      if(it->attribute("class").second.compare("title") != 0)
                           continue;    //如果不包含class不为title，则执行
                      m["link"] = ("http://www.jianshu.com" + it->attribute("href").second).data();
                      m["title"] = (++it)->text().data();
                      break;
                 }
             }

             for(;it != end; ++it){
                 if( it->tagName().compare("p") == 0 ){//匹配li里的p标签：大致的内容
                      it->parseAttributes();
                      if(it->attribute("class").second.compare("abstract") != 0)
                           continue;
                      m["content"] = (++it)->text().data();
                      break;
                 }
             }

             for(; it != end; ++it){

                 /*li标签结束，表示当前文章解析完成，退出循环解析下一篇*/
                 if( it->offset() >= li_end ){
                     result.push_back(m);
                     m.clear();
                     --it;   //<li> 和 </li>在同一个节点，因此需要前移
                     break;
                 }

                 it->parseAttributes();
                 QString classValue = it->attribute("class").second.data();

                 if(classValue == "iconfont ic-paid1"){
                     m["diamons"] = (++it)->text().data();
                 }
                 else if(classValue == "nickname"){
                     it->parseAttributes();
                     m["userHomePage"] = ("http://www.jianshu.com" + it->attribute("href").second).data();
                     m["nickname"] = (++it)->text().data();

                 }
                 else if(classValue == "iconfont ic-list-comments"){
                     m["comments"] = (++it)->text().data();
                 }
                 else if(classValue == "iconfont ic-list-like"){
                     m["likes"] = (++it)->text().data();
                 }else if(classValue == "iconfont ic-list-money"){
                     m["money"] = (++it)->text().data();
                 }else if( classValue == "iconfont ic-paid" ){
                     QString paid = (++it)->text().data();
                     paid.replace(" ", "");
                     paid.replace("\n","");
                     m["paid"] = paid;
                 }

             }
        }
    }

    emit sigSendResult(result);
}
