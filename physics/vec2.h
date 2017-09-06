#pragma once
#include <cmath>


class vec2 {
public:
	vec2() = default;

	vec2(float x, float y)
		: x_(x)
		, y_(y) {
	}

	double x() const {
		return this->x_;
	}

	void x(double v) {
		x_ = v;
	}

	double y() const {
		return this->y_;
	}

	void y(double v) {
		y_ = v;
	}

	double length() const;

private:
	double x_ = 0., y_ = 0.;
};
