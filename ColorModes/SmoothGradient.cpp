#include "SmoothGradient.hpp"
#include "../Fractals/FractalPixelIteration.hpp"
#include <QColor>

SmoothGradient::SmoothGradient(bool smooth)
  : ColorMode(), smooth(smooth)
{
  /*
  gradient.setColorAt(0.0, QColor(0, 7, 100));
  gradient.setColorAt(0.16, QColor(32, 107, 203));
  gradient.setColorAt(0.42, QColor(237, 255, 255));
  gradient.setColorAt(0.6425, QColor(255, 170, 0));
  gradient.setColorAt(0.8575, QColor(0, 2, 0));
  gradient.setColorAt(1.0, QColor(0, 7, 100));
  fillColormap();
   */
}

QRgb SmoothGradient::getColor(FractalPixelIteration& it) const {
  if(!smooth) {
    return ColorMode::getColor(it);
  }
  if(it.iterations == -1) {
    return inSetColor;
  }
  //double index = (double) (it.iterations % ColormapSize) / ColormapSize;
  //QColor color = gradient.keyValueAt(index).value<QColor>();
  //qDebug() << gradient.keyValueAt(index);
  //return color.rgb();
  double log_zn = std::log( it.xValue * it.xValue + it.yValue * it.yValue ) / 2;
  double nu = std::log( log_zn / std::log(2) ) / std::log(2);
  double iteration = it.iterations + 1 - nu;
  QColor color1 = QColor(colormap[(unsigned) std::floor(iteration) % ColormapSize]);
  QColor color2 = QColor(colormap[(unsigned) (std::floor(iteration) + 1) % ColormapSize]);
  double fraction = iteration - ((int) iteration);
  int r = interpolate(color1.red(), color2.red(), fraction);
  int g = interpolate(color1.green(), color2.green(), fraction);
  int b = interpolate(color1.blue(), color2.blue(), fraction);
  return QColor(r, g, b).rgb();
}

int SmoothGradient::interpolate(int start, int end, double fraction) const {
  int diff = end - start;
  return start + (int) ((double) diff * fraction);
}
