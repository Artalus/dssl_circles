#include "renderarea.h"

#include <QPainter>

std::vector<QPen> colors;

RenderArea::RenderArea(QWidget *parent)
	: QWidget(parent)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	for (int i = 0; i < 10; ++i ){
		s.add({10+ (rand() % 130), 10 + (rand() % 100)});
		colors.emplace_back(qRgb(10*(rand()%26),10*(rand()%26),10*(rand()%26)));
	}

	phys_thread = std::thread{&RenderArea::phys_loop, this};
}

RenderArea::~RenderArea() {
	should_finish = true;
	phys_thread.join();
}

QSize RenderArea::minimumSizeHint() const
{
	return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
	return QSize(400, 200);
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
	std::lock_guard<std::mutex> g(system_mutex);
	QPixmap doubleBuffer{this->size()};
	QPainter painter(this);
	QPainter dbPainter(&doubleBuffer);
	dbPainter.scale(3,3);

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
	constexpr auto frames_per_second = 10;
	constexpr auto required_delta = 1ms;
	// TODO: implement actual frame limiting
	auto last = clk::now();


	while(!should_finish) {
		const auto now = clk::now();
		auto actual_delta = now-last;
		if (actual_delta >= required_delta)
		{
			last = now;
			std::lock_guard<std::mutex> g(system_mutex);
			while (actual_delta >= required_delta) {
				auto step = duration_cast<milliseconds>(actual_delta < required_delta
					? actual_delta
					: required_delta);
				s.simulate(step);
				actual_delta -= required_delta;
			}
		}
		//std::this_thread::sleep_until(now + required_delta);
		if (!should_finish)
			this->update();
	}
}