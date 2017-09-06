#include "vec2.h"

double vec2::length() const {
	return sqrt(x_ * x_ + y_ * y_);
}
