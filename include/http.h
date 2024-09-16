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
  std::optional<QByteArray>
  post (QString const &url, QByteArray const &data)
  {
    auto req = QNetworkRequest (url);
    auto reply = nam.post (req, data);

    loop.exec ();

    if (reply->error ())
      return std::nullopt;
    return reply->readAll ();
  }

  std::optional<QByteArray>
  post (QString const &url, QMap<QString, QString> const &map)
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

    return post (url, str.toUtf8 ());
  }
};

#endif
