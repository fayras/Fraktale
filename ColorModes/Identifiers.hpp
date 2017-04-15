#ifndef FRAKTALE_COLORSIDENTIFIERS_HPP
#define FRAKTALE_COLORSIDENTIFIERS_HPP

#include <QtCore/QMetaType>

namespace Colors {
  enum ID {
    GRAYSCALE,
    WAVELENGTH,
    SMOOTH
  };
}

Q_DECLARE_METATYPE(Colors::ID);

#endif //FRAKTALE_COLORSIDENTIFIERS_HPP
