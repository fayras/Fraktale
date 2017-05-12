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

void Canvas::drawOverlay(QStringList infoLines) {
  // Ist keine Info vorhanden, so brauchen wir gar nicht
  // erst das durchsichtige Rechteck zu zeichnen.
  if(infoLines.isEmpty()) {
    return;
  }

  QString info = infoLines.join("\n");
  QPainter painter(&pixmap);
  QFontMetrics metrics = painter.fontMetrics();
  QRect bgRect = metrics.boundingRect(pixmap.rect(), Qt::TextWordWrap, info);
  bgRect.moveTo(pixmap.rect().right() - bgRect.width() - 10, 10);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(0, 0, 0, 127));
  painter.drawRect(bgRect.marginsAdded(QMargins(5, 5, 5, 5)));
  painter.setPen(Qt::white);
  painter.drawText(pixmap.rect().marginsRemoved(QMargins(10, 10, 10, 10)), Qt::AlignRight, info);
  update();
}
