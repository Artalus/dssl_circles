#include "phys_system.h"
#include <string>

uint64_t phys_system::add(const vec2 &pos) {
	ball_wrapper w(pos);
	auto id = w.id;
	const auto place = std::lower_bound(begin(balls_), end(balls_), id,
		[](auto &&lhs, auto rhs) {
		return lhs.id <= rhs;
	});
	balls_.emplace(place, std::move(w));
	return id;
}

size_t phys_system::size() const {
	return balls_.size();
}

void phys_system::simulate(fmilliseconds delta) {
	assert(delta.count() > 0);
}

const ball & phys_system::get(uint64_t id) {
	auto it = lower_bound(begin(balls_), end(balls_), id,
		[](auto &&lhs, auto rhs) {
		return lhs.id < rhs;
	});
	if (it == end(balls_))
		throw std::runtime_error("couldn't find ball with id " + std::to_string(id));
	return it->get();
}


uint64_t phys_system::ball_wrapper::guid_ = 1;
