#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtGui/QPainter>
#include <QWheelEvent>
#include <QDebug>
#include <cassert>
#include "FractalWindow.hpp"
#include "Fractals/Mandelbrot.hpp"
#include "Fractals/EmptyFractal.hpp"

FractalWindow::FractalWindow(QWidget *parent)
  : QDialog(parent), isDragging(false), currentFractal(new EmptyFractal()), scale(1)
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

  formLayout->addRow(new QLabel("Fraktal"), fractals);
  //QFrame* line = new QFrame;
  //line->setFrameStyle(QFrame::HLine);
  //line->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
  //formLayout->addRow(line);
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

void FractalWindow::zoom(QPoint center, double factor) {
  scale *= factor;
  currentFractal->update(center, scale);
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
      zoom(fractalCenter, -steps * 2);
    } else {
      zoom(fractalCenter, 1 / (steps * 2));
    }
  }
}

void FractalWindow::mousePressEvent(QMouseEvent *event) {
  //QWidget::mousePressEvent(event);
  if(canvas.underMouse() && event->buttons() & Qt::LeftButton) {
    isDragging = true;
    lastDragPos = event->pos();
  }
}

void FractalWindow::mouseReleaseEvent(QMouseEvent *event) {
  //QWidget::mouseReleaseEvent(event);
  isDragging = false;
  lastDragPos = QPoint();
}

void FractalWindow::mouseMoveEvent(QMouseEvent *event) {
  //QWidget::mouseMoveEvent(event);
  if(isDragging) {
    QPoint offset = event->pos() - lastDragPos;
    QPoint fractalCenter;
    fractalCenter.setX(canvas.width() / 2 - offset.x());
    fractalCenter.setY(canvas.height() / 2 - offset.y());
    lastDragPos = event->pos();
    currentFractal->update(fractalCenter, scale);
  }
}

void FractalWindow::createFractal(Fractals::ID fractalID) {
  resetFractal();

  auto found = fractalFactory.find(fractalID);
  assert(found != fractalFactory.end());

  currentFractal = std::move(found->second());
  connect(currentFractal.get(), &Fractal::drawSignal, this, &FractalWindow::draw);
  QPoint fractalCenter(canvas.size().width() / 2, canvas.size().height() / 2);
  currentFractal->update(fractalCenter, scale);
  for(auto const &setting : currentFractal->getSettings()) {
    settings->addRow(new QLabel(setting.first), setting.second);
  }
}

void FractalWindow::resetFractal() {
  scale = 1;
  while(settings->rowCount() > 0) {
    settings->removeRow(0);
  }
}
