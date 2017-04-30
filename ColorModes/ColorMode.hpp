#ifndef FRAKTALE_COLORMODE_HPP
#define FRAKTALE_COLORMODE_HPP

#include <QtGui/QRgb>
#include <memory>

struct FractalPixelIteration;

/**
 * Eine abstrakte Klasse für Farbgebungen.
 */
class ColorMode {
  public:
    /**
     * Zur vereinfachten Schreibweise eines (smart) Zeigers auf ein ColorMode.
     */
    typedef std::unique_ptr<ColorMode> Ptr;

    /**
     * Erzeugt eine neue Instanz der Klasse.
     */
    ColorMode();
    /**
     * Lieft die entsprechende Farbe zu einem berechneten Pixel.
     *
     * Die Farbe kommt dabei aus der zuvor berechneten @link ColorMode::colormap @endlink.
     * @see @link ColorMode::fillColormap @endlink
     *
     * @param it Der berechnete Pixel.
     * @return
     */
    virtual QRgb getColor(FractalPixelIteration& it) const;

  protected:
    /**
     * Anzahl der einzelnen verfügbaren Farben.
     */
    enum { ColormapSize = 256 };
    /**
     * Ein Array mit den verfügbaren Farben.
     */
    QRgb colormap[ColormapSize];
    /**
     * Die Farbe, welche "im Set" representiert.
     *
     * @see @link Mandelbrot @endlink, @link Julia @endlink
     */
    QRgb inSetColor;

    /**
     * Befüllt die colormap der Klasse mit Farben, welche dann mittels
     * @link ColorMode::getColor @endlink geholt werden können.
     */
    virtual void fillColormap() = 0;
};

#endif //FRAKTALE_COLORMODE_HPP
