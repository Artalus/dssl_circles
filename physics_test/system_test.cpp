#include <catch.hpp>
#include <chrono>
#include "../physics/phys_system.h"
#include "../physics/vec2.h"
#include "../physics/ball.h"

using clk = std::chrono::system_clock;
using namespace std::chrono_literals;
using namespace std::chrono;

SCENARIO("id progression") {
	GIVEN("empty system") {
		phys_system s;
		const vec2 p1{1,1}, p2{22,22}, p3{333,333};

		WHEN("add 3 balls") {
			auto i1 = s.add(p1),
				i2 = s.add(p2),
				i3 = s.add(p3);
			THEN("i1 < i2 < i3") {
				REQUIRE(i1 < i2);
				REQUIRE(i2 < i3);
			}
		}
		
		WHEN("add 3 balls one by one and immediately get them") {
			auto i1 = s.add(p1);
			auto b1 = s.get(i1);
			auto i2 = s.add(p2);
			auto b2 = s.get(i2);
			auto i3 = s.add(p3);
			auto b3 = s.get(i3);
			THEN("they are on same positions as originally") {
				REQUIRE(b1.pos() == p1);
				REQUIRE(b2.pos() == p2);
				REQUIRE(b3.pos() == p3);
			}
		}
	}
}

SCENARIO("single ball system") {
	GIVEN("empty system") {
		phys_system system;
		SECTION("size is 0")
			REQUIRE(system.size() == 0);

		WHEN("add a ball") {
			const ball original{{5.2, 5.3}};
			const auto i = system.add(original);
			THEN("size increased")
				REQUIRE(system.size() == 1);
			WHEN("get ball by id") {
				const auto b = system.get(i);
				THEN("it is same ball") {
					REQUIRE(b.pos() == original.pos());
					REQUIRE(&b == &original);
				}
			}
			
			AND_WHEN("simulate for a second at 60fps") {
				constexpr auto frames = 60;
				constexpr milliseconds delta = 1s/frames;
				for (int i = 0; i < frames; ++i) {
					system.simulate(delta);
				}
				THEN("ball is in same position")
					REQUIRE(system.get(i).pos() == vec2(0,0));
			}
		}
	}
}

SCENARIO("three-ball-line system") {
	GIVEN("empty system") {
		phys_system system;
		SECTION("size is 0")
			REQUIRE(system.size() == 0);

		WHEN("add line of three balls") {
			const auto b1 = system.add(ball{{-10, 0}}),
				bm = system.add(ball{{0, 0}}),
				b2 = system.add(ball{{10, 0}});
			THEN("their ids differ") {
				REQUIRE(b1 != b2);
				REQUIRE(b1 != bm);
				REQUIRE(b2 != bm);
			}
			THEN("size increased")
				REQUIRE(system.size() == 3);
			AND_WHEN("simulate for a second at 60fps") {
				constexpr auto frames = 60;
				constexpr milliseconds delta = 1s/frames;
				for (int i = 0; i < frames; ++i) {
					system.simulate(delta);
				}
				THEN("middle ball is in same position")
					REQUIRE(system.get(bm).pos() == vec2(0,0));
				THEN("side ones are closer for same distance") {
					const auto b1_pos = system.get(b1).pos(),
						b2_pos = system.get(b2).pos();
					auto b1_dist = vec2_from_two_points({-10,0}, b1_pos),
						b2_dist = vec2_from_two_points(b2_pos, {10,0});
					REQUIRE(b1_dist.length() == b2_dist.length());
					SECTION("and are on same line")
						REQUIRE(b1_dist == b2_dist);
					SECTION("and the distance is positive")
						REQUIRE(glm::length(b1_dist) > 0);
				}
			}
		}
	}
}