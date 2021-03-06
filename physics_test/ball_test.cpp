#include "common.h"
#include "../physics/ball.h"


namespace ball_test {
	SCENARIO("moving") {
		GIVEN("default-constructed ball") {
			ball b;
			SECTION("its pos is zero")
				REQUIRE(b.pos() == vec2(0,0));
			WHEN("set pos") {
				const vec2 p{5.1_r,6.2_r};
				b.pos(p);
				THEN("pos is changed")
					REQUIRE(b.pos() == p);
				AND_WHEN("moved") {
					const vec2 d{11.2_r, 4._r};
					b.move(d);
					THEN("pos is changed")
						REQUIRE(b.pos() == p+d);
				}
			}
		}
	}
	SCENARIO("constructing") {
		GIVEN("ball constructed with position x"){
			const vec2 p{1.1_r, 2.2_r};
			ball b{p};
			SECTION("its pos is this x")
				REQUIRE(b.pos() == p);
		}

		GIVEN("ball constructed from ball x") {
			ball b1{{3.4_r,5.6_r}}, b2{b1};
			SECTION("its pos == x.pos")
				REQUIRE(b1.pos() == b2.pos());
		}
	}

	SCENARIO("is point inside") {
		const vec2 pos{10._r,10._r}, y{0,0.5_r}, x{0.5_r,0};
		GIVEN("a ball at 10;10") {
			const ball b{pos};

			SECTION("its center is inside")
				REQUIRE(b.is_point_inside(pos));
			SECTION("its radius/2 offsets are inside") {
				REQUIRE(b.is_point_inside(pos+x));
				REQUIRE(b.is_point_inside(pos-x));
				REQUIRE(b.is_point_inside(pos+y));
				REQUIRE(b.is_point_inside(pos-y));
			}
			SECTION("its radius*3 is outside") {
				REQUIRE_FALSE(b.is_point_inside(pos+3.f*x));
				REQUIRE_FALSE(b.is_point_inside(pos-3.f*x));
				REQUIRE_FALSE(b.is_point_inside(pos+3.f*y));
				REQUIRE_FALSE(b.is_point_inside(pos-3.f*y));
			}
		}
	}
}
