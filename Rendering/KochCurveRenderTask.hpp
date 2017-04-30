#ifndef FRAKTALE_KOCHCURVERENDERTASK_HPP
#define FRAKTALE_KOCHCURVERENDERTASK_HPP

#include <QThread>
#include "../Fractals/KochCurve.hpp"

/**
 * Berechnet die einzelnen Punkte der Koch-Schneeflocke in einem Thread.
 */
class KochCurveRenderTask : public QThread  {
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
     * Rendert die Mandelbrot Menge in einem bestimmten Bereich.
     *
     * @param pRect          Der Bereich in Pixeln, welcher berechnet werden soll.
     * @param pBounds        Der Bereich der Mandelbrot-Menge, welcher berechnet werden soll.
     *                       Die Mandelbrot-Menge hat normalerweise einen Bereich
     *                       (-2.5, -1, 3.5, 2), wobei 3.5 und 2 jeweils die
     *                       Breite und die Höhe des Bereichs darstellen.
     * @param pMaxIterations Die maximale Anzahl von Iterationen pro Pixel.
     */
    void render(int depth, KochCurve::Line line);

  private:
    void calculateCurve(int depth, KochCurve::Line line, std::vector<QPointF>& points);

  private:
    int maxIterations;
    bool restart;
    QRectF rect;
    KochCurve::Line firstLine;
};


#endif //FRAKTALE_KOCHCURVERENDERTASK_HPP
