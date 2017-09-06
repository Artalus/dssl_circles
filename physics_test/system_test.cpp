#include <catch.hpp>
#include "../physics/phys_system.h"
#include <chrono>
#include "../physics/vec2.h"

SCENARIO("physical system") {
	GIVEN("empty system") {
		phys_system system;

		WHEN("add line of three balls") {
			auto b1 = system.add(ball{-10, 0}),
				bm = system.add(ball{0, 0}),
				b2 = system.add(ball{10, 0});
			THEN("size increased")
				REQUIRE(system.size() == 3);
			AND_WHEN("simulate for a second at 60fps") {
				using clk = std::chrono::system_clock;
				using namespace std::chrono_literals;
				using namespace std::chrono;
				constexpr auto frames = 60;
				milliseconds diff = 1s/frames;
				for (int i = 0; i < frames; ++i) {
					system.simulate(diff);
				}
				THEN("middle ball is in same position")
					REQUIRE(system.get(bm).pos() == {0,0});
				THEN("side ones are closer for same distance") {
					auto b1_pos = system.get(b1).pos(),
						b2_pos = system.get(b2).pos();
					auto b1_dist = vec2::from_two_points({-10,0}, b1_pos),
						b2_dist = vec2::from_two_points(b2_pos, {10,0});
					REQUIRE(b1_dist.length() == b2_dist.length());
					SECTION("and are on same line") {
						REQUIRE(b1_dist.x() == Approx(b2_dist.x()));
						REQUIRE(b1_dist.y() == Approx(b2_dist.y()));
					}
				}
			}
		}
	}
}