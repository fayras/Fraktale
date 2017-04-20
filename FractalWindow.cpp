#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QWheelEvent>
#include <cassert>
#include <QMenu>
#include <QDebug>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
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

  fractals->addItem("Keine Auswahl", QVariant::fromValue(Fractals::ID::EMPTY_FRACTAL));
  registerFractal<EmptyFractal>(Fractals::ID::EMPTY_FRACTAL);

  fractals->addItem("Mandelbrot", QVariant::fromValue(Fractals::ID::MANDELBROT));
  registerFractal<Mandelbrot>(Fractals::ID::MANDELBROT);

  formLayout->addRow("Fraktal", fractals);
  connect(fractals, &QComboBox::currentTextChanged, [=](const QString& text) {
    this->createFractal(fractals->currentData().value<Fractals::ID>());
  });
  settings = new QFormLayout;
  formLayout->addRow(settings);
  formLayout->addItem(new QSpacerItem(0, 65000, QSizePolicy::Expanding, QSizePolicy::Expanding));

  QHBoxLayout* buttons = new QHBoxLayout();
  QPushButton* btExport = new QPushButton("Menü");
  QMenu* menu = new QMenu();
  connect(menu->addAction("Fraktal importieren"), &QAction::triggered, [=] () {
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open File"),
        "./",
        tr("Fractal (*.frac)")
    );
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::information(this, tr("Unable to open file"),
                               file.errorString());
      return;
    }
    QDataStream in(&file);
    unsigned id;
    in >> id;
    createFractal(static_cast<Fractals::ID>(id));
    in >> *currentFractal;
  });
  menu->addSeparator();
  connect(menu->addAction("Fraktal exportieren"), &QAction::triggered, [=] () {
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save File"),
        "./fractal.frac",
        tr("Fractal (*.frac)")
    );
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly|QFile::Truncate)) {
      QMessageBox::information(this, tr("Unable to open file"),
                               file.errorString());
      return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);
    out << fractals->currentData().value<unsigned>() << *currentFractal;
  });
  connect(menu->addAction("Als Bild exportieren"), &QAction::triggered, [this] () {
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save File"),
        "./fractal.png",
        tr("Images (*.png *.xpm *.jpg)")
    );
    if(fileName.isEmpty()) {
      return;
    }
    this->currentFractal->getImage().save(fileName);
  });
  btExport->setMenu(menu);
  buttons->addWidget(btExport);
  buttons->addItem(new QSpacerItem(200, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
  QPushButton* info = new QPushButton("About");
  connect(info, &QPushButton::clicked, [this] () {
    QMessageBox::about(this, "About", "ABOUT PAGE...");
  });
  buttons->addWidget(info);
  formLayout->addRow(buttons);

  settingsGroup->setLayout(formLayout);
  return settingsGroup;
}

void FractalWindow::resizeEvent(QResizeEvent *event) {
  currentFractal->resize(canvas.size().width(), canvas.size().height());
  currentFractal->update();
}

void FractalWindow::draw() {
  currentFractal->draw(canvas);
}

void FractalWindow::wheelEvent(QWheelEvent *event) {
  if(canvas.underMouse()) {
    QPointF mousePos = canvas.mapFromGlobal(QCursor::pos());
    QPointF offset = QPointF(canvas.width() / 2.0, canvas.height() / 2.0) - mousePos;
    currentFractal->translate(offset);
    double steps = event->delta() / 8 / 15.0;
    if(steps < 0) {
      currentFractal->scale(-1.0 / (steps * 2.0));
    } else {
      currentFractal->scale(steps * 2.0);
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
    lastDragPos = event->pos();
    currentFractal->translate(offset);
  }
}

void FractalWindow::createFractal(Fractals::ID fractalID) {
  auto found = fractalFactory.find(fractalID);
  assert(found != fractalFactory.end());

  currentFractal = std::move(found->second());
  connect(currentFractal.get(), &Fractal::drawSignal, this, &FractalWindow::draw);

  QLayoutItem* child;
  while(settings->count() > 0 && (child = settings->takeAt(0)) != 0) {
    delete child->widget();
  }
  for(auto const &setting : currentFractal->getSettings()) {
    settings->addRow(setting.first, setting.second);
  }
  currentFractal->update();
}
