#ifndef FRAKTALE_SMOOTHGRADIENT_HPP
#define FRAKTALE_SMOOTHGRADIENT_HPP


#include "ColorMode.hpp"
#include "../Fractals/Fractal.hpp"
#include "WaveLengthMode.hpp"

class SmoothGradient : public WaveLengthMode {
  public:
    SmoothGradient();
    QRgb getColor(FractalPixelIteration& it) const override;
    int interpolate(int start, int end, double fraction) const;

  protected:
    void fillColormap() override;

    QGradient gradient;
};


#endif //FRAKTALE_SMOOTHGRADIENT_HPP
