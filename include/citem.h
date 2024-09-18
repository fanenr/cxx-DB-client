#ifndef CITEM_H
#define CITEM_H

#include "ui_citem.h"
#include <QListWidgetItem>

struct Course
{
  int id;
  QString name;
  QString start;
  QString teacher;
};

class CourseWidget : public QWidget
{
  Q_OBJECT

private:
  Ui::Citem ui = {};

public:
  CourseWidget (Course const &data)
  {
    ui.setupUi (this);
    ui.label1->setText (data.name);
    ui.label2->setText (data.teacher);
  }
};

class CourseItem : public QListWidgetItem
{
public:
  Course data;

  CourseItem (QListWidget *list, Course data)
      : QListWidgetItem (list), data (std::move (data))
  {
    auto widget = new CourseWidget (this->data);
    QListWidgetItem::setSizeHint (widget->sizeHint ());
    list->setItemWidget (this, widget);
  }

  bool
  operator< (QListWidgetItem const &other) const override
  {
    return data.name < ((CourseItem const &)other).data.name;
  }
};

#endif
