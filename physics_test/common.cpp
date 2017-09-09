#include "common.h"

std::ostream& operator<<(std::ostream &s, const vec2 &v) {
	return s << "vec2{" << v.x << " , " << v.y << "}";
}
