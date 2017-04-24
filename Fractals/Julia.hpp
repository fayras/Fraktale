#ifndef FRAKTALE_JULIA_HPP
#define FRAKTALE_JULIA_HPP

#include "Mandelbrot.hpp"

class JuliaRenderTask;

class Julia : public Mandelbrot {
  public:
    Julia(int width, int height);
    void update() override;

  protected:
    QDataStream &print(QDataStream &os) const override;
    QDataStream &read(QDataStream &os) override;

    void createWorkers();

  private:
    double rPart;
    double iPart;
    QRectF bounds;
    std::vector<JuliaRenderTask*> workers;
};


#endif //FRAKTALE_JULIA_HPP
