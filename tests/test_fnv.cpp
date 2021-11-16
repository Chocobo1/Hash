/*
 *  Chocobo1/Hash
 *
 *   Copyright 2021 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

#include "../src/fnv.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("fnv32_0")
{
	using Hash = Chocobo1::FNV32_0;

	// official test suite in rfc
	const char s1[] = "chongo <Landon Curt Noll> /\\../\\";
	REQUIRE("811c9dc5" == Hash().addData(s1, strlen(s1)).finalize().toString());
}


TEST_CASE("fnv32_1")
{
	using Hash = Chocobo1::FNV32_1;

	// my own tests
	REQUIRE("811c9dc5" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("e9c86c6e" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("7482b104" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("a48abd48" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("5d8e2b05"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toArray();
	const auto s16_2 = Hash().addData(s16).finalize().toArray();
	REQUIRE(s16_1 == s16_2);
}


TEST_CASE("fnv32_1a")
{
	using Hash = Chocobo1::FNV32_1a;

	// official test suite in rfc
	const char s1[] = "";
	REQUIRE("811c9dc5" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("e40c292c" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "foobar";
	REQUIRE("bf9cf968" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "";
	REQUIRE("050c5d1f" == Hash().addData(s1, (strlen(s4) + 1)).finalize().toString());

	const char s5[] = "a";
	REQUIRE("2b24d044" == Hash().addData(s2, (strlen(s5) + 1)).finalize().toString());

	const char s6[] = "foobar";
	REQUIRE("0c1c9eb8" == Hash().addData(s3, (strlen(s6) + 1)).finalize().toString());

	// my own tests
	REQUIRE("811c9dc5" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("048fff90" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("ecaf981a" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("dfd12786" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("d96f0f85"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toArray();
	const auto s16_2 = Hash().addData(s16).finalize().toArray();
	REQUIRE(s16_1 == s16_2);
}


TEST_CASE("fnv64_0")
{
	using Hash = Chocobo1::FNV64_0;

	// official test suite in rfc
	const char s1[] = "chongo <Landon Curt Noll> /\\../\\";
	REQUIRE("cbf29ce484222325" == Hash().addData(s1, strlen(s1)).finalize().toString());
}


TEST_CASE("fnv64_1")
{
	using Hash = Chocobo1::FNV64_1;

	// my own tests
	REQUIRE("cbf29ce484222325" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("a8b2f3117de37ace" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("8b8dd4b8e989ac24" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("b68fdcf39b783288" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("2cdecc1e8b312e65"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toArray();
	const auto s16_2 = Hash().addData(s16).finalize().toArray();
	REQUIRE(s16_1 == s16_2);
}


TEST_CASE("fnv64_1a")
{
	using Hash = Chocobo1::FNV64_1a;

	// official test suite in rfc
	const char s1[] = "";
	REQUIRE("cbf29ce484222325" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("af63dc4c8601ec8c" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "foobar";
	REQUIRE("85944171f73967e8" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "";
	REQUIRE("af63bd4c8601b7df" == Hash().addData(s1, (strlen(s4) + 1)).finalize().toString());

	const char s5[] = "a";
	REQUIRE("089be207b544f1e4" == Hash().addData(s2, (strlen(s5) + 1)).finalize().toString());

	const char s6[] = "foobar";
	REQUIRE("34531ca7168b8f38" == Hash().addData(s3, (strlen(s6) + 1)).finalize().toString());

	// my own tests
	REQUIRE("cbf29ce484222325" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("f3f9b7f5e7e47110" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("75c4d4d9092c6c5a" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("2f4442aa5eedc866" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("db007849f35ebbe5"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toArray();
	const auto s16_2 = Hash().addData(s16).finalize().toArray();
	REQUIRE(s16_1 == s16_2);
}
