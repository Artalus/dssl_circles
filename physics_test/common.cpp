#include "common.h"
#include "../physics/ball.h"

std::ostream& operator<<(std::ostream &s, const vec2 &v) {
	return s << "vec2{" << v.x << " , " << v.y << "}";
}

vec2::value_type operator ""_r(long double r) {
	return r * ball::radius;
}
