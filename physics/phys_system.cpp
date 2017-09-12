#include "phys_system.h"
#include <string>
#include <algorithm>
#include <vector>
#include <chrono>
#include <tuple>

vec2 force_formula(vec2 direction) {
	const auto r = length(direction);
	if (r==0)
		return normalize(vec2{rand(), rand()});
	vec2 result{(1/r - 1/r/r) * normalize(direction)};
	assert(!isnan(length(result)));
	return result;
}

uint64_t phys_system::add(const vec2 &pos) {
	ball_wrapper w(pos);
	const auto id = w.id;
	const auto place = lower_bound(cbegin(), cend(), id,
		[](auto &&lhs, auto rhs) {
		return lhs.id <= rhs;
	});
	balls_.emplace(place, std::move(w));
	return id;
}

size_t phys_system::size() const {
	return balls_.size();
}

void phys_system::simulate(std::chrono::milliseconds delta) {
	assert(delta.count() > 0);
	apply_forces(calc_forces(), delta);
}

ball& phys_system::get(uint64_t id) const {
	const auto it = lower_bound(cbegin(), cend(), id,
		[](auto &&lhs, auto rhs) {
		return lhs.id < rhs;
	});
	if (it == cend())
		throw std::runtime_error("couldn't find ball with id " + std::to_string(id));
	return it->get();
}

ball& phys_system::get_locked() const {
	assert(locked_id_.has_value() && "no ball was locked in system");
	return get(locked_id_.value());
}

bool phys_system::unlock() {
	bool r = false;
	if (locked_id_.has_value())
		r = true;
	locked_id_.reset();
	return r;
}

void phys_system::lock(const vec2 &pos) {
	assert(!locked_id_.has_value() && "cannot lock a ball when another is already locked");

	const auto target = find_in_pos(pos);
	if (target == end())
		return;

	locked_id_ = target->id;
}

void phys_system::drag(const vec2 &diff) {
	if(locked_id_.has_value())
		get_locked().move(diff);
}

void phys_system::apply_forces(const std::vector<std::tuple<bw_iterator, vec2>> &forces,
	std::chrono::milliseconds delta) {
	for ( auto&& pair : forces ) {
		//auto [it, force] = pair;
		auto it = std::get<0>(pair);
		const auto force = std::get<1>(pair);

		auto fdelta = static_cast<float>(delta.count()) / 1000;
		it->add_impulse(force*fdelta);

		auto dist = it->get_impulse() / it->mass * fdelta;
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

	for ( auto it = begin(); it != end(); ++it ) {
		if (locked_id_.has_value() && it->id == locked_id_)
			continue;
		result.emplace_back(make_tuple(it, calc_force_on(it->get(), it->id)));
	}

	return result;
}

phys_system::cbw_iterator phys_system::find_in_pos(const vec2 &pos) const {
	return find_if(cbegin(), cend(), [&pos](const ball_wrapper& bw)
		{ return bw.get().is_point_inside(pos); }
	);
}

uint64_t phys_system::ball_wrapper::guid_ = 1;

std::optional<uint64_t> phys_system::remove(const vec2 &pos) {
	const auto target = find_in_pos(pos);

	if (target == end())
		return std::nullopt;

	auto id = target->id;

	balls_.erase(target);
	return id;
}
