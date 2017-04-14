#ifndef FRAKTALE_MANDELBROTRENDERTASK_HPP
#define FRAKTALE_MANDELBROTRENDERTASK_HPP

#include <QThread>
#include <QtCore/QRect>

class MandelbrotRenderTask : public QThread {
    Q_OBJECT

  public:
    MandelbrotRenderTask(QObject *parent = 0);
    ~MandelbrotRenderTask();

  public:
    struct PixelIteration {
      PixelIteration(int pX, int pY, int pIt, double pXV, double pYV) {
        x = pX; y = pY; iterations = pIt; xValue = pXV; yValue = pYV;
      }

      int x;
      int y;
      int iterations;
      double xValue;
      double yValue;
    };

  signals:
    void rendered(std::vector<MandelbrotRenderTask::PixelIteration> pixelIterations);

  public:
    void run() override;
    void render(QRect pRect, QRectF pBounds, int pMaxIterations);

  private:
    QRect rect;
    QRectF fractalBounds;
    int maxIterations;
    bool restart;
};

Q_DECLARE_METATYPE(std::vector<MandelbrotRenderTask::PixelIteration>);

#endif //FRAKTALE_MANDELBROTRENDERTASK_HPP
