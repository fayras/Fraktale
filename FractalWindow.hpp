#ifndef FRAKTALE_FRACTALWINDOW_HPP
#define FRAKTALE_FRACTALWINDOW_HPP

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include "Fractals/Fractal.hpp"
#include "Canvas.hpp"
#include "Fractals/Identifiers.hpp"

class FractalWindow : public QDialog {
  Q_OBJECT

  public slots:
    void draw();
    void createFractal(Fractals::ID fractalID);

  public:
    explicit FractalWindow(QWidget *parent = 0);
    void zoom(QPoint center, double factor);
    template <typename T>
    void registerFractal(Fractals::ID fractalID);

  private:
    Canvas canvas;
    Fractal::Ptr currentFractal;
    double scale;
    bool isDragging;
    QPoint lastDragPos;
    QFormLayout* settings;
    std::map<Fractals::ID, std::function<Fractal::Ptr()>> fractalFactory;

    QWidget* createSettings();

    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resetFractal();
};

template<typename T>
void FractalWindow::registerFractal(Fractals::ID fractalID) {
  fractalFactory[fractalID] = [this] () {
    return Fractal::Ptr(new T(canvas.size().width(), canvas.size().height()));
  };
}


#endif //FRAKTALE_FRACTALWINDOW_HPP
