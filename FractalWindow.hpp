#ifndef FRAKTALE_FRACTALWINDOW_HPP
#define FRAKTALE_FRACTALWINDOW_HPP

#include <QtWidgets/QDialog>

class FractalWindow : public QDialog {
  Q_OBJECT

  public:
    explicit FractalWindow(QWidget *parent = 0);

  private:
    QWidget *canvas;
    QPixmap *pixmap;

    QWidget* createCanvas();
    QWidget* createSettings();
};


#endif //FRAKTALE_FRACTALWINDOW_HPP
