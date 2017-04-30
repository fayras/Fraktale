#ifndef FRAKTALE_FRACTALPIXELITERATION_HPP
#define FRAKTALE_FRACTALPIXELITERATION_HPP

#include <vector>
#include <QtCore/QMetaType>

/**
 * Speichert Informationen zu Berechneten Pixel.
 * Zur Zeit nur für Mandelbrot und Julia interessant.
 */
struct FractalPixelIteration {
  FractalPixelIteration(int pX, int pY, int pIt, int pMaxIt, double pXV, double pYV) {
    x = pX; y = pY; iterations = pIt; maxIterations = pIt; xValue = pXV; yValue = pYV;
  }

  /**
   * X-Koordinate auf der Zeichenfläche.
   */
  int x;
  /**
   * Y-Koordinate auf der Zeichenfläche.
   */
  int y;
  /**
   * Die Anzahl der berechneten Iterationen.
   */
  int iterations;
  /**
   * Die Anzahl der maximalen möglichen Iterationen.
   */
  int maxIterations;
  /**
   * Der x-Wert der Berechnung.
   */
  double xValue;
  /**
   * Der y-Wert der Berechnung.
   */
  double yValue;
};

Q_DECLARE_METATYPE(std::vector<FractalPixelIteration>);

#endif //FRAKTALE_FRACTALPIXELITERATION_HPP
