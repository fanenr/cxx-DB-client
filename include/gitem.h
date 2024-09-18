#ifndef GITEM_H
#define GITEM_H

#include "ui_gitem.h"
#include <QListWidgetItem>

struct Grade
{
  int id;
  int score;
  QString course;
  QString teacher;
};

class GradeWidget : public QWidget
{
  Q_OBJECT

private:
  Ui::Gitem ui = {};

public:
  GradeWidget (Grade const &data)
  {
    ui.setupUi (this);
    ui.label1->setText (data.course);
    ui.label2->setText (QString::number (data.score));
  }
};

class GradeItem : public QListWidgetItem
{
public:
  Grade data;

  GradeItem (QListWidget *list, Grade data)
      : QListWidgetItem (list), data (std::move (data))
  {
    auto widget = new GradeWidget (this->data);
    QListWidgetItem::setSizeHint (widget->sizeHint ());
    list->setItemWidget (this, widget);
  }

  bool
  operator< (QListWidgetItem const &other) const override
  {
    return data.course < ((GradeItem const &)other).data.course;
  }
};

#endif
