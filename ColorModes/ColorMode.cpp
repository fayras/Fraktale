//
// Created by dimitri on 12.04.17.
//

#include "ColorMode.hpp"

ColorMode::ColorMode()
    : inSetColor(qRgb(0, 0, 0))
{}

QRgb ColorMode::getColor(int it) const {
  if(it == -1) {
    return inSetColor;
  }
  return colormap[it % ColormapSize];
}
