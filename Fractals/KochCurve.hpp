#ifndef FRAKTALE_KOCHCURVE_HPP
#define FRAKTALE_KOCHCURVE_HPP

#include "Fractal.hpp"
#include <QPointF>
#include <cmath>
#include <vector>

class KochCurveRenderTask;

const double H = 1.0 / 3.0;

class KochCurve : public Fractal {
  public slots:
    void updatePixels(std::vector<QPointF> points);

  public:
    struct KochLine {
      KochLine(QPointF start = QPointF(), QPointF end = QPointF()) : start(start), end(end) {
        diff = QPointF(end.x() - start.x(), -1 * (end.y() - start.y()));
      }

      QPointF start;
      QPointF end;
      QPointF diff;

      QPointF point2() {
        return QPointF(start.x() + diff.x() / 3.0, start.y() - diff.y() / 3.0);
      }
      QPointF point3() {
        return QPointF(start.x() + diff.x() / 2.0 - H * diff.y() / 1.4, start.y() - diff.y() / 2.0 - H * diff.x());
      }
      QPointF point4() {
        return QPointF(start.x() + 2.0 * diff.x() / 3.0, start.y() - 2.0 * diff.y() / 3.0);
      }
    };
  public:
    KochCurve(int width, int height);
    void update() override;
    void translate(QPointF offset) override;
    void scale(double factor) override;
    std::map<QString, QWidget *> getSettings() override;

  private:
    void curve(int depth, KochLine line);

    QPointF _offset;
    double _scale;
    std::vector<QPointF> points;
    std::vector<std::unique_ptr<KochCurveRenderTask>> workers;
};


#endif //FRAKTALE_KOCHCURVE_HPP
