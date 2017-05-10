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
#include "Fractals/Julia.hpp"
#include "Fractals/KochCurve.hpp"

FractalWindow::FractalWindow(QWidget *parent)
  : QDialog(parent), isDragging(false), currentFractal(new EmptyFractal()), fractals(new QComboBox)
{
  // Verbindet die zwei Signale, so dass das Fraktal auf die Zeichenfläche
  // gezeichnet werden kann, sobald das Fraktal etwas zum Zeichnen hat.
  connect(currentFractal.get(), &Fractal::drawSignal, this, &FractalWindow::draw);

  // Registriere die Fraktale, welche in der Applikation benutzt wernde.
  registerFractal<EmptyFractal>(Fractals::ID::EMPTY_FRACTAL, "Keine Auswahl");
  registerFractal<Mandelbrot>(Fractals::ID::MANDELBROT, "Mandelbrot-Menge");
  registerFractal<Julia>(Fractals::ID::JULIA, "Julia-Menge");
  registerFractal<KochCurve>(Fractals::ID::KOCH_CURVE, "Koch-Schneeflocke");

  // Ein horizontales Layout, bietet Platz für die Zeichenfläche
  // und die Einstellungen. Dabei nimmt die Zeichenfläche zwei
  // Drittel des Platzes und die Einstellungen 1/3 ein.
  QHBoxLayout *mainLayout = new QHBoxLayout;
  QSizePolicy canvasSize(QSizePolicy::Preferred, QSizePolicy::Preferred);
  canvasSize.setHorizontalStretch(2);
  canvas.setSizePolicy(canvasSize);
  // Fügt einmal die Zeichenfläche und die Gruppe
  // mit den Einstellungen zum Layout hinzu.
  mainLayout->addWidget(&canvas);
  mainLayout->addWidget(createSettings());

  setLayout(mainLayout);
  setWindowTitle("Fraktale");
  // Setzt das Fenster auf die angegebene Größe. Zusätzlich
  // werden dadurch alle Zeichen-Signale ausgelöst, so
  // dass das Fraktal gezeichnet wird.
  resize(800, 400);
}

QWidget* FractalWindow::createSettings() {
  QGroupBox* settingsGroup = new QGroupBox("Einstellungen");
  QSizePolicy settingsSize(QSizePolicy::Preferred, QSizePolicy::Preferred);
  settingsSize.setHorizontalStretch(1);
  settingsGroup->setSizePolicy(settingsSize);
  QFormLayout* formLayout = new QFormLayout;

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
  connect(menu->addAction("Fraktal importieren"), &QAction::triggered, this, &FractalWindow::importFractal);
  menu->addSeparator();
  connect(menu->addAction("Fraktal exportieren"), &QAction::triggered, [=] () {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "./fractal.frac", "Fractal (*.frac)");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly|QFile::Truncate)) {
      QMessageBox::information(this, tr("Unable to open file"), file.errorString());
      return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);
    out << fractals->currentData().value<unsigned>() << *currentFractal;
  });
  connect(menu->addAction("Als Bild exportieren"), &QAction::triggered, [this] () {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "./fractal.png", "Images (*.png *.xpm *.jpg)");
    if(fileName.isEmpty()) {
      return;
    }
    this->currentFractal->getImage().save(fileName);
  });
  btExport->setMenu(menu);
  buttons->addItem(new QSpacerItem(200, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
  buttons->addWidget(btExport);
  QPushButton* info = new QPushButton("Hilfe");
  connect(info, &QPushButton::clicked, [this] () {
    QFile about(":/about");
    about.open(QIODevice::ReadOnly | QIODevice::Text);
    QMessageBox aboutBox(QMessageBox::NoIcon, "About", about.readAll(), QMessageBox::Close, nullptr, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    aboutBox.exec();
  });
  buttons->addWidget(info);

  QPushButton* exit = new QPushButton("Beenden");
  connect(exit, &QPushButton::clicked, this, &FractalWindow::close);
  buttons->addWidget(exit);
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

void FractalWindow::importFractal() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", "./", "Fractal (*.frac)");
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(this, tr("Unable to open file"), file.errorString());
    return;
  }
  QDataStream in(&file);
  unsigned id;
  in >> id;
  createFractal(static_cast<Fractals::ID>(id));
  int index = fractals->findData(static_cast<Fractals::ID>(id));
  if ( index != -1 ) { // -1 for not found
    fractals->setCurrentIndex(index);
  }
  in >> *currentFractal;
  currentFractal->update();
}
