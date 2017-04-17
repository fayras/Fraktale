#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QWheelEvent>
#include <cassert>
#include "FractalWindow.hpp"
#include "Fractals/Identifiers.hpp"
#include "Fractals/Mandelbrot.hpp"
#include "Fractals/EmptyFractal.hpp"

FractalWindow::FractalWindow(QWidget *parent)
  : QDialog(parent), isDragging(false), currentFractal(new EmptyFractal())
{
  connect(currentFractal.get(), &Fractal::drawSignal, this, &FractalWindow::draw);

  QHBoxLayout *mainLayout = new QHBoxLayout;
  QSizePolicy canvasSize(QSizePolicy::Preferred, QSizePolicy::Preferred);
  canvasSize.setHorizontalStretch(2);
  canvas.setSizePolicy(canvasSize);
  mainLayout->addWidget(&canvas);
  mainLayout->addWidget(createSettings());

  setLayout(mainLayout);
  setWindowTitle("Fraktale");
  resize(800, 400);
}

QWidget* FractalWindow::createSettings() {
  QGroupBox* settingsGroup = new QGroupBox("Einstellungen");
  QSizePolicy settingsSize(QSizePolicy::Preferred, QSizePolicy::Preferred);
  settingsSize.setHorizontalStretch(1);
  settingsGroup->setSizePolicy(settingsSize);
  QFormLayout* formLayout = new QFormLayout;

  QComboBox* fractals = new QComboBox;

  fractals->addItem("Keine Auswahl", Fractals::ID::EMPTY_FRACTAL);
  registerFractal<EmptyFractal>(Fractals::ID::EMPTY_FRACTAL);

  fractals->addItem("Mandelbrot", Fractals::ID::MANDELBROT);
  registerFractal<Mandelbrot>(Fractals::ID::MANDELBROT);

  formLayout->addRow("Fraktal", fractals);
  connect(fractals, &QComboBox::currentTextChanged, [=](const QString& text){
    this->createFractal(fractals->currentData().value<Fractals::ID>());
  });
  settings = new QFormLayout;
  formLayout->addRow(settings);
  settingsGroup->setLayout(formLayout);
  return settingsGroup;
}

void FractalWindow::resizeEvent(QResizeEvent *event) {
  currentFractal->resize(canvas.size().width(), canvas.size().height());
  QPoint fractalCenter;
  fractalCenter.setX(canvas.size().width() / 2);
  fractalCenter.setY(canvas.size().height() / 2);
  currentFractal->update(fractalCenter);
}

void FractalWindow::moveFractal(QPoint center, double factor) {
  currentFractal->update(center, factor);
}

void FractalWindow::draw() {
  currentFractal->draw(canvas);
}

void FractalWindow::wheelEvent(QWheelEvent *event) {
  if(canvas.underMouse()) {
    QPoint fractalCenter = canvas.mapFromGlobal(QCursor::pos());
    int numDegrees = event->delta() / 8;
    double steps = numDegrees / 15.0;
    if(steps < 0) {
      moveFractal(fractalCenter, -steps * 2);
    } else {
      moveFractal(fractalCenter, 1 / (steps * 2));
    }
  }
}

void FractalWindow::mousePressEvent(QMouseEvent *event) {
  if(canvas.underMouse() && event->buttons() & Qt::LeftButton) {
    isDragging = true;
    lastDragPos = event->pos();
  }
}

void FractalWindow::mouseReleaseEvent(QMouseEvent *event) {
  isDragging = false;
  lastDragPos = QPoint();
}

void FractalWindow::mouseMoveEvent(QMouseEvent *event) {
  if(isDragging) {
    QPoint offset = event->pos() - lastDragPos;
    QPoint fractalCenter;
    fractalCenter.setX(canvas.width() / 2 - offset.x());
    fractalCenter.setY(canvas.height() / 2 - offset.y());
    lastDragPos = event->pos();
    currentFractal->update(fractalCenter);
  }
}

void FractalWindow::createFractal(Fractals::ID fractalID) {
  auto found = fractalFactory.find(fractalID);
  assert(found != fractalFactory.end());

  currentFractal = std::move(found->second());
  connect(currentFractal.get(), &Fractal::drawSignal, this, &FractalWindow::draw);

  while(settings->rowCount() > 0) {
    settings->removeRow(0);
  }
  for(auto const &setting : currentFractal->getSettings()) {
    settings->addRow(setting.first, setting.second);
  }
  currentFractal->update();
}
