/*
 *  Chocobo1/Hash
 *
 *   Copyright 2017 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

#include "../src/sha2_512_224.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


#define ARRAY_LENGTH(a) (std::extent<decltype(a)>::value)


TEST_CASE("sha2-512/224")
{
	using Hash = Chocobo1::SHA2_512_224;

	// official test suite on NIST.gov
	const char s1[] = "abc";
	REQUIRE("4634270f707b6a54daae7530460842e20e37ed265ceee9a43e8924aa"
			== Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
	REQUIRE("23fec5bb94d60b23308192640b0c453335d664734fe40e7268674af9"
			== Hash().addData(s2, strlen(s2)).finalize().toString());


	// my own tests
	REQUIRE("6ed0dd02806fa89e25de060c19d3ac86cabb87d6a0ddd05c333b84f4"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("944cd2847fb54558d4775db0485a50003111c8e5daa63fe722c6aa37"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("6d6a9279495ec4061769752e7ff9c68b6b0b3c5a281b7917ce0572de"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("301fbe7e0354fe16b78b807653a930061efe2ab11caa11fe9bfc6c03"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("02682009a96ba45d69a5d10c95e108f0f9c0fb095e14a8a0abd62856"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(gsl::span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());
}
