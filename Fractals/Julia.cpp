#include "Julia.hpp"
#include "../Rendering/JuliaRenderTask.hpp"
#include <QSlider>
#include <QtGui/QPainter>

Julia::Julia(int width, int height)
    : Mandelbrot(width, height), rPart(-0.8), iPart(0.156)
{
  bounds.setRect(-2, -1.5, 4, 3);
  Mandelbrot::workers.clear();
  createWorkers();
}

QDataStream &Julia::print(QDataStream &os) const {
  return Mandelbrot::print(os) << rPart << iPart;
}

QDataStream &Julia::read(QDataStream &os) {
  Mandelbrot::print(os) >> rPart >> iPart;
  update();
  return os;
}

void Julia::createWorkers() {
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    workers.push_back(std::unique_ptr<JuliaRenderTask>(new JuliaRenderTask(this)));
    connect(workers[i].get(), &JuliaRenderTask::rendered, this, &Julia::updatePixels);
  };
}

void Julia::update() {
  for(int i = 0; i < QThread::idealThreadCount(); i++) {
    QRect rect(
        (int) std::floor(i * image.width() / QThread::idealThreadCount()),
        0,
        (int) std::ceil((double) image.width() / QThread::idealThreadCount()),
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

	QSlider* realPartSlider = new QSlider(Qt::Horizontal);
	realPartSlider->setFocusPolicy(Qt::StrongFocus);
	realPartSlider->setMinimum(-250);
	realPartSlider->setMaximum(100);
	realPartSlider->setSingleStep(1);
	settings.insert(std::pair<QString, QWidget*>("Reeller Teil", realPartSlider));
	connect(realPartSlider, &QSlider::valueChanged, [=](const int value){
		rPart = (double) value / 100.0;
		update();
	});

	QSlider* imgPartSlider = new QSlider(Qt::Horizontal);
	imgPartSlider->setFocusPolicy(Qt::StrongFocus);
	imgPartSlider->setMinimum(-100);
	imgPartSlider->setMaximum(100);
	imgPartSlider->setSingleStep(1);
	settings.insert(std::pair<QString, QWidget*>("ZImaginärer Teil", imgPartSlider));
	connect(imgPartSlider, &QSlider::valueChanged, [=](const int value){
		iPart = (double) value / 100.0;
		update();
	});

	return settings;
}

void Julia::draw(Canvas &target) {
	Fractal::draw(target);

  QPainter painter(&image);

	QString text = "Reeller Teil: " + QString::number(rPart) + ", Imaginärer Teil: " + QString::number(iPart);
	QFontMetrics metrics = painter.fontMetrics();
	int textWidth = metrics.width(text);

	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(0, 0, 0, 127));
	painter.drawRect((image.width() - textWidth) - 10, 0, textWidth + 10, metrics.lineSpacing() + 5);
	painter.setPen(Qt::white);
	painter.drawText(image.width() - textWidth - 5, metrics.leading() + metrics.ascent(), text);
}
