#pragma once
#include <catch.hpp>
#include "../physics/vec2.h"

/*bool operator==(const vec2& lhs, const vec2 &rhs) {
	return lhs.x() == Approx(rhs.x())
		&& lhs.y() == Approx(rhs.y());
}*/

std::ostream& operator<<(std::ostream &s, const vec2 &v);

vec2::value_type operator ""_r(long double r);
