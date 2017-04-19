#include "Mandelbrot.hpp"
#include "../Rendering/MandelbrotRenderTask.hpp"
#include "../ColorModes/Identifiers.hpp"
#include "../ColorModes/WaveLengthMode.hpp"
#include "../ColorModes/Grayscale.hpp"
#include <QPainter>
#include <cmath>

Mandelbrot::Mandelbrot(int width, int height)
  : Fractal(width, height), bounds(-2.5, -1, 3.5, 2)
{
  qRegisterMetaType<std::vector<FractalPixelIteration> >();
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    workers.push_back(new MandelbrotRenderTask(this));
    connect(workers[i], &MandelbrotRenderTask::rendered, this, &Mandelbrot::updatePixels);
  };
  registerColorMode<Grayscale>(Colors::ID::GRAYSCALE);
  registerColorMode<WaveLengthMode>(Colors::ID::WAVELENGTH, false);
  registerColorMode<WaveLengthMode>(Colors::ID::SMOOTH_WAVELENGTH, true);
  setColorMode(Colors::ID::WAVELENGTH);
}

void Mandelbrot::update() {
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
    workers[i]->render(rect, tempBounds, maxIterations);
    //workers[i]->wait();
  }
}

void Mandelbrot::updatePixels(std::vector<FractalPixelIteration> pixelIterations) {
  for(FractalPixelIteration& it : pixelIterations) {
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
