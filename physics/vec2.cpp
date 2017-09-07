#include "vec2.h"

double vec2::length() const {
	return sqrt(x_ * x_ + y_ * y_);
}

vec2 vec2::from_two_points(const vec2 &a, const vec2 &b) {
	return {b.x() - a.x(), b.y() - a.y()};
}
