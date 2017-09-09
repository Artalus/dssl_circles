#include "common.h"
#include <chrono>
#include "../physics/phys_system.h"
#include "../physics/ball.h"
#include <glm/gtx/vector_angle.hpp>

using clk = std::chrono::system_clock;
using namespace std::chrono_literals;
using namespace std::chrono;


constexpr auto frames = 60;
constexpr auto delta = fmilliseconds(1s)/frames;


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
			const vec2 original_pos{5.2, 5.3};
			const auto it = system.add(original_pos);
			THEN("size increased")
				REQUIRE(system.size() == 1);
			WHEN("get ball by id") {
				const auto b = system.get(it);
				THEN("it is same ball") {
					REQUIRE(b.pos() == original_pos);
				}
			}

			AND_WHEN("simulate for a second at 60fps") {
				for (int i = 0; i < frames; ++i) {
					system.simulate(delta);
				}
				THEN("ball is in same position")
					REQUIRE(system.get(it).pos() == original_pos);
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
			const vec2 original_1{-10,0},
				original_m{0,0},
				original_2{10,0};
			const auto b1 = system.add(original_1),
				bm = system.add(original_m),
				b2 = system.add(original_2);
			THEN("their ids differ") {
				REQUIRE(b1 != b2);
				REQUIRE(b1 != bm);
				REQUIRE(b2 != bm);
			}
			THEN("size increased")
				REQUIRE(system.size() == 3);
			AND_WHEN("simulate for a second at 60fps") {
				for (int i = 0; i < frames; ++i) {
					system.simulate(delta);
				}
				THEN("middle ball is in same position")
					REQUIRE(system.get(bm).pos() == original_m);
				THEN("side ones have moved for same distance") {
					const auto b1_pos = system.get(b1).pos(),
						b2_pos = system.get(b2).pos();
					auto b1_vec = vec2_from_two_points(original_1, b1_pos),
						b2_vec = -vec2_from_two_points(original_2, b2_pos);
					REQUIRE(b1_vec.length() == b2_vec.length());
					SECTION("and are on same line")
						REQUIRE(glm::angle(b1_vec, b2_vec) == Approx(0));
					SECTION("and the distance is positive")
						REQUIRE(glm::length(b1_vec) > 0);
				}
			}
		}
	}
}

SCENARIO("removing balls from a system") {
	std::vector<vec2> positions{vec2{11.1, 22.2}, vec2{0,1}, vec2{0,2.5}};
	positions.emplace_back(*rbegin(positions));


	GIVEN("an empty system") {
		phys_system s;
		REQUIRE(s.size() == 0);

		WHEN("clicking anywhere") {
			for (auto &&v : positions)
				s.remove(v);
			THEN("nothing changes")
				REQUIRE(s.size() == 0);
		}

		WHEN("adding 4 elements") {
			for (auto &&v : positions)
				s.add(v);
			THEN("size is 4")
				REQUIRE(s.size() == 4);

			WHEN("clicking on an empty space") {
				s.remove(positions[0]*2.f);
				THEN("nothing changes")
					REQUIRE(s.size() == 4);
			}
			WHEN("clicking on a point with a ball") {
				s.remove(positions[0]);
				THEN("size is decreased")
					REQUIRE(s.size() == 3);
				WHEN("clicking on a point with two balls") {
					s.remove(*rbegin(positions));
					THEN("removed is only 1 element")
						REQUIRE(s.size() == 2);
					WHEN("clicking on this point again") {
						s.remove(*rbegin(positions));
						THEN("removed is only 1 element")
							REQUIRE(s.size() == 1);
						AND_WHEN("clicking again") {
							s.remove(*rbegin(positions));
							THEN("nothing changed")
								REQUIRE(s.size() == 1);
						}
					}
				}
			}
		}
	}
}

SCENARIO("locking a ball") {
	phys_system s;
	vec2 m_pos{0,0}, l_pos{100,0};
	auto original_dist = distance(l_pos, m_pos);
	auto moving = s.add(m_pos);
	auto locked = s.add(l_pos);
	s.lock(l_pos);
	GIVEN("a system with a locked and unlocked ball") {
		WHEN("simulate for a second at 60fps") {
			for (int i=0; i<frames; ++i)
				s.simulate(delta);
			vec2 new_m = s.get(moving).pos();
			THEN("locked ball didn't move") {
				vec2 new_l = s.get_locked().pos();
				REQUIRE(new_l == l_pos);
				SECTION("and unlocked moved towards locked")
					REQUIRE(distance(l_pos, new_m) < original_dist);

				AND_WHEN("unlock locked ball and simulate again") {
					s.unlock();
					for (int i=0; i<frames; ++i)
						s.simulate(delta);

					THEN("locked ball moved") {
						new_l = s.get(locked).pos();
						REQUIRE(distance(l_pos, new_l) > 0);
					}
				}
			}
		}
	}
}


SCENARIO("dragging a ball") {
	phys_system s;
	const vec2 start{0,0}, diff={12,2}, end{start+diff};
	s.add({0,0});
	GIVEN("a system with 1 ball at 0;0") {
		s.lock(start);
		REQUIRE(s.get_locked().pos() == start);
		WHEN("simulate for a second at 60fps") {
			for (int i=0; i<frames; ++i)
				s.simulate(delta);
			AND_WHEN("dragged") {
				s.drag(diff);
				THEN("its pos changed for that diff")
					REQUIRE(s.get_locked().pos() == end);
			}
		}
		s.unlock();
	}
}
