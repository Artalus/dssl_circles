#pragma once
#include <glm/glm.hpp>

using vec2 = glm::vec2;
vec2 vec2_from_two_points(const vec2& a, const vec2& b);
/*
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

	vec2& operator+=(const vec2 &rhs) {
		x_ += rhs.x_;
		y_ += rhs.y_;
		return *this;
	}

	friend vec2 operator+(const vec2 &lhs, const vec2 &rhs) {
		vec2 r{lhs};
		return r += rhs;
	}

	vec2 operator-() const {
		return {-x_, -y_};
	}

	double length() const;

	static vec2 from_two_points(const vec2& a, const vec2& b);

private:
	double x_ = 0., y_ = 0.;
};
*/
