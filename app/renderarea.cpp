#include "renderarea.h"

#include <QPainter>
#include <QMouseEvent>
#include <iostream>

RenderArea::RenderArea(QWidget *parent)
	: QWidget(parent)
	, timer(this)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	for (int i = 0; i < 10; ++i ){
		add_ball({10+ (rand() % 330), 10 + (rand() % 250)});
	}
	phys_thread = std::thread{&RenderArea::phys_loop, this};
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(1000/60);
}

RenderArea::~RenderArea() {
	should_finish = true;
	try {
		phys_thread.join();
	} catch(std::exception &ex) {
		std::cerr << "couldn't safely join thread: " << ex.what();
	}
}

QSize RenderArea::minimumSizeHint() const
{
	return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
	return QSize(400, 200);
}

vec2 RenderArea::get_click_pos(QPointF pos) const {
	return { pos.x() / scale_factor, pos.y() / scale_factor };
}

QPointF last_pos;

void RenderArea::mousePressEvent(QMouseEvent *event) {
	if (event->button() != Qt::MouseButton::LeftButton)
		return;
	std::lock_guard<std::mutex> g(system_mutex);
	s.lock(get_click_pos(event->pos()));
	last_pos = event->pos();
}

void RenderArea::mouseMoveEvent(QMouseEvent *event) {
	if (!event->buttons().testFlag(Qt::MouseButton::LeftButton))
		return;

	std::lock_guard<std::mutex> g(system_mutex);
	const auto pos = event->pos();
	const QPointF delta = pos - last_pos;
	const auto v = get_click_pos(delta);
	s.drag(v);
	last_pos = pos;
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event) {
	std::lock_guard<std::mutex> g(system_mutex);
	bool was_locked = s.unlock();
	if (was_locked)
		return;

	const auto pos = get_click_pos(event->pos());
	if (event->button() == Qt::MouseButton::LeftButton) {
		add_ball(pos);
	}
	else if (event->button() == Qt::MouseButton::RightButton) {
		remove_ball(pos);
	}
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
	const static QBrush brush(Qt::GlobalColor::white);
	std::lock_guard<std::mutex> g(system_mutex);
	QPixmap doubleBuffer{this->size()};
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPainter dbPainter(&doubleBuffer);
	dbPainter.scale(scale_factor, scale_factor);
	dbPainter.setRenderHint(QPainter::Antialiasing, true);
	dbPainter.eraseRect(rect());

	for (auto &&b : s) {
		const auto &pos = b.get().pos();
		const auto r = ball::radius;
		const QRectF rec{pos.x - r, pos.y-r, 2*r, 2*r};
		dbPainter.setPen(colors[b.id-1]);
		dbPainter.drawEllipse(rec);
	}
	painter.drawPixmap(this->rect(), doubleBuffer);
}

void RenderArea::phys_loop() {
	using clk = std::chrono::system_clock;
	using namespace std::chrono;
	using namespace std::chrono_literals;
	constexpr auto frames_per_second = 60;
	constexpr auto maximal_delta = 1000ms/frames_per_second, minimal_delta = 1ms;
	// TODO: implement actual frame limiting
	auto last = clk::now();

	try {
		while(!should_finish) {
			const auto now = clk::now();
			auto actual_delta = now-last;
			if (actual_delta >= minimal_delta)
			{
				last = now;
				std::lock_guard<std::mutex> g(system_mutex);
				while (actual_delta >= maximal_delta) {
					s.simulate(maximal_delta);
					actual_delta -= maximal_delta;
				}
				if ( actual_delta >= minimal_delta )
					s.simulate(duration_cast<milliseconds>(actual_delta));
			}
			//std::this_thread::sleep_until(now + maximal_delta);
		}
	} catch(std::exception &ex) {
		std::cerr << "couldn't perform physics loop: " << ex.what();
	}
}

void RenderArea::add_ball(const vec2 &pos) {
	auto id = s.add(pos);
	colors[id] = {qRgb(10 * (rand() % 26), 10 * (rand() % 26), 10 * (rand() % 26))};
}

void RenderArea::remove_ball(const vec2 &pos) {
	auto removed = s.remove(pos);
	if (removed.has_value())
		colors.erase(removed.value()-1);
}
