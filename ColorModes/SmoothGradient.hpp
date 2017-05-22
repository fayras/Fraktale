#ifndef FRAKTALE_SMOOTHGRADIENT_HPP
#define FRAKTALE_SMOOTHGRADIENT_HPP

#include "ColorMode.hpp"

/**
 * Stellt Methoden zur Verfügung, so dass ein fließender
 * Übergang zwischen den Farben berechnet werden kann.
 */
class SmoothGradient : public ColorMode {
  public:
    /**
     * Erzeugt eine neue Instanz der Klasse.
     *
     * @param smooth Sollen Farben interpoliert werden, oder nicht.
     */
    SmoothGradient(bool smooth = true);
    QRgb getColor(FractalPixel& it) const override;

  private:
    /**
     * Interpoliert den Farbwert linear zwischen zwei Farben.
     *
     * @param start Erste Farbe.
     * @param end Zweite Farbe.
     * @param fraction An welcher Stelle die Farbe berechnet werden soll.
     * @return Interpolierter Farbwert.
     */
    int interpolate(int start, int end, double fraction) const;

  protected:
    /**
     * Sollen die Farben interpoliert werden?
     */
    bool smooth;
    /**
     * Der Wert für log(2), damit dieser nicht immer wieder berechnet werden muss.
     */
    double log_2;
};

#endif //FRAKTALE_SMOOTHGRADIENT_HPP
