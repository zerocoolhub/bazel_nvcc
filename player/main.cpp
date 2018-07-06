#include <QtWidgets/QApplication>
#include "main_window.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow mainWindow;

  mainWindow.resize(1024, 768);
  mainWindow.show();

  return app.exec();
}