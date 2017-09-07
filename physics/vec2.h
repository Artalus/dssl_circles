#pragma once
#include <cmath>


class vec2 {
public:
	vec2() = default;

	vec2(double x, double y)
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

	static vec2 from_two_points(const vec2& a, const vec2& b);

private:
	double x_ = 0., y_ = 0.;
};
