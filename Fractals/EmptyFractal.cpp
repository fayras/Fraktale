//
// Created by dimitri on 11.04.17.
//

#include <QtGui/QPainter>
#include <QDebug>
#include "EmptyFractal.hpp"

EmptyFractal::EmptyFractal(int width, int height)
    : Fractal(width, height)
{}

void EmptyFractal::update() {
  emit drawSignal();
}

void EmptyFractal::draw(Canvas &target) {
  QPainter painter(&image);
  painter.fillRect(image.rect(), Qt::white);
  painter.drawText(image.rect(), Qt::AlignCenter | Qt::AlignVCenter, "Bitte ein Fraktal auswählen.");
  target.draw(image);
}

std::map<QString, QWidget *> EmptyFractal::getSettings() {
  std::map<QString, QWidget *> map;
  return map;
}
