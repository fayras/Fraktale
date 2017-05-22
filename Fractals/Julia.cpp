#include "Julia.hpp"
#include "../Rendering/JuliaRenderTask.hpp"
#include <QSlider>
#include <QtGui/QPainter>

Julia::Julia(int width, int height)
    : Mandelbrot(width, height), rPart(-0.8), iPart(0.16)
{
  bounds.setRect(-2, -1.5, 4, 3);
  // Entferne die Workers des Mandelbrot Fraktals...
  Mandelbrot::workers.clear();
  // ... und erstelle die neuen Worker.
  createWorkers();
}

QDataStream &Julia::print(QDataStream &os) const {
  os << maxIterations << static_cast<unsigned>(colormapID) << bounds << rPart << iPart;
	return os;
}

QDataStream &Julia::read(QDataStream &os) {
  unsigned colorID;
  os >> maxIterations >> colorID >> bounds >> rPart >> iPart;
  setColorMode(static_cast<Colors::ID>(colorID));
  emit iterationsChanged(maxIterations);
  emit rPartChanged(rPart);
  emit iPartChanged(iPart);
  update();
  return os;
}

void Julia::createWorkers() {
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    workers.push_back(std::unique_ptr<JuliaRenderTask>(new JuliaRenderTask(this)));
    connect(workers[i].get(), &JuliaRenderTask::rendered, this, &Julia::updatePixels);
    connect(workers[i].get(), &JuliaRenderTask::finished, this, &Julia::checkThreadStatus);
  };
}

void Julia::update() {
  emit startRendering();
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    QRect rect(
        (int) std::round(i * image.width() / QThread::idealThreadCount()),
        0,
        (int) std::ceil((double) image.width() / QThread::idealThreadCount()) + 1,
        image.height()
    );
    QRectF tempBounds(
        bounds.left() + i * bounds.width() / QThread::idealThreadCount(),
        bounds.top(),
        bounds.width() / QThread::idealThreadCount(),
        bounds.height()
    );
    workers[i]->render(rect, tempBounds, maxIterations, rPart, iPart);
  }
}

std::map<QString, QWidget*> Julia::getSettings() {
	std::map<QString, QWidget*> settings = Fractal::getSettings();

  // Slider Einstellung zum Ändern des Real-/Imaginärteils.
	QSlider* realPartSlider = new QSlider(Qt::Horizontal);
	realPartSlider->setFocusPolicy(Qt::StrongFocus);
  // Der Slider kann standardmäßig nur Integer verabrieten, deswegen
  // werden die Werte mit 100 multipliziert.
	realPartSlider->setMinimum(-250);
	realPartSlider->setMaximum(100);
	realPartSlider->setSingleStep(1);
  realPartSlider->setValue(-80);
	settings.insert(std::pair<QString, QWidget*>("Teil reell", realPartSlider));
	connect(realPartSlider, &QSlider::valueChanged, [=](const int value) {
    // Da Slider nur Interger verabrieten kann, siehe oben.
		rPart = (double) value / 100.0;
		update();
	});
  connect(this, &Julia::rPartChanged, [=](const double value) {
    // Da Slider nur Interger verabrieten kann, siehe oben.
    realPartSlider->setValue((int) (value * 100));
  });

	QSlider* imgPartSlider = new QSlider(Qt::Horizontal);
	imgPartSlider->setFocusPolicy(Qt::StrongFocus);
	imgPartSlider->setMinimum(-100);
	imgPartSlider->setMaximum(100);
	imgPartSlider->setSingleStep(1);
  imgPartSlider->setValue(16);
	settings.insert(std::pair<QString, QWidget*>("Teil imaginär", imgPartSlider));
	connect(imgPartSlider, &QSlider::valueChanged, [=](const int value){
		iPart = (double) value / 100.0;
		update();
	});
  connect(this, &Julia::iPartChanged, [=](const double value) {
    imgPartSlider->setValue((int) (value * 100));
  });

	return settings;
}

QString Julia::getOverlayInformation() const {
  return "Reeller Teil: " + QString::number(rPart) + ", Imaginärer Teil: " + QString::number(iPart);
}

void Julia::checkThreadStatus() {
  // Überprüfe, ob alle Threads fertig mit ihren
  // Berechnungen sind.
  bool allThreadsDone = true;
  for(auto& worker : workers) {
    if(!worker->isFinished()) {
      allThreadsDone = false;
    }
  }
  if(allThreadsDone) {
    emit finishedRendering();
  }
}
