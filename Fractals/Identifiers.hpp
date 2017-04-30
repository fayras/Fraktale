#ifndef FRAKTALE_FRACTALIDENTIFIERS_HPP
#define FRAKTALE_FRACTALIDENTIFIERS_HPP

namespace Fractals {
  /**
   * IDs zu allen verfügbaren Fraktalen.
   */
  enum ID : unsigned {
    EMPTY_FRACTAL,
    MANDELBROT,
    JULIA,
    KOCH_CURVE
  };
}

Q_DECLARE_METATYPE(Fractals::ID);

#endif //FRAKTALE_FRACTALIDENTIFIERS_HPP
