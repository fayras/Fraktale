#ifndef FRAKTALE_GRAYSCALE_HPP
#define FRAKTALE_GRAYSCALE_HPP

#include "ColorMode.hpp"

/**
 * Farbgebung für Graustufen.
 */
class Grayscale : public ColorMode {
  public:
    Grayscale();

  protected:
    void fillColormap() override;
};

#endif //FRAKTALE_GRAYSCALE_HPP
