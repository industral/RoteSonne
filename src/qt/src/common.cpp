#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>

#include "ui/MainWindow_UI.hpp"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  RoteSonne::UI::MainWindow_UI * mainWindow =
      new RoteSonne::UI::MainWindow_UI();
  QWidget *widget = mainWindow -> getUI();

  if (!widget)
    return 2;

  //  QWidget *l = widget->findChild < QWidget* > ("centralwidget");
  //  if (!l) {
  //    qDebug("widget not found");
  //    delete widget;
  //    return 2;
  //  }

  //  QLayout *lay = l->layout();
  //  if (!lay) {
  //    qDebug("widget has no layout");
  //    return 2;
  //  }

  //  QGridLayout *grid = qobject_cast < QGridLayout* > (lay);
  //  if (!grid) {
  //    qDebug("Not a grid");
  //    return 2;
  //  }

  //  grid->addWidget(new QLabel("TESTING"), 0, 0, 1, 4);
  widget->show();
  int r = app.exec();

  delete mainWindow;
  delete widget;

  return r;
}
