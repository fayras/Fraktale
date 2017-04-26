#ifndef FRAKTALE_JULIA_HPP
#define FRAKTALE_JULIA_HPP

#include "Mandelbrot.hpp"

class JuliaRenderTask;

class Julia : public Mandelbrot {
  public:
    Julia(int width, int height);
    void update() override;
    virtual std::map<QString, QWidget*> getSettings();

  protected:
    QDataStream &print(QDataStream &os) const override;
    QDataStream &read(QDataStream &os) override;

    void createWorkers();

    double rPart;
    double iPart;
    std::vector<std::unique_ptr<JuliaRenderTask>> workers;
};


#endif //FRAKTALE_JULIA_HPP
