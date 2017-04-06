#include "FractalWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  FractalWindow window;
  window.show();

  return app.exec();
}