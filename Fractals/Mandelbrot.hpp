#ifndef FRAKTALE_MANDELBROT_HPP
#define FRAKTALE_MANDELBROT_HPP

#include "Fractal.hpp"
#include "FractalPixelIteration.hpp"
#include "../Rendering/MandelbrotRenderTask.hpp"
#include <vector>

/**
 * Representiert das gesamte Mandelbrot-Fraktal.
 */
class Mandelbrot : public Fractal {
    Q_OBJECT

  public slots:
    /**
     * Slot:
     * Aktualisiert Pixel des Mandelbrot-Fraktals.
     *
     * @slot
     * @param pixelIterations Ein Array mit den berechneten Pixeln.
     */
    void updatePixels(std::vector<FractalPixelIteration> pixelIterations);

  public:
    /**
     * Erzeugt eine neue Instanz des Mandebrot-Fraktals.
     *
     * @param width Breite der Fläche, wie viele Pixel das Fraktal anzeigen soll.
     * @param height Höhe der Fläche, wie viele Pixel das Fraktal anzeigen soll.
     */
    Mandelbrot(int width, int height);
    /**
     * Aktualisiert das Fraktal.
     */
    void update() override;
    /**
     * Verschiebt das Fraktal um einen bestimmten Offset.
     *
     * @param offset Koordinaten, um wie viele Pixel das Fraktal verschoben werden soll.
     */
    void translate(QPointF offset) override;
    /**
     * Zoomt das Fraktal hinei oder heraus.
     *
     * @param factor Der Faktor, um den das Fraktal skaliert werden soll.
     *               2 bedeutet hereinzoomen, 0.5 bedeutet herauszoomen.
     */
    void scale(double factor) override;

  protected:
    QDataStream &print(QDataStream &os) const override;
    QDataStream &read(QDataStream &os) override;
    virtual void createWorkers();

    /**
     * Der aktuelle Ausschnitt, welcher angezeigt wird.
     */
    QRectF bounds;
    /**
     * Die RenderTasks, welche die Pixel berechnen sollen.
     */
    std::vector<std::unique_ptr<MandelbrotRenderTask>> workers;
};

#endif //FRAKTALE_MANDELBROT_HPP
