#ifndef FRAKTALE_KOCHCURVERENDERTASK_HPP
#define FRAKTALE_KOCHCURVERENDERTASK_HPP

#include "RenderTask.hpp"
#include "../Fractals/KochCurve.hpp"

/**
 * Berechnet die einzelnen Punkte der Koch-Schneeflocke in einem Thread.
 */
class KochCurveRenderTask : public RenderTask  {
  Q_OBJECT

  public:
    /**
     * Ezeugt eine neue Instanz des KochCurveRenderTasks.
     *
     * @param parent Das Objekt, welches den Besitz des Tasks übernimmt.
     */
    KochCurveRenderTask(QObject *parent = 0);
    ~KochCurveRenderTask();

  signals:
    /**
     * Signal:
     * Wird ausgelöst, sobald genügend Pixel zum Zeichnen berechnet wurden.
     *
     * @signal
     * @param pixelIterations Ein std::vector mit den iterierten Pixeln.
     * @see FractalPixelIteration
     */
    void rendered(std::vector<QPointF> pixelIterations);

  public:
    /**
     * Die Methode, welche in einem eigenen
     * Thread ausgeführt werden sollte.
     *
     * @see http://doc.qt.io/qt-5.8/qthread.html#run
     */
    void run() override;
    /**
     * Berechnet die Pixel des Fraktals.
     *
     * Löst das Signal KochCurveRenderTask::rendered aus.
     *
     * @param depth Anzahl in Iterationen.
     * @param line Die Angfangslinie.
     */
    void render(int depth, KochCurve::Line line);

  private:
    void calculateCurve(int depth, KochCurve::Line line, std::vector<QPointF>& points);

  private:
    int maxIterations;
    QRectF rect;
    KochCurve::Line firstLine;
};


#endif //FRAKTALE_KOCHCURVERENDERTASK_HPP
