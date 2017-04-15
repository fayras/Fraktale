#include <QtGui/QRgb>
#include <QDebug>
#include "MandelbrotRenderTask.hpp"

double map(double n, double start1, double stop1, double start2, double stop2) {
  return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
}

MandelbrotRenderTask::MandelbrotRenderTask(QObject *parent)
  : QThread(parent), restart(false), maxIterations(100), rect(), fractalBounds()
{}

MandelbrotRenderTask::~MandelbrotRenderTask() {
  restart = true;
  wait();
}

void MandelbrotRenderTask::run() {
  for(int pass = 8; pass > 0; pass = pass >> 1) {
    std::vector<FractalPixelIteration> pixelIterations;
    double passMaxIt = maxIterations / pass;
    for(int w = rect.left(); w < rect.right(); w += pass) {
      double x0 = map(w, rect.left(), rect.right(), fractalBounds.left(), fractalBounds.right());
      for(int h = rect.top(); h < rect.bottom(); h += pass) {
        double y0 = map(h, rect.top(), rect.bottom(), fractalBounds.top(), fractalBounds.bottom());
        double x = 0;
        double y = 0;
        int iterations = 0;
        while(x*x + y*y < 4 && iterations < passMaxIt) {
          if(restart) {
            return;
          }
          double xtemp = x*x - y*y + x0;
          double prod = x*y;
          y = prod + prod + y0;
          x = xtemp;
          iterations++;
        }

        for(int passW = 0; passW < pass; passW++) {
          for(int passH = 0; passH < pass; passH++) {
            if(iterations < passMaxIt) {
              pixelIterations.push_back(FractalPixelIteration(w + passW, h + passH, iterations, (int) passMaxIt, x, y));
            } else {
              pixelIterations.push_back(FractalPixelIteration(w + passW, h + passH, -1, (int) passMaxIt, x, y));
            }
          }
        }
      }
    }
    emit rendered(pixelIterations);
  }
}

void MandelbrotRenderTask::render(QRect pRect, QRectF pBounds, int pMaxIterations) {
  rect = pRect;
  fractalBounds = pBounds;
  maxIterations = pMaxIterations;
  restart = true;
  wait();
  restart = false;
  start();
}
