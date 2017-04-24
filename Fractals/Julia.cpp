#include "Julia.hpp"
#include "../Rendering/JuliaRenderTask.hpp"

Julia::Julia(int width, int height)
    : Mandelbrot(width, height), rPart(0), iPart(0)
{
  workers.clear();
  createWorkers();
}

QDataStream &Julia::print(QDataStream &os) const {
  return Mandelbrot::print(os) << rPart << iPart;
}

QDataStream &Julia::read(QDataStream &os) {
  Mandelbrot::print(os) >> rPart >> iPart;
  update();
  return os;
}

void Julia::createWorkers() {
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    workers.push_back(new JuliaRenderTask(this));
    connect(workers[i], &JuliaRenderTask::rendered, this, &Julia::updatePixels);
  };
}

void Julia::update() {
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    QRect rect(
        (int) std::floor(i * image.width() / QThread::idealThreadCount()),
        0,
        (int) std::ceil((double) image.width() / QThread::idealThreadCount()),
        image.height()
    );
    QRectF tempBounds(
        bounds.left() + i * bounds.width() / QThread::idealThreadCount(),
        bounds.top(),
        bounds.width() / QThread::idealThreadCount(),
        bounds.height()
    );
    workers[i]->render(rect, tempBounds, maxIterations, rPart, iPart);
  }
}
