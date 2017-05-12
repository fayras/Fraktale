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
  colors->setCurrentIndex(1);
  map.insert(std::pair<QString, QWidget*>("Farbgebung", colors));
  connect(colors, &QComboBox::currentTextChanged, [=](const QString& text){
    this->setColorMode(colors->currentData().value<Colors::ID>());
  });
  connect(this, &Fractal::colorChanged, [=](const Colors::ID id){
    int index = colors->findData(id);
    if ( index != -1 ) { // -1 for not found
      colors->setCurrentIndex(index);
    }
  });

  QSpinBox* iter = new QSpinBox;
  iter->setRange(1, 5000);
  iter->setValue(maxIterations);
  map.insert(std::pair<QString, QWidget*>("Iterationen", iter));
  connect(iter, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Fractal::setMaxIterations);
  connect(this, &Fractal::iterationsChanged, iter, &QSpinBox::setValue);
  return map;
}

void Fractal::setMaxIterations(unsigned iterations) {
  emit iterationsChanged(iterations);
  maxIterations = iterations;
  update();
}

void Fractal::setColorMode(Colors::ID colorsID) {
  auto found = colorsFactory.find(colorsID);
  assert(found != colorsFactory.end());

  colormapID = colorsID;
  colormap = std::move(found->second());
  emit colorChanged(colorsID);
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

QString Fractal::getOverlayInformation() const {
  return QString();
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
