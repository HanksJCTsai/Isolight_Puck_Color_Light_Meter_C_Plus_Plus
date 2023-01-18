#include "mainwindow.h"

#include <qfile.h>
#include <qtextstream.h>
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
  QFile f(":/qdarkstyle/dark/darkstyle.qss");
  f.open(QFile::ReadOnly | QFile::Text);
  QTextStream ts(&f);
  qApp->setStyleSheet(ts.readAll());
  MainWindow w;
  w.show();
  return a.exec();
}
