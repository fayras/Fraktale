#include "Fractal.hpp"

Fractal::Fractal(unsigned width, unsigned height)
  : width(width), height(height)
{
  resize(width, height);
}

void Fractal::resize(unsigned pWidth, unsigned pHeight) {
  width = pWidth;
  height = pHeight;
  std::vector<int> vec(height, 0);
  pixels = std::vector<std::vector<int> >(width, vec);
}
