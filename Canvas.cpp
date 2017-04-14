#include <QtGui/QPainter>
#include "Canvas.hpp"

void Canvas::paintEvent(QPaintEvent *event) {
  if(pixmap.isNull()) {
    return;
  }

  QPainter painter(this);
  painter.fillRect(rect(), Qt::black);
  painter.drawPixmap(0, 0, pixmap);
}

void Canvas::draw(QImage& image) {
  pixmap = QPixmap::fromImage(image);
  update();
}
