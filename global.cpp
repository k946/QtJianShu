#include "global.h"


QThread* GLOBAL::thread = new QThread();
TitleBar* GLOBAL::titleBar = 0;
QNetworkAccessManager* GLOBAL::http = new QNetworkAccessManager();
