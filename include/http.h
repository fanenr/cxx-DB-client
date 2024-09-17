#ifndef HTTP_H
#define HTTP_H

#include <optional>

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

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
  std::optional<QByteArray>
  get (QNetworkRequest const &req)
  {
    auto reply = nam.get (req);

    loop.exec ();

    if (reply->error ())
      return std::nullopt;
    return reply->readAll ();
  }

  std::optional<QByteArray>
  post (QNetworkRequest const &req, QByteArray const &data)
  {
    auto reply = nam.post (req, data);

    loop.exec ();

    if (reply->error ())
      return std::nullopt;
    return reply->readAll ();
  }

  std::optional<QByteArray>
  post (QNetworkRequest const &req, QMap<QString, QString> const &map)
  {
    auto str = QString ();

    for (auto it = map.cbegin (); it != map.cend ();)
      {
        str += it.key ();
        str.append ("=");
        str += it.value ();

        if (++it != map.cend ())
          str.append ("&");
      }

    return post (req, str.toUtf8 ());
  }
};

#endif
