#pragma once
#include <chrono>
#include <memory>
#include <vector>
#include <tuple>
#include <optional>

#include "ball.h"

class phys_system
{
	class ball_wrapper {
		friend phys_system;

		static uint64_t guid_;
		std::unique_ptr<ball> b_;
	public:
		uint64_t id;

		explicit ball_wrapper(vec2 pos = {})
			: b_{std::make_unique<ball>(pos)}
			, id{guid_++}
		{
			if (guid_ == 0)
				throw std::runtime_error("guid overflow");
		}

		ball_wrapper(const ball_wrapper&) = delete;
		ball_wrapper(ball_wrapper&&) = default;
		ball_wrapper& operator=(const ball_wrapper&) = delete;
		ball_wrapper& operator=(ball_wrapper&&) = default;

		ball& get() const {
			return *b_;
		}
		bool operator<(const ball_wrapper &rhs) const {
			return id < rhs.id;
		}
	};

	std::vector<ball_wrapper> balls_;

public:
	phys_system() = default;
	~phys_system() = default;

	uint64_t add(const vec2 &pos);
	void remove(const vec2 &pos);
	size_t size() const;

	void simulate(std::chrono::milliseconds delta);

	ball& get(uint64_t bm) const;
	ball& get_locked() const;
	void unlock();
	void lock(const vec2 &pos);
	void drag(const vec2 &diff);

private:
	using bw_iterator = decltype(balls_.cbegin());
	void apply_forces(const std::vector<std::tuple<bw_iterator, vec2>> &forces,
		std::chrono::milliseconds delta);
	vec2 calc_force_on(const ball &b, const uint64_t id);
	std::vector<std::tuple<bw_iterator, vec2>> calc_forces();

	bw_iterator find_in_pos(const vec2 &pos) const;

	std::optional<decltype(ball_wrapper::guid_)> locked_id_ = std::nullopt;
};
