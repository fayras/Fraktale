#ifndef FRAKTALE_FRACTAL_HPP
#define FRAKTALE_FRACTAL_HPP

#include <vector>
#include <QObject>
#include <QtCore/QPoint>
#include <QtGui/QRgb>
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include <memory>
#include "../Canvas.hpp"
#include "../ColorModes/ColorMode.hpp"
#include "../ColorModes/Identifiers.hpp"

class Fractal : public QObject {
    Q_OBJECT

  signals:
    void drawSignal();

  public slots:
    void setMaxIterations(unsigned iterations);
    void setColorMode(Colors::ID colorsID);

  public:
    typedef std::unique_ptr<Fractal> Ptr;

    Fractal(int width, int height);

    void update(QPoint center);
    void update(double factor);
    void update(QPoint center, double factor);
    virtual void update() = 0;
    virtual void draw(Canvas& target);
    virtual void resize(int pWidth, int pHeight);
    virtual std::map<QString, QWidget*> getSettings();
    template <typename T>
    void registerColorMode(Colors::ID colorID);

  protected:
    QImage image;
    int width;
    int height;
    ColorMode::Ptr colormap;
    unsigned maxIterations;
    QPoint fractalCenter;
    double scale;
    std::map<Colors::ID, std::function<ColorMode::Ptr()>> colorsFactory;
};

template<typename T>
void Fractal::registerColorMode(Colors::ID colorID) {
  colorsFactory[colorID] = [this] () {
    return ColorMode::Ptr(new T());
  };
}

#endif //FRAKTALE_FRACTAL_HPP
