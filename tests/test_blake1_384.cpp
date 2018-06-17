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

#include "../src/blake1_384.h"

#include "catch/single_include/catch.hpp"

#include <cstring>


TEST_CASE("blake1-384")
{
	using Hash = Chocobo1::Blake1_384;

	// official test suite from blake1 SHA3-proposal
	const char s1[1] = {0};
	REQUIRE("10281f67e135e90ae8e882251a355510a719367ad70227b137343e1bc122015c29391e8545b5272d13a7c2879da3d807"
			== Hash().addData(s1, sizeof(s1)).finalize().toString());

	const char s2[144] = {0};
	REQUIRE("0b9845dd429566cdab772ba195d271effe2d0211f16991d766ba749447c5cde569780b2daa66c4b224a2ec2e5d09174c"
			== Hash().addData(s2, sizeof(s2)).finalize().toString());


	// my own tests
	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("22ccce05b0ac1ceda1b0b0dc0021e6a3957779260cd2fae8a21c3d5432ce204f1df2d62a00d8f505cae6e70e3be18701"
			== test3.finalize().toString());

	REQUIRE("c6cbd89c926ab525c242e6621f2f5fa73aa4afe3d9e24aed727faaadd6af38b620bdb623dd2b4788b1c8086984af8706"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("67c9e8ef665d11b5b57a1d99c96adffb3034d8768c0827d1c6e60b54871e8673651767a2c6c43d0ba2a9bb2500227406"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("73c9a55749f42ace6caf83a0e421b7b52ec473b7f6c48774cc6857d2f471b43c8f0e7b7e416c2df934713a4947ca27af"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("19443cc895a288c114416e192bb0c5504e33f1733d30e436e28afa942c5b047a860bc3113323614dd6e23a9e6917dde6"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(128, 'a');  // length == BLOCK_SIZE
	REQUIRE("787b569ec1ff86d46f298c25957e182047ca82bd50149dcdb4c53c070d10d62a07f15c73ca0b0cdacc8f3b4d1862d86c"
			== Hash().addData(s14.data(), s14.size()).finalize().toString());

	const std::vector<char> s15(129, 'a');
	REQUIRE("787b569ec1ff86d46f298c25957e182047ca82bd50149dcdb4c53c070d10d62a07f15c73ca0b0cdacc8f3b4d1862d86c"
			== Hash().addData(s15.data() + 1, s15.size() - 1).finalize().toString());

	const int s16[2] = {0};
	const char s16_2[8] = {0};
	REQUIRE(Hash().addData(gsl::span<const int>(s16)).finalize().toString()
			== Hash().addData(s16_2).finalize().toString());
}
