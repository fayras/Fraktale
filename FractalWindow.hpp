#ifndef FRAKTALE_FRACTALWINDOW_HPP
#define FRAKTALE_FRACTALWINDOW_HPP

#include <QtWidgets/QDialog>
#include "Canvas.hpp"
#include "Fractals/Fractal.hpp"

namespace Fractals {
  enum ID : unsigned;
}

class QFormLayout;

/**
 * Dies ist das Hauptfenster, welches die Frak-
 * tale und zusätzliche Einstellungen anzeigt.
 */
class FractalWindow : public QDialog {
  // Setzt das Fenster als Q_OBJECT, so dass
  // Signal/Slot-Funktionalität gegeben ist.
  Q_OBJECT

  public slots:
    /**
     * Slot:
     * Zeichnet das Fraktal, welches zur Zeit
     * ausgewählt ist auf das Canvas.
     *
     * @slot
     * @see Canvas()
     */
    void draw();

    /**
     * Slot:
     * Erstellt ein neues Fraktal mit, welches der
     * ID fractalID zugeordnet wurde. Dabei wird
     * das Zentrum, der Zoom und Einstellungen
     * des vorherigen Fraktals zurückgesetzt.
     *
     * @param fractalID Id, welcher ein Fraktal mittels der Funktion
     *                  'registerFractal' zugeordnet wurde.
     *
     * @slot
     * @see registerFractal
     */
    void createFractal(Fractals::ID fractalID);

  public:
    /**
     * Konstruktor, welcher eine Instanz von 'FractalWindow' erzeugt.
     *
     * @param parent Das QWidget, welches den Besitzt über
     *               die Instanz übernehmen sollte.
     */
    explicit FractalWindow(QWidget *parent = 0);

    /**
     * Verschiebt und/oder vergrößert oder verkleinert das aktuelle Fraktal.
     *
     * @param center Das neue Zentrum des aktiven Fraktals.
     * @param factor Der Factor, um welchen das Fraktal vergrößert werden
     *               soll. Dabei ist z.B. factor = 2 gleichbedeutend mit
     *               herauszoomen, factor = 0.5 mit hereinzoomen usw.
     */
    void moveFractal(QPoint center, double factor);

    /**
     * Registriert ein neues Fraktal mit der ID fractalID, welches
     * später mittels 'createFractal' erzeugt werden kann.
     *
     * @tparam T        Das Fraktal, welches instanziiert werden
     *                  soll, beim Ändern durch den Benutzer.
     * @param fractalID Die ID, welcher das zu erzeugende
     *                  Fratal zugewiesen werden soll.
     *
     * @see createFractal
     */
    template <typename T>
    void registerFractal(Fractals::ID fractalID);

  private:
    /**
     * Das Canvas, auf das Fraktale gezeichnet werden.
     */
    Canvas canvas;
    /**
     * Ein Pointer (unique_ptr), welcher auf das aktuelle Fraktal zeigt.
     */
    Fractal::Ptr currentFractal;
    /**
     * Der aktuelle Zoomlevel.
     */
    bool isDragging;
    QPoint lastDragPos;
    QFormLayout* settings;
    std::map<Fractals::ID, std::function<Fractal::Ptr()>> fractalFactory;

    QWidget* createSettings();

    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resetFractal();
};

template<typename T>
void FractalWindow::registerFractal(Fractals::ID fractalID) {
  fractalFactory[fractalID] = [this] () {
    return Fractal::Ptr(new T(canvas.size().width(), canvas.size().height()));
  };
}

#endif //FRAKTALE_FRACTALWINDOW_HPP
