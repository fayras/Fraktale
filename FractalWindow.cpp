#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include "FractalWindow.hpp"


FractalWindow::FractalWindow(QWidget *parent)
  : QDialog(parent)
{
  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(canvas = createCanvas());
  mainLayout->addWidget(createSettings());

  setLayout(mainLayout);
  setWindowTitle("Fraktale");
  resize(800, 400);
}

QWidget* FractalWindow::createCanvas() {
  QWidget* canvas = new QWidget;
  QSizePolicy canvasSize(QSizePolicy::Preferred, QSizePolicy::Preferred);
  canvasSize.setHorizontalStretch(2);
  canvas->setSizePolicy(canvasSize);
  return canvas;
}

QWidget* FractalWindow::createSettings() {
  QGroupBox* settingsGroup = new QGroupBox("Einstellungen");
  QSizePolicy settingsSize(QSizePolicy::Preferred, QSizePolicy::Preferred);
  settingsSize.setHorizontalStretch(1);
  settingsGroup->setSizePolicy(settingsSize);
  QFormLayout* formLayout = new QFormLayout;
  formLayout->addRow(new QLabel("Fraktal"), new QComboBox);
  formLayout->addRow(new QLabel("Farbgebung"), new QComboBox);
  formLayout->addRow(new QLabel("Iterationen"), new QSpinBox);
  settingsGroup->setLayout(formLayout);
  return settingsGroup;
}
