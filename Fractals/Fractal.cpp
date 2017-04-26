#include "Fractal.hpp"
#include "../Canvas.hpp"
#include "../ColorModes/Identifiers.hpp"
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include <cassert>
#include <QtGui/QPainter>

Fractal::Fractal(int width, int height)
  : maxIterations(1000),
    image(width, height, QImage::Format_RGB32),
    colormap(nullptr)
{
  resize(width, height);
}

void Fractal::resize(int pWidth, int pHeight) {
  image = image.scaled(pWidth, pHeight);
}

void Fractal::draw(Canvas& target) {
  target.draw(image);
}

std::map<QString, QWidget*> Fractal::getSettings() {
  std::map<QString, QWidget *> map;

  QComboBox* colors = new QComboBox;
  colors->addItem("Graustufen", QVariant::fromValue(Colors::ID::GRAYSCALE));
  colors->addItem("Wellenlänge", QVariant::fromValue(Colors::ID::WAVELENGTH));
  colors->addItem("Fließend", QVariant::fromValue(Colors::ID::SMOOTH_WAVELENGTH));
  map.insert(std::pair<QString, QWidget*>("Farbgebung", colors));
  connect(colors, &QComboBox::currentTextChanged, [=](const QString& text){
    this->setColorMode(colors->currentData().value<Colors::ID>());
  });

  QSpinBox* iter = new QSpinBox;
  iter->setRange(1, 5000);
  iter->setValue(maxIterations);
  map.insert(std::pair<QString, QWidget*>("Iterationen", iter));
  connect(iter, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Fractal::setMaxIterations);
  return map;
}

void Fractal::setMaxIterations(unsigned iterations) {
  maxIterations = iterations;
  update();
}

void Fractal::setColorMode(Colors::ID colorsID) {
  auto found = colorsFactory.find(colorsID);
  assert(found != colorsFactory.end());

  colormap = std::move(found->second());
  update();
}

void Fractal::translate(QPointF offset) {
  QImage traslated(image);
  QPainter painter(&image);

  image.fill(Qt::black);
  painter.drawImage(offset, traslated);
  painter.end();

  emit drawSignal();
}

void Fractal::scale(double factor) {
  QImage scaled(image);
  QPainter painter(&image);

  image.fill(Qt::black);
  painter.translate(image.width() / 2, image.height() / 2);
  painter.scale(factor, factor);
  painter.translate(-image.width() / 2, -image.height() / 2);
  painter.drawImage(0, 0, scaled);
  painter.end();

  emit drawSignal();
}

const QImage &Fractal::getImage() const {
  return image;
}

QDataStream& operator<<(QDataStream& os, const Fractal& fractal) {
  return fractal.print(os);
}

QDataStream &Fractal::print(QDataStream &os) const {
  return os;
}

QDataStream& operator>>(QDataStream& os, Fractal& fractal) {
  return fractal.read(os);
}

QDataStream &Fractal::read(QDataStream &os) {
  return os;
}
