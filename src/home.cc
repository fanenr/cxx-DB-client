#include "home.h"
#include "citem.h"
#include "gitem.h"
#include "http.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Home::Home (Type type, Info info)
    : QMainWindow (), type (type), info (std::move (info))
{
  ui.setupUi (this);
  setAttribute (Qt::WA_DeleteOnClose);

  ui.info1->setText (this->info.name);
  ui.info2->setText (this->info.user);

  if (this->type == Type::TEACHER)
    ui.pbtn5->setText (tr ("创建课程"));

  connect (ui.sort, &QCheckBox::toggled, [this] (bool dec) {
    ui.list->sortItems (dec ? Qt::DescendingOrder : Qt::AscendingOrder);
  });

  load_course ();
}

void
Home::load_course ()
{
  ui.list->clear ();
  ui.list->clearSelection ();
  ui.gbox2->setTitle (tr ("课程列表"));

  auto req_url
      = QString (type == Type::STUDENT ? URL_STUDENT_LIST : URL_TEACHER_LIST);

  auto http = Http ();
  auto req = Http::make_req (req_url, info.token);

  auto reply = http.get (req);
  if (!util::check_reply (reply))
    return;

  auto data = reply->readAll ();
  auto array = QJsonDocument::fromJson (data).array ();
  for (auto e : array)
    {
      auto obj = e.toObject ();
      auto item = Course{
        .id = obj["id"].toInt (),
        .name = obj["name"].toString (),
        .start = obj["start"].toString (),
      };
      item.teacher
          = (type == Type::STUDENT) ? obj["teacher"].toString () : info.name;
      new CourseItem (ui.list, std::move (item));
    }
}

void
Home::load_grade ()
{
  ui.list->clear ();
  ui.list->clearSelection ();
  ui.gbox2->setTitle (tr ("成绩列表"));

  auto req_url = QString (URL_STUDENT_GRADE);

  auto http = Http ();
  auto req = Http::make_req (req_url, info.token);

  auto reply = http.get (req);
  if (!util::check_reply (reply))
    return;

  auto data = reply->readAll ();
  auto array = QJsonDocument::fromJson (data).array ();
  for (auto e : array)
    {
      auto obj = e.toObject ();
      auto item = Grade{
        .id = obj["id"].toInt (),
        .score = obj["score"].toInt (),
        .course = obj["course"].toString (),
        .teacher = obj["teacher"].toString (),
      };
      new GradeItem (ui.list, std::move (item));
    }
}

void
Home::on_pbtn1_clicked ()
{
}

void
Home::on_pbtn2_clicked ()
{
  close ();
}

void
Home::on_pbtn3_clicked ()
{
  load_course ();
}

void
Home::on_pbtn4_clicked ()
{
  load_grade ();
}

#include "ui_form.h"

void
Home::course_new ()
{
  auto dialog = QDialog (this);
  auto ui = Ui::Form ();
  ui.setupUi (&dialog);

  dialog.setWindowTitle (tr ("创建课程"));
  ui.ledit2->setInputMask ("0000-00-00");
  ui.label->setText (tr ("课程信息"));
  ui.hint1->setText (tr ("课程名称"));
  ui.hint2->setText (tr ("开课时间"));

  connect (ui.pbtn1, &QPushButton::clicked, [&dialog] { dialog.close (); });

  connect (ui.pbtn2, &QPushButton::clicked, [this, &ui, &dialog] {
    auto name = ui.ledit1->text ();
    auto start = ui.ledit2->text ();
    if (name.isEmpty () || start.isEmpty ())
      return (void)QMessageBox::warning (nullptr, tr ("提示"),
                                         tr ("请完整填写信息"));

    auto req_url = QString (URL_TEACHER_NEW);
    auto req_data = QMap<QString, QVariant> ();
    req_data["start"] = std::move (start);
    req_data["name"] = std::move (name);

    auto http = Http ();
    auto req = Http::make_req (req_url, info.token);

    auto reply = http.post (req, req_data);
    if (!util::check_reply (reply))
      return;

    QMessageBox::information (nullptr, tr ("成功"), tr ("创建成功"));
    dialog.close ();
    load_course ();
  });

  dialog.exec ();
}

void
Home::on_pbtn5_clicked ()
{
  if (type == Type::TEACHER)
    return course_new ();

  auto item = (CourseItem *)ui.list->currentItem ();
  if (item == nullptr)
    return;

  auto req_url = QString (URL_STUDENT_TAKE);
  auto req_data = QMap<QString, QVariant> ();
  req_data["cid"] = QString::number (item->data.id);

  auto http = Http ();
  auto req = Http::make_req (req_url, info.token);

  auto reply = http.post (req, req_data);
  if (!util::check_reply (reply))
    return;

  QMessageBox::information (nullptr, tr ("成功"), tr ("选修成功"));
}

void
Home::on_pbtn6_clicked ()
{
}
