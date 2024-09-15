#include "log.h"
#include "home.h"
#include "http.h"
#include "reg.h"

Type
Log::category ()
{
  if (ui.rbtn1->isChecked ())
    return Type::STUDENT;
  if (ui.rbtn2->isChecked ())
    return type::TEACHER;
  abort ();
}

void
Log::on_pbtn1_clicked ()
{
  auto user = ui.ledit1->text ();
  auto pass = ui.ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    return (void)QMessageBox::warning (this, tr ("提示"),
                                       tr ("请输入帐号密码"));

  auto reg = Reg (this);
  reg.exec ();
}

void
Log::on_pbtn2_clicked ()
{
  auto user = ui.ledit1->text ();
  auto pass = ui.ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    return (void)QMessageBox::warning (this, tr ("提示"),
                                       tr ("请输入帐号密码"));

  auto type = category ();
  auto req_url = QString ();

  switch (type)
    {
    case Type::STUDENT:
      req_url = URL_STUDENT_LOG;
      break;

    case Type::TEACHER:
      req_url = URL_TEACHER_LOG;
      break;
    }

  auto req_data = QMap<QStringView, QStringView> ();
  req_data["username"] = user;
  req_data["password"] = pass;

  auto http = Http ();
  auto res = http.post (req_url, req_data);

  if (!res.has_value ())
    return (void)QMessageBox::warning (this, tr ("错误"),
                                       tr ("无法发送网络请求"));

  auto object = QJsonDocument::fromJson (res.value ());

  auto info = Home::Info{
    .name = object["name"].toString (),
    .token = object["token"].toString (),
  };

  auto home = new Home (type, std::move (info));
  home->show ();
  close ();
}
