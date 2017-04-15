#ifndef FRAKTALE_FRACTALPIXELITERATION_HPP
#define FRAKTALE_FRACTALPIXELITERATION_HPP

#include <vector>
#include <QtCore/QMetaType>

struct FractalPixelIteration {
  FractalPixelIteration(int pX, int pY, int pIt, int pMaxIt, double pXV, double pYV) {
    x = pX; y = pY; iterations = pIt; maxIterations = pIt; xValue = pXV; yValue = pYV;
  }

  int x;
  int y;
  int iterations;
  int maxIterations;
  double xValue;
  double yValue;
};

Q_DECLARE_METATYPE(std::vector<FractalPixelIteration>);

#endif //FRAKTALE_FRACTALPIXELITERATION_HPP
