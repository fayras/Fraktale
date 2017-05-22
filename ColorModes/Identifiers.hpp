#ifndef FRAKTALE_COLORSIDENTIFIERS_HPP
#define FRAKTALE_COLORSIDENTIFIERS_HPP

#include <QtCore/QMetaType>

namespace Colors {
  /**
   * IDs für die verschiedenen Farbmodi.
   */
  enum ID : unsigned {
    GRAYSCALE,
    WAVELENGTH,
    SMOOTH_WAVELENGTH
  };
}

/**
 * Mittels Q_DECLARE_METATYPE wird ein neuer Typ deklariert,
 * so dass dieser mit QVariant benutzt werden kann.
 */
Q_DECLARE_METATYPE(Colors::ID);

#endif //FRAKTALE_COLORSIDENTIFIERS_HPP
