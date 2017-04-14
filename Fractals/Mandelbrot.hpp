#ifndef FRAKTALE_MANDELBROT_HPP
#define FRAKTALE_MANDELBROT_HPP

#include "Fractal.hpp"
#include "../Rendering/MandelbrotRenderTask.hpp"
#include <vector>

class Mandelbrot : public Fractal {
    Q_OBJECT

  public slots:
    void updatePixels(std::vector<MandelbrotRenderTask::PixelIteration> pixelIterations);

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
