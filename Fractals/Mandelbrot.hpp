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
    void calculatePreview(QPoint center, double zoom);

  private:
    QRectF bounds;
    std::vector<MandelbrotRenderTask*> workers;

    void calculateNewBounds(QPoint center, double zoom);
    void calculatePixels();
};

#endif //FRAKTALE_MANDELBROT_HPP
