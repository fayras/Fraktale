#ifndef FRAKTALE_RENDERTASK_HPP
#define FRAKTALE_RENDERTASK_HPP

#include <QtCore/QThread>

/**
 * Abstrakte Klasse zum Berechnen der einzelnen Fraktale.
 */
class RenderTask : public QThread {
  Q_OBJECT

  public:
    explicit RenderTask(QObject *parent = 0);
    ~RenderTask();

  protected:
    /**
     * Bildet die Zahl n vom Interval [start1, stop1] in das Interval [start2, stop2] ab.
     *
     * @param  n
     * @param  start1
     * @param  stop1
     * @param  start2
     * @param  stop2
     * @return
     */
    double map(double n, double start1, double stop1, double start2, double stop2);

    /**
     * Flag zum einstellen, ob die Berechnung neugestartet werden soll.
     */
    bool restart;
};


#endif //FRAKTALE_RENDERTASK_HPP
