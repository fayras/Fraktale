#ifndef FRAKTALE_JULIA_HPP
#define FRAKTALE_JULIA_HPP

#include "Mandelbrot.hpp"

class JuliaRenderTask;

/**
 * Representiert die Julia-Menge.
 */
class Julia : public Mandelbrot {
    Q_OBJECT

  signals:
    /**
     * Signal:
     * Wird ausgelöst, wenn sich der reelle Part ändert.
     *
     * @signal
     * @param rP Der reelle Part des Fraktals.
     */
    void rPartChanged(double rP);
    /**
     * Signal:
     * Wird ausgelöst, wenn sich der imaginäre Part ändert.
     *
     * @signal
     * @param rP Der imaginäre Part des Fraktals.
     */
    void iPartChanged(double iP);

  private slots:
    void checkThreadStatus();

  public:
    /**
     * Erzeugt eine neue Instanz des Julia-Fraktals.
     *
     * @param width Breite der Fläche, wie viele Pixel das Fraktal anzeigen soll.
     * @param height Höhe der Fläche, wie viele Pixel das Fraktal anzeigen soll.
     */
    Julia(int width, int height);
    void update() override;
    virtual std::map<QString, QWidget*> getSettings();
    QString getOverlayInformation() const override;

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
    /**
     * Die RenderTasks, welche die Pixel berechnen sollen.
     */
    std::vector<std::unique_ptr<JuliaRenderTask>> workers;
};


#endif //FRAKTALE_JULIA_HPP
