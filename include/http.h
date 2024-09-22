#ifndef HTTP_H
#define HTTP_H

#include <QEventLoop>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class Http : QObject
{
  Q_OBJECT

private:
  QEventLoop loop;
  QNetworkAccessManager nam;

public:
  Http (QObject *parent = nullptr) : QObject (parent)
  {
    connect (&nam, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
  }

public:
  static QNetworkRequest
  make_req (QString const &url)
  {
    return QNetworkRequest (url);
  }

  static QNetworkRequest
  make_req (QString const &url, QString const &jwt)
  {
    auto req = QNetworkRequest (url);
    auto token = QString ("Bearer ") + jwt;
    req.setRawHeader ("Authorization", token.toUtf8 ());
    return req;
  }

public:
  QNetworkReply *
  get (QNetworkRequest const &req)
  {
    auto reply = nam.get (req);
    loop.exec ();
    return reply;
  }

  QNetworkReply *
  post (QNetworkRequest const &req, QByteArray const &data)
  {
    auto reply = nam.post (req, data);
    loop.exec ();
    return reply;
  }

  QNetworkReply *
  post (QNetworkRequest const &req, QMap<QString, QVariant> const &map)
  {
    auto str = QString ();

    for (auto it = map.cbegin (); it != map.cend ();)
      {
        str.append (it.key ()).append ("=");
        auto value = it.value ().toString ();
        str += QUrl::toPercentEncoding (value);

        if (++it != map.cend ())
          str.append ("&");
      }

    return post (req, str.toUtf8 ());
  }
};

#endif
