#ifndef FRAKTALE_WAVELENGTHMODE_HPP
#define FRAKTALE_WAVELENGTHMODE_HPP

#include "SmoothGradient.hpp"

/**
 * Farbgebung, welche auf Wellenlängen aus dem Qt-Beispiel für Fraktale stammt.
 *
 * @see <a href="http://doc.qt.io/qt-5/qtcore-threads-mandelbrot-renderthread-cpp.html">Qt Beispiel</a>
 */
class WaveLength : public SmoothGradient {
  public:
    /**
     * Erzeugt eine neue Instanz der Klasse.
     *
     * @param smooth Sollen Farben interpoliert werden, oder nicht.
     */
    WaveLength(bool smooth = true);

  protected:
    void fillColormap() override;

  private:
    QRgb rgbFromWaveLength(double wave);
};


#endif //FRAKTALE_WAVELENGTHMODE_HPP
