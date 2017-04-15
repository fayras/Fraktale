#ifndef FRAKTALE_SMOOTHGRADIENT_HPP
#define FRAKTALE_SMOOTHGRADIENT_HPP

#include "ColorMode.hpp"
#include "../Fractals/Fractal.hpp"

class SmoothGradient : public ColorMode {
  public:
    SmoothGradient(bool smooth = true);
    QRgb getColor(FractalPixelIteration& it) const override;
    int interpolate(int start, int end, double fraction) const;

  protected:
    bool smooth;
    //QGradient gradient;
};

#endif //FRAKTALE_SMOOTHGRADIENT_HPP
