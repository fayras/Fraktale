#include <QtGui/QPainter>
#include <QDebug>
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

void Canvas::drawOverlay(QString info) {
  // Ist keine Info vorhanden, so brauchen wir gar nicht
  // erst das durchsichtige Rechteck zu zeichnen.
  if(info.isEmpty()) {
    return;
  }

  QPainter painter(&pixmap);
  painter.setPen(Qt::white);
  painter.drawText(pixmap.rect().marginsRemoved(QMargins(10, 10, 10, 10)), Qt::AlignRight, info);
  update();
}
