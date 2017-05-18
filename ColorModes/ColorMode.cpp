#include "ColorMode.hpp"
#include "../Fractals/FractalPixel.hpp"

ColorMode::ColorMode()
    : inSetColor(qRgb(0, 0, 0))
{}

QRgb ColorMode::getColor(FractalPixel& it) const {
  if(it.iterations == -1) {
    return inSetColor;
  }
  return colormap[it.iterations % ColormapSize];
}
