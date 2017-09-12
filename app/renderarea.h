#pragma once
#include <thread>
#include <mutex>
#include <QWidget>
#include <QTimer>
#include "../physics/phys_system.h"


class RenderArea : public QWidget {
Q_OBJECT
	const float scale_factor = 1;

public:
	RenderArea(QWidget *parent = nullptr);
	~RenderArea();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

private:
	vec2 RenderArea::get_click_pos(QPointF pos) const;
	void phys_loop();
	void add_ball(const vec2 &pos);
	void remove_ball(const vec2 &pos);

	QTimer timer;
	phys_system s;

	std::map<uint64_t, QPen> colors;

	std::thread phys_thread;
	std::mutex system_mutex;
	std::atomic<bool> should_finish = false;
};
