#include "RenderTask.hpp"

RenderTask::RenderTask(QObject *parent)
    : QThread(parent), restart(false)
{}

RenderTask::~RenderTask() {
  restart = true;
  wait();
}

double RenderTask::map(double n, double start1, double stop1, double start2, double stop2) {
  return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
}
