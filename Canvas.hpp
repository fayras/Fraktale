#ifndef FRAKTALE_CANVAS_HPP
#define FRAKTALE_CANVAS_HPP

#include <QtWidgets/QWidget>

class Canvas : public QWidget {
  public:
    void draw(QImage& image);

  protected:
    void paintEvent(QPaintEvent *event) override;

  private:
    QPixmap pixmap;
};


#endif //FRAKTALE_CANVAS_HPP
