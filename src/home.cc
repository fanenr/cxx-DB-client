#include "home.h"

Home::Home (Type type, Info info)
    : QMainWindow (), type (type), info (std::move (info))
{
  ui.setupUi (this);
  setAttribute (Qt::WA_DeleteOnClose);

  ui.info1->setText (this->info.name);
  ui.info2->setText (this->info.user);

  connect (ui.sort, &QCheckBox::toggled, [this] (bool dec) {});
}

void
Home::on_pbtn1_clicked ()
{
}

void
Home::on_pbtn2_clicked ()
{
}

void
Home::on_pbtn3_clicked ()
{
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
