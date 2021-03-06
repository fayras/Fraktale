#include "KochCurve.hpp"
#include "../Rendering/KochCurveRenderTask.hpp"
#include <QPainter>
#include <QDebug>
#include <QtWidgets/QSpinBox>

KochCurve::KochCurve(int width, int height)
    : Fractal(width, height), _scale(1), _offset()
{
  qRegisterMetaType<std::vector<QPointF> >();
  maxIterations = 5;
  for(int i = 0; i < 3; i++) {
    workers.push_back(std::unique_ptr<KochCurveRenderTask>(new KochCurveRenderTask(this)));
    connect(workers[i].get(), &KochCurveRenderTask::rendered, this, &KochCurve::updatePixels);
    connect(workers[i].get(), &KochCurveRenderTask::finished, this, &KochCurve::checkThreadStatus);
  };
}

void KochCurve::update() {
  emit startRendering();
  image.fill(Qt::white);
  points.clear();
  // Die drei Anfangslinien, welche die Schneeflocke ausmachen.
  Line line1(QPointF(0.15, 0.7), QPointF(0.5, 0.01));
  Line line2(QPointF(0.5, 0.01), QPointF(0.85, 0.7));
  Line line3(QPointF(0.85, 0.7), QPointF(0.15, 0.7));
  workers[0]->render(maxIterations, line1);
  workers[1]->render(maxIterations, line2);
  workers[2]->render(maxIterations, line3);
}

void KochCurve::curve(int depth, Line line) {
  if(depth <= 0) {
    points.push_back(line.start);
    points.push_back(line.end);
  } else {
    // Hole die neuen Punkte, führe den Algorithmus
    // für die Unterlinien aus.
    QPointF p2 = line.point2();
    QPointF p3 = line.point3();
    QPointF p4 = line.point4();
    curve(depth - 1, Line(line.start, p2));
    curve(depth - 1, Line(p2, p3));
    curve(depth - 1, Line(p3, p4));
    curve(depth - 1, Line(p4, line.end));
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

  painter.translate(image.width() / 2, image.height() / 2);
  painter.scale(_scale, _scale);
  painter.translate(-image.width() / 2, -image.height() / 2);
  painter.translate(_offset);
  painter.setClipRect(0, 0, image.width(), image.height());
  painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  QPainterPath path(QPointF(points[0].x() * image.width(), points[0].y() * image.height()));
  for(int i = 1; i < points.size(); i += 2) {
    //QPointF a(points[i - 1].x() * image.width(), points[i - 1].y() * image.height());
    QPointF b(points[i].x() * image.width(), points[i].y() * image.height());
    //painter.drawLine(a, b);
    path.lineTo(b);
  }

  painter.drawPath(path);
  emit drawSignal();
}

std::map<QString, QWidget *> KochCurve::getSettings() {
  std::map<QString, QWidget *> map;

  QSpinBox* iter = new QSpinBox;
  iter->setRange(1, 5000);
  iter->setValue(maxIterations);
  map.insert(std::pair<QString, QWidget*>("Iterationen", iter));
  connect(iter, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Fractal::setMaxIterations);
  connect(this, &Fractal::iterationsChanged, iter, &QSpinBox::setValue);
  return map;
}

QDataStream &KochCurve::print(QDataStream &os) const {
  return os << maxIterations << _offset << _scale;
}

QDataStream &KochCurve::read(QDataStream &os) {
  os >> maxIterations >> _offset >> _scale;
  emit iterationsChanged(maxIterations);
  update();
  return os;
}


void KochCurve::checkThreadStatus() {
  bool allThreadsDone = true;
  for(auto& worker : workers) {
    if(!worker->isFinished()) {
      allThreadsDone = false;
    }
  }
  if(allThreadsDone) {
    emit finishedRendering();
  }
}
