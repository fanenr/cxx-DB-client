#include "reg.h"
#include "http.h"
#include "log.h"

#include <QJsonDocument>
#include <QJsonObject>

Reg::Reg (Log *parent) : QDialog (parent)
{
  this->parent = parent;
  ui.setupUi (this);
}

int
Reg::exec ()
{
  if (parent->category () == Type::TEACHER)
    {
      ui.hint2->setVisible (false);
      ui.ledit2->setVisible (false);
    }
  return QDialog::exec ();
}

void
Reg::on_pbtn1_clicked ()
{
  close ();
}

void
Reg::on_pbtn2_clicked ()
{
  auto name = ui.ledit1->text ();
  auto date = ui.ledit2->text ();

  if (name.isEmpty () || date.isEmpty ())
    return (void)QMessageBox::warning (nullptr, tr ("提示"),
                                       tr ("请完整填写信息"));

  auto type = parent->category ();
  auto req_url
      = QString (type == Type::STUDENT ? URL_STUDENT_REG : URL_TEACHER_REG);

  auto req_data = QMap<QString, QVariant> ();
  req_data["username"] = parent->ui.ledit1->text ();
  req_data["password"] = parent->ui.ledit2->text ();
  req_data["start"] = std::move (date);
  req_data["name"] = std::move (name);

  auto http = Http ();
  auto req = Http::make_req (req_url);
  auto reply = http.post (req, req_data);

  if (!util::check_reply (reply))
    return;

  QMessageBox::information (nullptr, tr ("成功"), tr ("注册成功，请返回登录"));
  close ();
}
