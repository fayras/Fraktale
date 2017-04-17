#include "Mandelbrot.hpp"
#include "../Rendering/MandelbrotRenderTask.hpp"
#include "../ColorModes/Identifiers.hpp"
#include "../ColorModes/WaveLengthMode.hpp"
#include "../ColorModes/Grayscale.hpp"
#include <QPainter>

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
  calculatePreview(fractalCenter, scale);
  calculateNewBounds(fractalCenter, scale);
  calculatePixels();
}

void Mandelbrot::calculateNewBounds(QPoint center, double zoom) {
  QPointF offset((double) center.x() / image.width(), (double) center.y() / image.height());
  bounds.setRect(
      bounds.left() + offset.x() * bounds.width() - 3.5 / 2 * zoom,
      bounds.top() + offset.y() * bounds.height() - zoom,
      3.5 * zoom,
      2 * zoom
  );
}

void Mandelbrot::calculatePixels() {
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

void Mandelbrot::calculatePreview(QPoint center, double zoom) {
  QImage traslated(image);
  QPainter painter(&image);
  QPoint translate(image.width() / 2 - center.x(), image.height() / 2 - center.y());

  image.fill(Qt::black);
  painter.drawImage(translate, traslated);

  emit drawSignal();
}
