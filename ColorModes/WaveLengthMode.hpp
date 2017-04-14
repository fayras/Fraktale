#ifndef FRAKTALE_WAVELENGTHMODE_HPP
#define FRAKTALE_WAVELENGTHMODE_HPP

#include "ColorMode.hpp"

class WaveLengthMode : public ColorMode {
  public:
    WaveLengthMode();

  protected:
    void fillColormap() override;

  private:
    QRgb rgbFromWaveLength(double wave);
};


#endif //FRAKTALE_WAVELENGTHMODE_HPP
