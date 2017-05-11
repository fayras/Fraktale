#ifndef FRAKTALE_RENDERTASK_HPP
#define FRAKTALE_RENDERTASK_HPP

#include <QtCore/QThread>

class RenderTask : public QThread {
  Q_OBJECT

  signals:
    void done();

  public:
    explicit RenderTask(QObject *parent = 0);
    ~RenderTask();

  protected:
    double map(double n, double start1, double stop1, double start2, double stop2);

    bool restart;
};


#endif //FRAKTALE_RENDERTASK_HPP
