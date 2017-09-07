#include <catch.hpp>
#include "../physics/vec2.h"
#include "../physics/ball.h"


namespace ball_test {
	SCENARIO("moving") {
		GIVEN("default-constructed ball") {
			ball b;
			SECTION("its pos is zero")
				REQUIRE(b.pos() == vec2(0,0));
			WHEN("set pos") {
				const vec2 p{5.1,6.2};
				b.pos(p);
				THEN("pos is changed")
					REQUIRE(b.pos() == p);
				AND_WHEN("moved") {
					const vec2 d{11.2, 4};
					b.move(d);
					THEN("pos is changed")
						REQUIRE(b.pos() == p+d);
				}
			}
		}
	}
	SCENARIO("constructing") {
		GIVEN("ball constructed with position x"){
			const vec2 p{1.1, 2.2};
			ball b{p};
			SECTION("its pos is this x")
				REQUIRE(b.pos() == p);
		}

		GIVEN("ball constructed from ball x") {
			ball b1{{3.4,5.6}}, b2{b1};
			SECTION("its pos == x.pos")
				REQUIRE(b1.pos() == b2.pos());
		}
	}
}
