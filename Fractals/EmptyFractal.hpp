#ifndef FRAKTALE_EMPTYFRACTAL_HPP
#define FRAKTALE_EMPTYFRACTAL_HPP

#include "Fractal.hpp"

class EmptyFractal : public Fractal {
  public:
    EmptyFractal(int width = 1, int height = 1);
    void update() override;
    void draw(Canvas& target) override;

    std::map<QString, QWidget *> getSettings() override;
};

#endif //FRAKTALE_EMPTYFRACTAL_HPP
