#include "KochCurve.hpp"
#include "../Rendering/KochCurveRenderTask.hpp"
#include <QPainter>
#include <QDebug>

KochCurve::KochCurve(int width, int height)
    : Fractal(width, height), _scale(1), _offset()
{
  qRegisterMetaType<std::vector<QPointF> >();
  maxIterations = 5;
  for(int i = 0; i < 3; i++) {
    workers.push_back(std::unique_ptr<KochCurveRenderTask>(new KochCurveRenderTask(this)));
    connect(workers[i].get(), &KochCurveRenderTask::rendered, this, &KochCurve::updatePixels);
  };
}

void KochCurve::update() {
  image.fill(Qt::white);
  points.clear();
  KochLine line1(QPointF(0.15, 0.7), QPointF(0.5, 0.01));
  KochLine line2(QPointF(0.5, 0.01), QPointF(0.85, 0.7));
  KochLine line3(QPointF(0.85, 0.7), QPointF(0.15, 0.7));
  workers[0]->render(maxIterations, line1);
  workers[1]->render(maxIterations, line2);
  workers[2]->render(maxIterations, line3);
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

void KochCurve::updatePixels(std::vector<QPointF> points) {
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(image.width() / 2, image.height() / 2);
  painter.scale(_scale, _scale);
  painter.translate(-image.width() / 2, -image.height() / 2);
  painter.translate(_offset);
  painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  for(int i = 1; i < points.size(); i += 2) {
    QPointF a(points[i - 1].x() * image.width(), points[i - 1].y() * image.height());
    QPointF b(points[i].x() * image.width(), points[i].y() * image.height());
    painter.drawLine(a, b);
  }
  emit drawSignal();
}
