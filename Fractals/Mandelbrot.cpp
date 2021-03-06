#include "Mandelbrot.hpp"
#include "../Rendering/MandelbrotRenderTask.hpp"
#include "../ColorModes/Identifiers.hpp"
#include "../ColorModes/WaveLength.hpp"
#include "../ColorModes/Grayscale.hpp"
#include <QPainter>
#include <cmath>
#include <QDebug>

Mandelbrot::Mandelbrot(int width, int height)
  : Fractal(width, height), bounds(-2.5, -1, 3.5, 2)
{
  qRegisterMetaType<std::vector<FractalPixel> >();
  // Erzeuge die Workers zum Berechnen des Fraktals.
  createWorkers();
  // Registriere die Farbmodie, welche benutzt werden können.
  registerColorMode<Grayscale>(Colors::ID::GRAYSCALE);
  registerColorMode<WaveLength>(Colors::ID::WAVELENGTH, false);
  registerColorMode<WaveLength>(Colors::ID::SMOOTH_WAVELENGTH, true);
  setColorMode(Colors::ID::WAVELENGTH);
}

void Mandelbrot::update() {
  emit startRendering();
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    QRect rect(
        (int) std::round((double) i * (double) image.width() / (double) QThread::idealThreadCount()),
        0,
        (int) std::ceil((double) image.width() / (double) QThread::idealThreadCount()) + 1,
        image.height()
    );
    QRectF tempBounds(
        bounds.left() + (double) i * bounds.width() / (double) QThread::idealThreadCount(),
        bounds.top(),
        bounds.width() / (double) QThread::idealThreadCount(),
        bounds.height()
    );
    workers[i]->render(rect, tempBounds, maxIterations);
    //workers[i]->wait();
  }
}

void Mandelbrot::updatePixels(std::vector<FractalPixel> pixelIterations) {
  for(FractalPixel& it : pixelIterations) {
    if(it.x < image.width() && it.y < image.height()) {
      image.setPixel(it.x, it.y, colormap->getColor(it));
    }
  }
  emit drawSignal();
}

void Mandelbrot::translate(QPointF offset) {
  Fractal::translate(offset);
  bounds.translate(
      -offset.x() / image.width() * bounds.width(),
      -offset.y() / image.height() * bounds.height()
  );
  update();
}

void Mandelbrot::scale(double factor) {
  Fractal::scale(factor);
  QPointF center = bounds.center();
  bounds.setRect(
      center.x() - bounds.width() / factor / 2,
      center.y() - bounds.height() / factor / 2,
      bounds.width() / factor,
      bounds.height() / factor
  );
  update();
}

QDataStream &Mandelbrot::print(QDataStream &os) const {
  return os << maxIterations << static_cast<unsigned>(colormapID) << bounds;
}

QDataStream &Mandelbrot::read(QDataStream &os) {
  unsigned colorID;
  os >> maxIterations >> colorID >> bounds;
  setColorMode(static_cast<Colors::ID>(colorID));
  emit iterationsChanged(maxIterations);
  update();
  return os;
}

void Mandelbrot::createWorkers() {
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    workers.push_back(std::unique_ptr<MandelbrotRenderTask>(new MandelbrotRenderTask(this)));
    connect(workers[i].get(), &MandelbrotRenderTask::rendered, this, &Mandelbrot::updatePixels);
    connect(workers[i].get(), &MandelbrotRenderTask::finished, this, &Mandelbrot::checkThreadStatus);
  };
}

void Mandelbrot::checkThreadStatus() {
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
