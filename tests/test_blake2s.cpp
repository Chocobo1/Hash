/*
 *  Chocobo1/Hash
 *
 *   Copyright 2017-2018 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

#include "../src/blake2s.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("blake2s")
{
	using Hash = Chocobo1::Blake2s;

	// official test suite in rfc
	const char s1[] = "abc";
	REQUIRE("508c5e8c327c14e2e1a72ba34eeb452f37458b209ed63a294d999b4c86675982"
			== Hash().addData(s1, strlen(s1)).finalize().toString());


	// my own tests
	REQUIRE(Hash() == Hash());
	REQUIRE(Hash().addData("123").finalize() != Hash().finalize());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("bec0c0e6cde5b67acb73b81f79a67a4079ae1c60dac9d2661af18e9f8b50dfa5"
			== test3.finalize().toString());

	REQUIRE("69217a3079908094e11121d042354a7c1f55b6482ca1a51e1b250dfd1ed0eef9"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("606beeec743ccbeff6cbcdf5d5302aa855c256c29b88c8ed331ea1a6bf3c8812"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("95bca6e1b761dca1323505cc629949a0e03edf11633cc7935bd8b56f393afcf2"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("512c6a77330f65b279a42f7a30dddb3793367cc2a73bca12c0dff3a910c38660"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(64, 'a');  // length == BLOCK_SIZE
	REQUIRE("651d2f5f20952eacaea2fba2f2af2bcd633e511ea2d2e4c9ae2ac0d9ffb7b252"
			== Hash().addData(s14.data(), s14.size()).finalize().toString());

	const std::vector<char> s15(65, 'a');
	REQUIRE("651d2f5f20952eacaea2fba2f2af2bcd633e511ea2d2e4c9ae2ac0d9ffb7b252"
			== Hash().addData(s15.data() + 1, s15.size() - 1).finalize().toString());

	const int s16[2] = {0};
	const char s16_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s16)).finalize().toString()
			== Hash().addData(s16_2).finalize().toString());

	const unsigned char s17[] = {0x00, 0x0A};
	const auto s17_1 = Hash().addData(s17, 2).finalize().toArray();
	const auto s17_2 = Hash().addData(s17).finalize().toArray();
	REQUIRE(s17_1 == s17_2);

	REQUIRE(0x69217a3079908094 == std::hash<Hash> {}(Hash().finalize()));
}
