#ifndef FRAKTALE_MANDELBROTRENDERTASK_HPP
#define FRAKTALE_MANDELBROTRENDERTASK_HPP

#include <QThread>
#include <QtCore/QRect>
#include "../Fractals/Fractal.hpp"
#include "../Fractals/FractalPixelIteration.hpp"

class MandelbrotRenderTask : public QThread {
    Q_OBJECT

  public:
    MandelbrotRenderTask(QObject *parent = 0);
    ~MandelbrotRenderTask();

  signals:
    void rendered(std::vector<FractalPixelIteration> pixelIterations);

  public:
    void run() override;
    void render(QRect pRect, QRectF pBounds, int pMaxIterations);

  private:
    QRect rect;
    QRectF fractalBounds;
    int maxIterations;
    bool restart;
};

#endif //FRAKTALE_MANDELBROTRENDERTASK_HPP
