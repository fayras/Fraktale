#include "KochCurve.hpp"
#include <QPainter>
#include <QDebug>

KochCurve::KochCurve(int width, int height)
    : Fractal(width, height), _scale(1), _offset()
{
  maxIterations = 5;
}

void KochCurve::update() {
  points.clear();
  KochLine line(QPointF(0, 0.5), QPointF(1, 0.5));
  curve(maxIterations, line);

  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(image.width() / 2, image.height() / 2);
  painter.scale(_scale, _scale);
  painter.translate(-image.width() / 2, -image.height() / 2);
  painter.translate(_offset);
  painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  image.fill(Qt::white);
  for(int i = 0; i < points.size() - 1; i += 2) {
    QPointF a(points[i].x() * image.width(),points[i].y() * image.height());
    QPointF b(points[i + 1].x() * image.width(), points[i + 1].y() * image.height());
    painter.drawLine(a, b);
  }
  emit drawSignal();
}

void KochCurve::curve(int depth, KochLine line) {
  if(depth <= 0) {
    points.push_back(line.start);
    points.push_back(line.end);
  } else {
    QPointF p2 = line.point2();
    QPointF p3 = line.point3();
    QPointF p4 = line.point4();
    curve(depth - 1, KochLine(line.start, p2));
    curve(depth - 1, KochLine(p2, p3));
    curve(depth - 1, KochLine(p3, p4));
    curve(depth - 1, KochLine(p4, line.end));
  }
}

void KochCurve::translate(QPointF offset) {
  Fractal::translate(offset);
  _offset = _offset + offset / _scale;
  update();
}

void KochCurve::scale(double factor) {
  Fractal::scale(factor);
  _scale *= factor;
  update();
}
