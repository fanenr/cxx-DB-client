#include "home.h"
#include "http.h"
#include "item.h"

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
  ui.info3->setText (this->info.start);

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
  if (type == Type::STUDENT)
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

  connect (ui.pbtn1, &QPushButton::clicked, [&] { dialog.close (); });

  connect (ui.pbtn2, &QPushButton::clicked, [&, this] {
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
  req_data["cid"] = item->data.id;

  auto http = Http ();
  auto req = Http::make_req (req_url, info.token);

  auto reply = http.post (req, req_data);
  if (!util::check_reply (reply))
    return;

  QMessageBox::information (nullptr, tr ("成功"), tr ("选修成功"));
}

#include "ui_grade.h"

void
Home::grade_analy ()
{
  auto dialog = QDialog (this);
  auto ui = Ui::Grade ();
  ui.setupUi (&dialog);

  auto list = this->ui.list;
  auto count = list->count ();
  auto items = QList<GradeItem *> ();

  for (int i = 0; i < count; i++)
    items.append ((GradeItem *)list->item (i));

  long sum_grade = 0;
  double sum_point = 0;

  auto points = QMap<double, int> ();
  points[0.0] = 0;
  points[1.0] = 0;
  points[1.5] = 0;
  points[2.0] = 0;
  points[2.3] = 0;
  points[2.7] = 0;
  points[3.0] = 0;
  points[3.3] = 0;
  points[3.7] = 0;
  points[4.0] = 0;

  for (auto i : items)
    {
      auto s = i->data.score;
      sum_grade += s;

      if (s < 60)
        points[0.0]++;
      else if (s < 64)
        points[1.0]++;
      else if (s < 68)
        points[1.5]++;
      else if (s < 72)
        points[2.0]++;
      else if (s < 75)
        points[2.3]++;
      else if (s < 78)
        points[2.7]++;
      else if (s < 82)
        points[3.0]++;
      else if (s < 85)
        points[3.3]++;
      else if (s < 90)
        points[3.7]++;
      else
        points[4.0]++;
    }

  for (auto it = points.cbegin (); it != points.cend (); it++)
    sum_point += it.key () * it.value ();

  auto avg_grade = count ? sum_grade / count : 0;
  auto avg_point = count ? sum_point / (double)count : 0.0;
  auto rate = count ? (count - points[0.0]) / (double)count : 0.0;

  ui.info1->setText (QString::number (count));
  ui.info3->setText (QString::number (avg_grade));
  ui.info2->setText (QString::number (rate, 'f', 1));
  ui.info4->setText (QString::number (avg_point, 'f', 1));

  dialog.exec ();
}

void
Home::on_pbtn6_clicked ()
{
  load_grade ();
  grade_analy ();
}
