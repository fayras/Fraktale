#ifndef FRAKTALE_WAVELENGTHMODE_HPP
#define FRAKTALE_WAVELENGTHMODE_HPP

#include "SmoothGradient.hpp"

class WaveLengthMode : public SmoothGradient {
  public:
    WaveLengthMode(bool smooth = true);

  protected:
    void fillColormap() override;

  private:
    QRgb rgbFromWaveLength(double wave);
};


#endif //FRAKTALE_WAVELENGTHMODE_HPP
