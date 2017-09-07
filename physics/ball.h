#pragma once
#include "vec2.h"


class ball
{
public:
	ball() = default;

	explicit ball(const vec2 &pos)
		:pos_(pos)
	{}

	vec2 pos() const {
		return pos_;
	}
	void pos(const vec2 &p) {
		pos_ = p;
	}

	void move(const vec2 &dir) {
		pos_ += dir;
	}
private:
	vec2 pos_;
};

