#include <catch.hpp>
#include "../physics/vec2.h"


/*bool operator==(const vec2& lhs, const vec2 &rhs) {
	return lhs.x() == Approx(rhs.x())
		&& lhs.y() == Approx(rhs.y());
}*/

std::ostream& operator<<(std::ostream& s, const vec2 &v) {
	return s << "vec2{" << v.x << " , " << v.y << "}";
}

namespace test_vec2 {
	SCENARIO( "assignment" ) {
		GIVEN("empty vector") {
			vec2 v;
			REQUIRE(v.x == 0);
			REQUIRE(v.y == 0);

			WHEN("assign x") {
				v.x = 8;
				THEN("only x changed") {
					REQUIRE(v.x> 0);
					REQUIRE(v.x== Approx(8));
					REQUIRE(v.y== Approx(0));
				}
			}

			WHEN("assign y") {
				v.y = -2.1;
				THEN("only y changed") {
					REQUIRE(v.y < 0);
					REQUIRE(v.y == Approx(-2.1));
					REQUIRE(v.x == Approx(0));
				}
			}

			WHEN("assign to another vector") {
				const vec2 v2{5,6};
				v = v2;
				THEN("both coordinates changed") {
					REQUIRE(v.x == Approx(v2.x));
					REQUIRE(v.y == Approx(v2.y));
				}
			}
		}

		GIVEN("vector copy constructed from another ") {
			const vec2 v2{7, 8};
			vec2 v(v2);
			SECTION("both coordinates are equal to another ones") {
				REQUIRE(v.x == Approx(v2.x));
				REQUIRE(v.y == Approx(v2.y));
			}
		}
	}

	SCENARIO("length") {
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
	SCENARIO("negation vector") {
		GIVEN("zero vector")
			SECTION("result is zero")
				REQUIRE( -vec2() == vec2());

		GIVEN("positive vector")
			SECTION("result has negative coordinates")
				REQUIRE( -vec2(1,4) == vec2(-1,-4));
		
		GIVEN("negative vector")
			SECTION("result has positive coordinates")
				REQUIRE( -vec2(-3.1,-1.2) == vec2(3.1,1.2));
	}

	SCENARIO("from two points") {
		GIVEN("2 zero vectors") {
			vec2 a{}, b{};
			SECTION("result is zero") {
				REQUIRE(vec2_from_two_points(a,b) == vec2(0,0));
			}
		}
		GIVEN("2 same vectors") {
			vec2 a{10.5,12.5}, b{a};
			SECTION("result is zero") {
				REQUIRE(vec2_from_two_points(a,b) == vec2(0,0));
			}
		}
		GIVEN("zero and x") {
			vec2 z{0,0}, x{3.3,5.5};
			SECTION("result is x") {
				REQUIRE(vec2_from_two_points(z,x) == x);
			}
			SECTION("backwards is -x") {
				REQUIRE(vec2_from_two_points(x,z) == -x);
			}
		}
	}

	SCENARIO("addition") {
		GIVEN("zero and 2 some vectors") {
			vec2 z{}, x{1.1,3.3}, y{9,5};
			WHEN("zero + zero") {
				THEN("result is zero")
					REQUIRE((z+z) == z);
			}
			WHEN("zero += zero") {
				auto r = z;
				r+=z;
				THEN("result is zero")
					REQUIRE(r == z);
			}
			WHEN("zero + x") {
				THEN("result is x")
					REQUIRE((z+x) == x);
			}
			WHEN("zero += x") {
				auto r = z;
				r+=x;
				THEN("result is x")
					REQUIRE(r == x);
			}
			WHEN("x + y") {
				vec2 r{x.x+y.x, x.y + y.y};
				THEN("coordinates are summed")
					REQUIRE((x+y) == r);
			}
		}
	}
}
