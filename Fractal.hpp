#ifndef FRAKTALE_FRACTAL_HPP
#define FRAKTALE_FRACTAL_HPP

#include <vector>
#include <QtCore/QPoint>

class Fractal {
  public:
    Fractal(unsigned width, unsigned height);

    void resize(unsigned pWidth, unsigned pHeight);

    virtual void calculate(QPoint center, float zoom) = 0;
    virtual void draw() = 0;

  private:
    unsigned width;
    unsigned height;
    std::vector<std::vector<int> > pixels;
};


#endif //FRAKTALE_FRACTAL_HPP
