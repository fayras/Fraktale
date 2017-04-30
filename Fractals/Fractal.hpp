#ifndef FRAKTALE_FRACTAL_HPP
#define FRAKTALE_FRACTAL_HPP

#include <QObject>
#include <QPoint>
#include <QImage>
#include <memory>
#include "../ColorModes/ColorMode.hpp"

namespace Colors {
  enum ID : unsigned;
}

class Canvas;

/**
 * Abstrakte Klasse für weitere Fraktale.
 */
class Fractal : public QObject {
    Q_OBJECT

  signals:
    /**
     * Signal:
     * Wird ausgelöst, sobald das Fraktal berechnet wurde und
     * das Fraktal oder Teile davon gezeichnet werden können.
     *
     * @signal
     */
    void drawSignal();
    /**
     * Signal:
     * Wird ausgelöst, wenn die Anzahl der Iterationen geändert wurde.
     *
     * @signal
     * @param it Die Anzahl der Iterationen.
     */
    void iterationsChanged(unsigned it);
    /**
     * Signal:
     * Wird ausgelöst, wenn die Farbgebung des Fraktals geändert wurde.
     *
     * @signal
     * @param id Die ID der neuen Farbgebung.
     */
    void colorChanged(Colors::ID id);

  public slots:
    /**
     * Slot:
     * Setzt die Iterations-Anzahl.
     *
     * @slot
     * @param iterations Die neue Anzahl an Iterationen.
     */
    void setMaxIterations(unsigned iterations);
    /**
     * Slot:
     * Setzt eine neue Farbgebung.
     *
     * @slot
     * @param colorsID Die ID der neuen Farbgebung.
     */
    void setColorMode(Colors::ID colorsID);

  public:
    /**
     * Zur vereinfachten Schreibweise eines (smart) Zeigers auf ein Fraktal.
     */
    typedef std::unique_ptr<Fractal> Ptr;

    /**
     * Erzeugt eine neue Instanz des Fraktals.
     *
     * @param width Breite des sichtabaren Bereichs.
     * @param height Höhe des sichtabaren Bereichs.
     */
    Fractal(int width, int height);

    /**
     * Verschiebt das Fraktal um einen bestimmten Offset.
     *
     * @param offset Um wie viele Pixel das Fraktal verschoben werden soll.
     */
    virtual void translate(QPointF offset);
    /**
     * Skaliert das Fraktal um einen bestimmten Faktor.
     *
     * @param factor Faktor, um welchen das Fraktal skaliert werden soll.
     *               2 bedeutet hereinzoomen, 0.5 bedeutet herauszoomen.
     */
    virtual void scale(double factor);
    /**
     * Aktualisiert das Fraktal.
     */
    virtual void update() = 0;
    /**
     * Zeichnet das Fraktal auf eine Zeichenfläche.
     *
     * @param target Zeichenfläche, auf die das Fraktal gezeichnet werden soll.
     */
    virtual void draw(Canvas& target);
    /**
     * Verändert das Fraktal in der Größe.
     *
     * @param pWidth Die neue Breite des sichtbaren Bereichs.
     * @param pHeight Die neue Höhe des sichtbaren Bereichs.
     */
    virtual void resize(int pWidth, int pHeight);
    /**
     * Liefert die Einstellungen des Fraktals, welche verändert werden können.
     *
     * Standardmäßig können die Farbgebung und die Anzahl an Iterationen
     * verändert werden.
     *
     * @return Eine Gruppe von QWidgets, welche diverse Einstellungen am Fraktal verändern können.
     */
    virtual std::map<QString, QWidget*> getSettings();
    /**
     * Registriert eine Farbgebung, welche in dem Fraktal verwendet werden kann.
     *
     * @tparam T Die Klasse der Farbgebung. Siehe @link ColorMode @endlink
     * @tparam Args Zusätzliche Parameter, je nachdem welche Farbgebung benutzt wird.
     * @param colorID Die ID, unter welcher die Farbgebung gespeichert werden soll.
     * @param args Zusätzliche Parameter, welche an die Klasse beim Intanzieren übergeben werden sollen.
     */
    template <typename T, typename... Args>
    void registerColorMode(Colors::ID colorID, Args&&... args);
    /**
     * Gibt das gerenderte Bild des Fraktals zurück.
     *
     * @return Das Fraktal als ein Bild.
     */
    const QImage& getImage() const;

    /**
     * Operator-Overload, sodass man das Fraktal an
     * einen QDataStream hängen kann.
     *
     * @param os Data-Stream, welcher die Werte aus dem Fraktal liest.
     * @param fractal
     * @return
     */
    friend QDataStream& operator<<(QDataStream& os, const Fractal& fractal);
    /**
     * Operator-Overload, sodass man das Fraktal an
     * einen QDataStream hängen kann.
     *
     * @param os Data-Stream, welcher die Werte in das Fraktal schreibt.
     * @param fractal
     * @return
     */
    friend QDataStream& operator>>(QDataStream& os, Fractal& fractal);

  protected:
    /**
     * Hier drauf werden die einzelnen Pixel des Fraktals gezeichnet.
     */
    QImage image;
    /**
     * Ein Zeiger auf die aktuelle Farbgebung.
     */
    ColorMode::Ptr colormap;
    /**
     * Die ID der aktuellen Farbgebung.
     */
    Colors::ID colormapID;
    /**
     * Aktuelle Anzahl an Iterationen.
     */
    unsigned maxIterations;
    /**
     * Ein Factory, welche neue Farbgebungen kreiert, wenn welche benötigt werden.
     */
    std::map<Colors::ID, std::function<ColorMode::Ptr()>> colorsFactory;

    /**
     * Funktion, welche aufgerufen wird, wenn das Fraktal in einen Data-Stream
     * geschrieben werden soll.
     *
     * @param os Data-Stream, welcher die Werte in das Fraktal schreibt.
     * @return
     */
    virtual QDataStream& print(QDataStream& os) const;
    /**
     * Funktion, welche aufgerufen wird, wenn das Fraktal aus einem Data-Stream
     * gelesen werden soll.
     *
     * @param os Data-Stream, welcher die Werte aus dem Fraktal liest.
     * @return
     */
    virtual QDataStream& read(QDataStream& os);
};

template<typename T, typename... Args>
void Fractal::registerColorMode(Colors::ID colorID, Args&&... args) {
  colorsFactory[colorID] = [=] () -> ColorMode::Ptr {
    return ColorMode::Ptr(new T(args...));
  };
}

#endif //FRAKTALE_FRACTAL_HPP
