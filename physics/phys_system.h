#pragma once
#include <chrono>
#include <memory>

#include "ball.h"


class phys_system
{
	class ball_wrapper {
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

		const ball& get() const {
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
	size_t size() const;
	void simulate(std::chrono::milliseconds delta);
	const ball& get(uint64_t bm);
};
