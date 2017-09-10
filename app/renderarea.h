#pragma once
#include <QWidget>
#include "../physics/phys_system.h"
#include <thread>
#include <mutex>


class RenderArea : public QWidget
{
	Q_OBJECT
	friend std::thread;
public:
	RenderArea(QWidget *parent = nullptr);
	~RenderArea();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void phys_loop();
	phys_system s;
	std::thread phys_thread;
	std::mutex system_mutex;
	std::atomic<bool> should_finish = false;
};
