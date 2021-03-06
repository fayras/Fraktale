#ifndef FRAKTALE_FRACTALPIXELITERATION_HPP
#define FRAKTALE_FRACTALPIXELITERATION_HPP

#include <vector>
#include <QtCore/QMetaType>

/**
 * Speichert Informationen zu Berechneten Pixel.
 * Zur Zeit nur für Mandelbrot und Julia interessant.
 */
struct FractalPixel {
  /**
   * Erzeugt eine neue Instanz der Klasse.
   *
   * @param pX X-Koordinate auf der Zeichenfläche.
   * @param pY Y-Koordinate auf der Zeichenfläche.
   * @param pIt Die Anzahl der berechneten Iterationen.
   * @param pMaxIt Die Anzahl der maximalen möglichen Iterationen.
   * @param pXV Der x-Wert der Berechnung.
   * @param pYV Der y-Wert der Berechnung.
   */
  FractalPixel(int pX, int pY, int pIt, int pMaxIt, double pXV, double pYV) {
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

Q_DECLARE_METATYPE(std::vector<FractalPixel>);

#endif //FRAKTALE_FRACTALPIXELITERATION_HPP
