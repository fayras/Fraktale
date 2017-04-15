#ifndef FRAKTALE_COLORMODE_HPP
#define FRAKTALE_COLORMODE_HPP

#include <QtGui/QRgb>
#include <memory>
#include "../Fractals/FractalPixelIteration.hpp"

class ColorMode {
  public:
    typedef std::unique_ptr<ColorMode> Ptr;

    ColorMode();

    virtual QRgb getColor(FractalPixelIteration& it) const;

  protected:
    enum { ColormapSize = 256 };
    QRgb colormap[ColormapSize];
    QRgb inSetColor;

    virtual void fillColormap() = 0;
};

#endif //FRAKTALE_COLORMODE_HPP
