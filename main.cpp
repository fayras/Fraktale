#include "FractalWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  // Zeigt das Hauptfenster der Applikation, in
  // dem Fraktale und Menüs angezeigt werden.
  FractalWindow window;
  window.show();

  return app.exec();
}