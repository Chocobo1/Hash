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

#include "../src/ripemd_160.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("ripemd-160")
{
	using Hash = Chocobo1::RIPEMD_160;

	// official test suite from ripemd-160 webiste
	const char s1[] = "";
	REQUIRE("9c1185a5c5e9fc54612808977ee8f548b2258d31" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("0bdc9d2d256b3ee9daae347be6f4dc835a467ffe" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("8eb208f7e05d987a9b044a8e98c6b087f15a0bfc" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("5d0689ef49d2fae572b881b123a85ffa21595f36" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("f71c27109c692c1b56bbdceb5b9d2865b3708dbc" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	REQUIRE("12a053384a9c0c88e405a06c27dcf49ada62eb2b" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("b0e20b6e3116640286ed3a87a5713079b21f5189" == Hash().addData(s7, strlen(s7)).finalize().toString());

	const char s8[] = "1234567890";
	Hash test8;
	for (int i = 0 ; i < 8; ++i)
		test8.addData(s8, strlen(s8));
	REQUIRE("9b752e45573d4b39f4dbd3323cab82bf63326bfb" == test8.finalize().toString());

	const char s9[] = "a";
	Hash test9;
	for (long int i = 0 ; i < 1000000; ++i)
		test9.addData(s9, strlen(s9));
	REQUIRE("52783243c1697bdbe16d37f97f68f08325dc1528" == test9.finalize().toString());


	// my own tests
	REQUIRE("9c1185a5c5e9fc54612808977ee8f548b2258d31" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("37f332f68db77bd9d7edd4969571ad671cf9dd3b" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("fc850169b1f2ce72e3f8aa0aeb5ca87d6f8519c6" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("80407833d8f49264dcf4de5c019b3bf9942cc040" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("52783243c1697bdbe16d37f97f68f08325dc1528"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toArray();
	const auto s16_2 = Hash().addData(s16).finalize().toArray();
	REQUIRE(s16_1 == s16_2);

	const char s17[55] = {0};
	REQUIRE("e323d78db60afc7404def79abb82b8fb73591037"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());
}
