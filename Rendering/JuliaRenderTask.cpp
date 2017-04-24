#include "JuliaRenderTask.hpp"
#include "../Fractals/FractalPixelIteration.hpp"
#include <QDebug>

namespace JuliaRender {
  double map(double n, double start1, double stop1, double start2, double stop2) {
    return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
  }
}

JuliaRenderTask::JuliaRenderTask(QObject *parent)
    : QThread(parent), restart(false), maxIterations(100), rect(), fractalBounds()
{}

JuliaRenderTask::~JuliaRenderTask() {
  restart = true;
  wait();
}

void JuliaRenderTask::run() {
  for(int pass = 8; pass > 0; pass = pass >> 1) {
    std::vector<FractalPixelIteration> pixelIterations;
    double passMaxIt = maxIterations / pass;
    for(int w = rect.left(); w < rect.right(); w += pass) {
      for(int h = rect.top(); h < rect.bottom(); h += pass) {
        double x = JuliaRender::map(w, rect.left(), rect.right(), fractalBounds.left(), fractalBounds.right());
        double y = JuliaRender::map(h, rect.top(), rect.bottom(), fractalBounds.top(), fractalBounds.bottom());
        int iterations = 0;
        while(x*x + y*y < 4 && iterations < passMaxIt) {
          if(restart) {
            return;
          }
          double xtemp = x*x - y*y + rPart;
          double prod = x*y;
          y = prod + prod + iPart;
          x = xtemp;
          iterations++;
        }

        if(iterations < passMaxIt) {
          pixelIterations.push_back(FractalPixelIteration(w, h, iterations, (int) passMaxIt, x, y));
        } else {
          pixelIterations.push_back(FractalPixelIteration(w, h, -1, (int) passMaxIt, x, y));
        }
      }
    }
    emit rendered(pixelIterations);
  }
}

void JuliaRenderTask::render(QRect pRect, QRectF pBounds, int pMaxIterations, double rP, double iP) {
  rect = pRect;
  fractalBounds = pBounds;
  maxIterations = pMaxIterations;
  rPart = rP;
  iPart = iP;
  restart = true;
  wait();
  restart = false;
  start();
}