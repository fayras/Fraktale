#ifndef FRAKTALE_MANDELBROTRENDERTASK_HPP
#define FRAKTALE_MANDELBROTRENDERTASK_HPP

#include <QThread>
#include <QtCore/QRect>

struct FractalPixelIteration;

/**
 * Rendert einzelne Pixel der Mandelbrot-Menge in einem Thread.
 */
class MandelbrotRenderTask : public QThread {
    Q_OBJECT

  public:
    MandelbrotRenderTask(QObject *parent = 0);
    ~MandelbrotRenderTask();

  signals:
    /**
     * Signal:
     * Wird ausgelöst, sobald genügend Pixel zum Zeichnen berechnet wurden.
     *
     * @signal
     * @param pixelIterations Ein std::vector mit den iterierten Pixeln.
     * @see FractalPixelIteration
     */
    void rendered(std::vector<FractalPixelIteration> pixelIterations);

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
    void render(QRect pRect, QRectF pBounds, int pMaxIterations);

  private:
    QRect rect;
    QRectF fractalBounds;
    int maxIterations;
    bool restart;
};

#endif //FRAKTALE_MANDELBROTRENDERTASK_HPP
