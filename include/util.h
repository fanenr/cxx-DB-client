#ifndef UTIL_H
#define UTIL_H

#define URL_BASE "http://127.0.0.1:5000"

#define URL_STUDENT URL_BASE "/student"
#define URL_TEACHER URL_BASE "/teacher"

#define URL_STUDENT_REG URL_STUDENT "/reg"
#define URL_STUDENT_LOG URL_STUDENT "/log"
#define URL_STUDENT_LIST URL_STUDENT "/list"
#define URL_STUDENT_TAKE URL_STUDENT "/take"
#define URL_STUDENT_GRADE URL_STUDENT "/grade"

#define URL_TEACHER_REG URL_TEACHER "/reg"
#define URL_TEACHER_LOG URL_TEACHER "/log"
#define URL_TEACHER_NEW URL_TEACHER "/new"
#define URL_TEACHER_LIST URL_TEACHER "/list"
#define URL_TEACHER_GRADE URL_TEACHER "/grade"

enum class Type
{
  STUDENT,
  TEACHER,
};

#include <QMessageBox>
#include <QtNetwork/QNetworkReply>

namespace util
{

inline bool
check_reply (QNetworkReply *reply)
{
  if (reply->error () == QNetworkReply::NoError)
    return true;

  auto sts = reply->attribute (QNetworkRequest::HttpStatusCodeAttribute);
  if (!sts.isValid ())
    {
      QMessageBox::warning (nullptr, QObject::tr ("错误"),
                            QObject::tr ("网络错误：无法发送网络请求"));
      return false;
    }

  auto msg = QString::fromUtf8 (reply->readAll ());
  auto body = QString ("%1（%2）：%3")
                  .arg (QObject::tr ("响应错误"))
                  .arg (sts.toInt ())
                  .arg (msg);

  QMessageBox::warning (nullptr, QObject::tr ("错误"), body);
  return false;
}

};

#endif
