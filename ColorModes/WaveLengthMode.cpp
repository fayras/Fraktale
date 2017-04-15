#include "WaveLengthMode.hpp"
#include "SmoothGradient.hpp"

WaveLengthMode::WaveLengthMode(bool smooth)
    : SmoothGradient(smooth)
{
  fillColormap();
}

void WaveLengthMode::fillColormap() {
  for (int i = 0; i < ColormapSize; ++i) {
    colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
  }
}

QRgb WaveLengthMode::rgbFromWaveLength(double wave) {
  double r = 0.0;
  double g = 0.0;
  double b = 0.0;

  if (wave >= 380.0 && wave <= 440.0) {
    r = -1.0 * (wave - 440.0) / (440.0 - 380.0);
    b = 1.0;
  } else if (wave >= 440.0 && wave <= 490.0) {
    g = (wave - 440.0) / (490.0 - 440.0);
    b = 1.0;
  } else if (wave >= 490.0 && wave <= 510.0) {
    g = 1.0;
    b = -1.0 * (wave - 510.0) / (510.0 - 490.0);
  } else if (wave >= 510.0 && wave <= 580.0) {
    r = (wave - 510.0) / (580.0 - 510.0);
    g = 1.0;
  } else if (wave >= 580.0 && wave <= 645.0) {
    r = 1.0;
    g = -1.0 * (wave - 645.0) / (645.0 - 580.0);
  } else if (wave >= 645.0 && wave <= 780.0) {
    r = 1.0;
  }

  double s = 1.0;
  if (wave > 700.0)
    s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
  else if (wave <  420.0)
    s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);

  r = std::pow(r * s, 0.8);
  g = std::pow(g * s, 0.8);
  b = std::pow(b * s, 0.8);
  return qRgb(int(r * 255), int(g * 255), int(b * 255));
}
