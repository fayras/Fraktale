#ifndef FRAKTALE_KOCHCURVE_HPP
#define FRAKTALE_KOCHCURVE_HPP

#include "Fractal.hpp"
#include <QPointF>
#include <cmath>
#include <vector>

class KochCurveRenderTask;

/**
 * Konstante für die Höhe der eingefügten Dreiecke.
 */
const double H = 1.0 / 3.0;

/**
 * Das Koch-Schneeflocke-Fraktal.
 */
class KochCurve : public Fractal {
  public slots:
    /**
     * Slot:
     * Zeichnet das Fraktal basierend darauf, welche Punkte berechnet wurden.
     * Dabei wird davon ausgegangen, dass im Array 'points' immer zwei auf-
     * einander folgende Punkte eine Linie darstellen.
     *
     * @slot
     * @param points Array mit den berechneten Punkten aller Linien.
     */
    void updatePixels(std::vector<QPointF> points);

  public:
    /**
     * Stellt eine Linie dar, welche Punkte für die nächste
     * Iteration einer Koch-Kurve berechnen kann.
     */
    struct Line {
      /**
       * Liefert eine neue Instanz eine Linie. Dabei wird automatisch die
       * Differenz zwischen der beiden Endpunkte berechnet.
       *
       * @param start Anfangspunkt der Linie.
       * @param end Endpunkt der Linie.
       */
      Line(QPointF start = QPointF(), QPointF end = QPointF()) : start(start), end(end) {
        diff = QPointF(end.x() - start.x(), -1 * (end.y() - start.y()));
      }

      /**
       * Anfangspunkt der Linie.
       */
      QPointF start;
      /**
       * Endpunkt der Linie.
       */
      QPointF end;
      /**
       * Differenz zwischen den beiden Endpunkten.
       */
      QPointF diff;

      /**
       * Berechnet den Punkt B laut <a href="https://de.wikipedia.org/wiki/Koch-Kurve#/media/File:Kochkurve.jpg">diesem Diagramm</a>.
       *
       * @return
       */
      QPointF point2() {
        return QPointF(start.x() + diff.x() / 3.0, start.y() - diff.y() / 3.0);
      }
      /**
       * Berechnet den Punkt C laut <a href="https://de.wikipedia.org/wiki/Koch-Kurve#/media/File:Kochkurve.jpg">diesem Diagramm</a>.
       *
       * @return
       */
      QPointF point3() {
        return QPointF(start.x() + diff.x() / 2.0 - H * diff.y() / 1.4, start.y() - diff.y() / 2.0 - H * diff.x());
      }
      /**
       * Berechnet den Punkt D laut <a href="https://de.wikipedia.org/wiki/Koch-Kurve#/media/File:Kochkurve.jpg">diesem Diagramm</a>.
       *
       * @return
       */
      QPointF point4() {
        return QPointF(start.x() + 2.0 * diff.x() / 3.0, start.y() - 2.0 * diff.y() / 3.0);
      }
    };
  public:
    /**
     * Erstellt eine neue Instanz des Frajtals.
     *
     * @param width Breite des sichtabaren Bereichs.
     * @param height Höhe des sichtabaren Bereichs.
     */
    KochCurve(int width, int height);
    void update() override;
    void translate(QPointF offset) override;
    void scale(double factor) override;
    /**
     * Liefert die Einstellungen des Fraktals, welche verändert werden können.
     *
     * Dazu gehören das Ändern der Iterationen.
     *
     * @return Eine Gruppe von QWidgets, welche diverse Einstellungen am Fraktal verändern können.
     */
    std::map<QString, QWidget *> getSettings() override;

  protected:
    QDataStream &print(QDataStream &os) const override;
    QDataStream &read(QDataStream &os) override;

  private:
    void curve(int depth, Line line);

    QPointF _offset;
    double _scale;
    std::vector<QPointF> points;
    std::vector<std::unique_ptr<KochCurveRenderTask>> workers;
};


#endif //FRAKTALE_KOCHCURVE_HPP
