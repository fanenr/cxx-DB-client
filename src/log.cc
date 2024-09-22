#include "log.h"
#include "home.h"
#include "http.h"

#include <QJsonDocument>
#include <QJsonObject>

Type
Log::category ()
{
  if (ui.rbtn1->isChecked ())
    return Type::STUDENT;
  if (ui.rbtn2->isChecked ())
    return Type::TEACHER;
  abort ();
}

#include "ui_reg.h"

void
Log::on_pbtn1_clicked ()
{
  auto user = ui.ledit1->text ();
  auto pass = ui.ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    return (void)QMessageBox::warning (nullptr, tr ("提示"),
                                       tr ("请输入帐号密码"));

  auto dialog = QDialog (this);
  auto ui = Ui::Reg ();
  ui.setupUi (&dialog);

  connect (ui.pbtn1, &QPushButton::clicked, [&] { dialog.close (); });

  connect (ui.pbtn2, &QPushButton::clicked, [&, this] {
    auto type = category ();
    auto name = ui.ledit1->text ();
    auto date = ui.ledit2->text ();

    if (name.isEmpty () || date.isEmpty ())
      return (void)QMessageBox::warning (nullptr, tr ("提示"),
                                         tr ("请完整填写信息"));

    auto req_url
        = QString (type == Type::STUDENT ? URL_STUDENT_REG : URL_TEACHER_REG);

    auto req_data = QMap<QString, QVariant> ();
    req_data["username"] = std::move (user);
    req_data["password"] = std::move (pass);
    req_data["start"] = std::move (date);
    req_data["name"] = std::move (name);

    auto http = Http ();
    auto req = Http::make_req (req_url);
    auto reply = http.post (req, req_data);

    if (!util::check_reply (reply))
      return;

    QMessageBox::information (nullptr, tr ("成功"),
                              tr ("注册成功，请返回登录"));
    dialog.close ();
  });

  dialog.exec ();
}

void
Log::on_pbtn2_clicked ()
{
  auto user = ui.ledit1->text ();
  auto pass = ui.ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    return (void)QMessageBox::warning (nullptr, tr ("提示"),
                                       tr ("请输入帐号密码"));

  auto type = category ();
  auto req_url
      = QString (type == Type::STUDENT ? URL_STUDENT_LOG : URL_TEACHER_LOG);

  auto req_data = QMap<QString, QVariant> ();
  req_data["password"] = std::move (pass);
  req_data["username"] = user;

  auto http = Http ();
  auto req = Http::make_req (req_url);
  auto reply = http.post (req, req_data);

  if (!util::check_reply (reply))
    return;

  auto data = reply->readAll ();
  auto obj = QJsonDocument::fromJson (data).object ();

  auto info = Home::Info{
    .user = std::move (user),
    .name = obj["name"].toString (),
    .start = obj["start"].toString (),
    .token = obj["token"].toString (),
  };

  auto home = new Home (type, std::move (info));
  home->show ();
  close ();
}
