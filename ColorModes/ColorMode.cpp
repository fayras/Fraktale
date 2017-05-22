#include "ColorMode.hpp"
#include "../Fractals/FractalPixel.hpp"

ColorMode::ColorMode()
    : inSetColor(qRgb(0, 0, 0))
{}

QRgb ColorMode::getColor(FractalPixel& it) const {
  // Anzahl der Iterationen beträgt genau dann -1,
  // wenn die maximale Anzahl erreicht wurde.
  if(it.iterations == -1) {
    return inSetColor;
  }
  // Ansonsten liefe die entsprechende Farbe aus der Tabelle.
  return colormap[it.iterations % ColormapSize];
}
