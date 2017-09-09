#include "phys_system.h"
#include <string>
#include <algorithm>
#include <vector>
#include <chrono>
#include <tuple>

vec2 force_formula(vec2 direction) {
	const auto r = length(direction);
	return (1/r - 1/r/r) * direction;
}

uint64_t phys_system::add(const vec2 &pos) {
	ball_wrapper w(pos);
	const auto id = w.id;
	const auto place = lower_bound(cbegin(balls_), cend(balls_), id,
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
	apply_forces(calc_forces(), delta);
}

const ball& phys_system::get(uint64_t id) {
	const auto it = lower_bound(cbegin(balls_), cend(balls_), id,
		[](auto &&lhs, auto rhs) {
		return lhs.id < rhs;
	});
	if (it == end(balls_))
		throw std::runtime_error("couldn't find ball with id " + std::to_string(id));
	return it->get();
}

void phys_system::apply_forces(const std::vector<std::tuple<bw_iterator, vec2>> &forces,
	fmilliseconds delta) {
	for ( auto&& pair : forces ) {
		//auto [it, force] = pair;
		const auto it = std::get<0>(pair);
		const auto force = std::get<1>(pair);

		//TODO: implement actual FORCE, not interprete it as velocity
		const vec2 dist = force * static_cast<float>(delta.count());
		it->get().move(dist);
	}
}

vec2 phys_system::calc_force_on(const ball &b, const uint64_t id) {
	vec2 force;
	const vec2 pos = b.pos();
	for (auto &&bw : balls_)
		if (bw.id != id) {
			force += force_formula(bw.get().pos() - pos);
		}
	return force;
}

std::vector<std::tuple<phys_system::bw_iterator, vec2>> phys_system::calc_forces() {
	std::vector<std::tuple<bw_iterator, vec2>> result;
	result.reserve(size());

	for ( auto it = cbegin(balls_); it != cend(balls_); ++it ) {
		result.emplace_back(make_tuple(it, calc_force_on(it->get(), it->id)));
	}

	return result;
}

uint64_t phys_system::ball_wrapper::guid_ = 1;




//TEST_CASE("calc_force")
