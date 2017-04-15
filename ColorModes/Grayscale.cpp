#include "Grayscale.hpp"

Grayscale::Grayscale() {
  fillColormap();
}

void Grayscale::fillColormap() {
  for (int i = 0; i < ColormapSize; ++i) {
    colormap[i] = qRgb(i % 256, i % 256, i % 256);
  }
}
