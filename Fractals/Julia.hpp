#ifndef FRAKTALE_JULIA_HPP
#define FRAKTALE_JULIA_HPP

#include "Mandelbrot.hpp"

class JuliaRenderTask;

/**
 * Representiert die Julia-Menge.
 */
class Julia : public Mandelbrot {
  public:
    /**
     * Erzeugt eine neue Instanz des Julia-Fraktals.
     *
     * @param width Breite der Fläche, wie viele Pixel das Fraktal anzeigen soll.
     * @param height Höhe der Fläche, wie viele Pixel das Fraktal anzeigen soll.
     */
    Julia(int width, int height);
    void update() override;
    /**
     * Zeichnet das Fraktal auf eine Zeichenfläche.
     *
     * Zusätzlich werden noch der reelle und imaginäre Teil in
     * der oberen rechten Ecke als Information gezeichnet.
     *
     * @param target Zeichenfläche, auf die das Fraktal gezeichnet werden soll.
     */
    void draw(Canvas &target) override;
    virtual std::map<QString, QWidget*> getSettings();

  protected:
    QDataStream &print(QDataStream &os) const override;
    QDataStream &read(QDataStream &os) override;

    void createWorkers();

    /**
     * Der reelle Teil.
     */
    double rPart;
    /**
     * Der imaginäre Teil.
     */
    double iPart;
    std::vector<std::unique_ptr<JuliaRenderTask>> workers;
};


#endif //FRAKTALE_JULIA_HPP
