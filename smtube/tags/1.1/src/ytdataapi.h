#ifndef YTDATAAPI_H
#define YTDATAAPI_H

#include <QObject>
#include <QDate>


class QNetworkAccessManager;
class QNetworkReply;

struct SingleResult
{
    QString desc;
    QString thumbUrl;
    QString title;
    int duration;
    QDate publishedDate;
    QString videoId;
    int viewCount;
    int favCount;
};

struct YTReply
{
    int totalResultCount;
    int thisResultCount;
    int startIndex;
    int itemsPerPage;
    QString queryString;
    QString nextUrl;
    QList<SingleResult> results;
    int replyPointer;
    QString errorString;

};


class YTDataAPI : public QObject
{
Q_OBJECT
public:

    explicit YTDataAPI(QObject *parent = 0);

    int getMost(int tab, QString searchTerm = QString());
    int getNextResults(QString url);
    void reset();
    static QString unescape(QString html);

private:

    int getXmlResponse(QString url);
    QNetworkAccessManager* manager;
    QByteArray rawReply;
    void parseXmlReply(QByteArray arr, YTReply& formattedReply);

signals:

    void finalResults(const YTReply&);

public slots:
    void gotReply(QNetworkReply* reply);

};

#endif // YTDATAAPI_H
