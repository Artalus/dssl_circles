#include "phys_system.h"

int phys_system::add(const ball &ball) {
	return 1;
}

size_t phys_system::size() const {
	return 0;
}

void phys_system::simulate(std::chrono::milliseconds delta) {
}

const ball & phys_system::get(int bm) {
	static ball b{{20,-30}};
	return b;
}
