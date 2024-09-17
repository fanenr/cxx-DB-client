#include "home.h"
#include "citem.h"
#include "http.h"

#include <QJsonArray>

Home::Home (Type type, Info info)
    : QMainWindow (), type (type), info (std::move (info))
{
  ui.setupUi (this);
  setAttribute (Qt::WA_DeleteOnClose);

  ui.info1->setText (this->info.name);
  ui.info2->setText (this->info.user);

  connect (ui.sort, &QCheckBox::toggled, [this] (bool dec) {});

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

  auto res = http.get (req);
  if (!res.has_value ())
    return (void)QMessageBox::warning (this, tr ("错误"),
                                       tr ("无法发送网络请求"));

  auto array = QJsonDocument::fromJson (res.value ()).array ();
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
}

void
Home::on_pbtn5_clicked ()
{
}

void
Home::on_pbtn6_clicked ()
{
}
