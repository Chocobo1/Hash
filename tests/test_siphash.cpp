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

#include "../src/siphash.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("siphash")
{
	using Hash = Chocobo1::SipHash;

	// official test from "SipHash: a fast short-input PRF" specification appendix A "Test values"
	const unsigned char key1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
	const unsigned char s1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E};
	REQUIRE("a129ca6149be45e5" == Hash(key1).addData(s1).finalize().toString());

	// my own tests
	const unsigned char key2[16] = {};
	REQUIRE("1e924b9d737700d7" == Hash(key2).finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("590052062570e40d" == Hash(key2).addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("04881a54901382b2" == Hash(key2).addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("422152e3bef97797" == Hash(key2).addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("f82add0bfc05de4a"
			== Hash(key2).addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash(key2).addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash(key2).addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash(key2).addData(s16, 2).finalize().toArray();
	const auto s16_2 = Hash(key2).addData(s16).finalize().toArray();
	REQUIRE(s16_1 == s16_2);

	const char s17[7] = {0};
	REQUIRE("85f896b6040a13ec"
			== Hash(key2).addData(s17, sizeof(s17)).finalize().toString());
}
