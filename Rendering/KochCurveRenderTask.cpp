#include "KochCurveRenderTask.hpp"
#include <QDebug>

KochCurveRenderTask::KochCurveRenderTask(QObject *parent)
    : QThread(parent), restart(false), maxIterations(5), firstLine()
{}

KochCurveRenderTask::~KochCurveRenderTask() {
  restart = true;
  wait();
}

void KochCurveRenderTask::run() {
  std::vector<QPointF> points;
  calculateCurve(maxIterations, firstLine, points);
  emit rendered(points);
}

void KochCurveRenderTask::render(int pDepth, KochCurve::KochLine pLine) {
  firstLine = pLine;
  maxIterations = pDepth;
  restart = true;
  wait();
  restart = false;
  start();
}

void KochCurveRenderTask::calculateCurve(int depth, KochCurve::KochLine line, std::vector<QPointF>& points) {
  if(restart) {
    return;
  }

  if(depth <= 0) {
    points.push_back(line.start);
    points.push_back(line.end);
  } else {
    QPointF p2 = line.point2();
    QPointF p3 = line.point3();
    QPointF p4 = line.point4();
    calculateCurve(depth - 1, KochCurve::KochLine(line.start, p2), points);
    calculateCurve(depth - 1, KochCurve::KochLine(p2, p3), points);
    calculateCurve(depth - 1, KochCurve::KochLine(p3, p4), points);
    calculateCurve(depth - 1, KochCurve::KochLine(p4, line.end), points);
  }
}
