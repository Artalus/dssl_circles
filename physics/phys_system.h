#pragma once
#include "ball.h"
#include <chrono>


class phys_system
{
public:
	phys_system() = default;
	~phys_system() = default;
	int add(const ball & ball);
	size_t size() const;
	void simulate(std::chrono::milliseconds delta);
	const ball& get(int bm);
};

