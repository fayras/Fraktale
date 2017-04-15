#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include <cassert>
#include "Fractal.hpp"

Fractal::Fractal(int width, int height)
  : width(width), height(height),
    maxIterations(1000),
    image(width, height, QImage::Format_RGB32),
    scale(1), fractalCenter(width / 2, height / 2),
    colormap(nullptr)
{
  resize(width, height);
}

void Fractal::resize(int pWidth, int pHeight) {
  width = pWidth;
  height = pHeight;
  image = image.scaled(width, height);
}

void Fractal::draw(Canvas& target) {
  target.draw(image);
}

std::map<QString, QWidget*> Fractal::getSettings() {
  std::map<QString, QWidget *> map;

  QComboBox* colors = new QComboBox;
  colors->addItem("Graustufen", Colors::ID::GRAYSCALE);
  colors->addItem("Wellenlänge", Colors::ID::WAVELENGTH);
  colors->addItem("Fließend", Colors::ID::SMOOTH_WAVELENGTH);
  map.insert(std::pair<QString, QWidget*>("Farbgebung", colors));
  connect(colors, &QComboBox::currentTextChanged, [=](const QString& text){
    this->setColorMode(colors->currentData().value<Colors::ID>());
  });

  QSpinBox* iter = new QSpinBox;
  iter->setRange(10, 5000);
  iter->setValue(maxIterations);
  map.insert(std::pair<QString, QWidget*>("Iterationen", iter));
  connect(iter, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Fractal::setMaxIterations);
  return map;
}

void Fractal::setMaxIterations(unsigned iterations) {
  maxIterations = iterations;
  update();
}

void Fractal::update(QPoint center) {
  fractalCenter = center;
  update();
}

void Fractal::update(double factor) {
  scale *= factor;
  update();
}

void Fractal::update(QPoint center, double factor) {
  fractalCenter = center;
  scale *= factor;
  update();
}

void Fractal::setColorMode(Colors::ID colorsID) {
  auto found = colorsFactory.find(colorsID);
  assert(found != colorsFactory.end());

  colormap = std::move(found->second());
  update();
}
