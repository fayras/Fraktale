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

  private slots:
    /**
     * Slot:
     * Überprüft, ob alle Render-Thread fertig sind und löst
     * letztendlich das Signal "finishedRendering" aus.
     */
    void checkThreadStatus();

  public:
    /**
     * Erzeugt eine neue Instanz des Mandebrot-Fraktals.
     *
     * @param width Breite der Fläche, wie viele Pixel das Fraktal anzeigen soll.
     * @param height Höhe der Fläche, wie viele Pixel das Fraktal anzeigen soll.
     */
    Mandelbrot(int width, int height);
    void update() override;
    void translate(QPointF offset) override;
    void scale(double factor) override;

  protected:
    QDataStream &print(QDataStream &os) const override;
    QDataStream &read(QDataStream &os) override;
    /**
     * Erstellt die einzelnen Thread-Klasse, welche das Fraktal
     * später berechnen sollen.
     */
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
