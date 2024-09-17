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
    return Type::TEACHER;
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
  auto req_url
      = QString (type == Type::STUDENT ? URL_STUDENT_LOG : URL_TEACHER_LOG);

  auto req_data = QMap<QString, QString> ();
  req_data["username"] = user;
  req_data["password"] = pass;

  auto http = Http ();
  auto req = Http::make_req (req_url);
  auto res = http.post (req, req_data);

  if (!res.has_value ())
    return (void)QMessageBox::warning (this, tr ("错误"),
                                       tr ("无法发送网络请求"));

  auto obj = QJsonDocument::fromJson (res.value ()).object ();

  auto info = Home::Info{
    .user = std::move (user),
    .name = obj["name"].toString (),
    .token = obj["token"].toString (),
  };

  auto home = new Home (type, std::move (info));
  home->show ();
  close ();
}
