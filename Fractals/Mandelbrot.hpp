#ifndef FRAKTALE_MANDELBROT_HPP
#define FRAKTALE_MANDELBROT_HPP

#include "Fractal.hpp"
#include "FractalPixelIteration.hpp"
#include <vector>

class MandelbrotRenderTask;

class Mandelbrot : public Fractal {
    Q_OBJECT

  public slots:
    void updatePixels(std::vector<FractalPixelIteration> pixelIterations);

  public:
    Mandelbrot(int width, int height);
    void update() override;
    void translate(QPointF offset) override;
    void scale(double factor) override;

  protected:
    QDataStream &print(QDataStream &os) const override;
    QDataStream &read(QDataStream &os) override;
    virtual void createWorkers();

    QRectF bounds;
    std::vector<MandelbrotRenderTask*> workers;
};

#endif //FRAKTALE_MANDELBROT_HPP
