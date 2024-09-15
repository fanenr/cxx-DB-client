#ifndef HOME_H
#define HOME_H

#include "ui_home.h"
#include "util.h"

class Mod;
class New;
class Eva;
class Log;

class Home : public QMainWindow
{
  Q_OBJECT

  friend class Log;
  friend class Mod;
  friend class New;
  friend class Eva;

public:
  struct Info
  {
    QString name;
    QString token;
  };

private:
  Ui::Home ui = {};
  Info info;
  Type typ;

public:
  Home (Type typ, Info info);
  void load_info ();
  void load_dish ();
  void load_eval ();

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
  void on_pbtn4_clicked ();
  void on_pbtn5_clicked ();
  void on_pbtn6_clicked ();
};

#endif
