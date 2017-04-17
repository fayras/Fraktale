#include <QtGui/QPainter>
#include "Canvas.hpp"

void Canvas::paintEvent(QPaintEvent *event) {
  // Gibt es keine Pixel zum Zeichnen, so muss erst
  // gar nicht ein QPainter erstellt werden.
  if(pixmap.isNull()) {
    return;
  }

  QPainter painter(this);
  // Setzt alle Pixel auf Schwarz, damit keine
  // Pixel des vorherigen Bilds sichtbar sind.
  painter.fillRect(rect(), Qt::black);
  painter.drawPixmap(0, 0, pixmap);
}

void Canvas::draw(QImage& image) {
  pixmap = QPixmap::fromImage(image);
  update();
}
