#include <catch.hpp>
#include "../physics/vec2.h"


namespace test_vec2 {
	SCENARIO( "assignment" ) {
		GIVEN("empty vector") {
			vec2 v;
			REQUIRE(v.x() == 0);
			REQUIRE(v.y() == 0);

			WHEN("assign x") {
				v.x(8);
				THEN("only x changed") {
					REQUIRE(v.x() > 0);
					REQUIRE(v.x() == Approx(8));
					REQUIRE(v.y() == Approx(0));
				}
			}

			WHEN("assign y") {
				v.y(-2.1);
				THEN("only y changed") {
					REQUIRE(v.y() < 0);
					REQUIRE(v.y() == Approx(-2.1));
					REQUIRE(v.x() == Approx(0));
				}
			}

			WHEN("assign to another vector") {
				const vec2 v2{5,6};
				v = v2;
				THEN("both coordinates changed") {
					REQUIRE(v.x() == Approx(v2.x()));
					REQUIRE(v.y() == Approx(v2.y()));
				}
			}
		}

		GIVEN("vector copy constructed from another ") {
			const vec2 v2(7, 8);
			vec2 v(v2);
			SECTION("both coordinates are equal to another ones") {
				REQUIRE(v.x() == Approx(v2.x()));
				REQUIRE(v.y() == Approx(v2.y()));
			}
		}
	}

	SCENARIO("methods") {
		GIVEN("default constructed vector") {
			vec2 v{};
			SECTION("length is 0")
				REQUIRE(v.length() == 0);
		}
		GIVEN("some vector") {
			vec2 v1{1,0}, v2{0,1}, v3{3,4};
			SECTION("length is present") {
				REQUIRE(v1.length() == Approx(1));
				REQUIRE(v2.length() == Approx(1));
				REQUIRE(v3.length() == Approx(5));
			}
		}
	}
}
