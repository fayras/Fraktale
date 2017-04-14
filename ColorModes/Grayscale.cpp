//
// Created by dimitri on 14.04.17.
//

#include "Grayscale.hpp"

Grayscale::Grayscale() {
  fillColormap();
}

void Grayscale::fillColormap() {
  for (int i = 0; i < ColormapSize; ++i) {
    colormap[i] = qRgb(i % 255, i % 255, i % 255);
  }
}
