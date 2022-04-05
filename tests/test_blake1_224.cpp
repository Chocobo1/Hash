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

#include "../src/blake1_224.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("blake1-224")
{
	using Hash = Chocobo1::Blake1_224;

	// official test suite from blake1 SHA3-proposal
	const char s1[1] = {0};
	REQUIRE("4504cb0314fb2a4f7a692e696e487912fe3f2468fe312c73a5278ec5"
			== Hash().addData(s1, sizeof(s1)).finalize().toString());

	const char s2[72] = {0};
	REQUIRE("f5aa00dd1cb847e3140372af7b5c46b4888d82c8c0a917913cfb5d04"
			== Hash().addData(s2, sizeof(s2)).finalize().toString());


	// my own tests
	REQUIRE(Hash() == Hash());
	REQUIRE(Hash().addData("123").finalize() != Hash().finalize());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("738443f8093ae703ebe4fe991b4f00208701e2e7be1275fd1bd84ef1"
			== test3.finalize().toString());

	REQUIRE("7dc5313b1c04512a174bd6503b89607aecbee0903d40a8a569c94eed"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("c8e92d7088ef87c1530aee2ad44dc720cc10589cc2ec58f95a15e51b"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("91e670c2da5728a9cdc2e92cd9215393d59abf053843e49d4ce204f2"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("abe9cb6525ff22cba701fc7a20e215c4c2782d2a832b46e390344d8f"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const char s14[] = "a";
	Hash test14;
	for (long int i = 0 ; i < 64; ++i)
		test14.addData(s14, strlen(s14));
	REQUIRE("28ae307b62eb14a5c50d83c4f6fbe04dd30a5f8c08454f59b0ab7afc"
			== test14.finalize().toString());

	const std::vector<char> s15(65, 'a');
	REQUIRE("28ae307b62eb14a5c50d83c4f6fbe04dd30a5f8c08454f59b0ab7afc"
			== Hash().addData(s15.data() + 1, s15.size() - 1).finalize().toString());

	const int s16[2] = {0};
	const char s16_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s16)).finalize().toString()
			== Hash().addData(s16_2).finalize().toString());

	const unsigned char s17[] = {0x00, 0x0A};
	const auto s17_1 = Hash().addData(s17, 2).finalize().toArray();
	const auto s17_2 = Hash().addData(s17).finalize().toArray();
	REQUIRE(s17_1 == s17_2);

	const char s18[55] = {0};
	REQUIRE("502a0663e562d1cda878b9fe86e6c475f7399e12379526be742b1c93"
			== Hash().addData(s18, sizeof(s18)).finalize().toString());

	REQUIRE(0x7dc5313b1c04512a == std::hash<Hash> {}(Hash().finalize()));
}
