#ifndef FRAKTALE_COLORMODE_HPP
#define FRAKTALE_COLORMODE_HPP

#include <QtGui/QRgb>
#include <memory>

class ColorMode {
  public:
    typedef std::unique_ptr<ColorMode> Ptr;

    ColorMode();
    QRgb getColor(int it) const;

  protected:
    enum { ColormapSize = 512 };
    QRgb colormap[ColormapSize];
    QRgb inSetColor;

    virtual void fillColormap() = 0;
};

#endif //FRAKTALE_COLORMODE_HPP
