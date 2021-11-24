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

#include "../src/crc_32.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("crc-32")
{
	using Hash = Chocobo1::CRC_32;

	// my own tests
	REQUIRE("00000000" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("414fa339" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("519025e9" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("d99691f3" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("89b46555"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toArray();
	const auto s16_2 = Hash().addData(s16).finalize().toArray();
	REQUIRE(s16_1 == s16_2);

	REQUIRE(0 == std::hash<Hash> {}(Hash().finalize()));
}
