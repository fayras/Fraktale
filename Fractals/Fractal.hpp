#ifndef FRAKTALE_FRACTAL_HPP
#define FRAKTALE_FRACTAL_HPP

#include <QObject>
#include <QPoint>
#include <QImage>
#include <memory>
#include "../ColorModes/ColorMode.hpp"

namespace Colors {
  enum ID : unsigned;
}

class Canvas;

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
    template <typename T, typename... Args>
    void registerColorMode(Colors::ID colorID, Args&&... args);

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

template<typename T, typename... Args>
void Fractal::registerColorMode(Colors::ID colorID, Args&&... args) {
  colorsFactory[colorID] = [=] () -> ColorMode::Ptr {
    return ColorMode::Ptr(new T(args...));
  };
}

#endif //FRAKTALE_FRACTAL_HPP
