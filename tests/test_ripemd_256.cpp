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

#include "../src/ripemd_256.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("ripemd-256")
{
	using Hash = Chocobo1::RIPEMD_256;

	// official test suite from ripemd-160 webiste
	const char s1[] = "";
	REQUIRE("02ba4c4e5f8ecd1877fc52d64d30e37a2d9774fb1e5d026380ae0168e3c5522d" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("f9333e45d857f5d90a91bab70a1eba0cfb1be4b0783c9acfcd883a9134692925" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("afbd6e228b9d8cbbcef5ca2d03e6dba10ac0bc7dcbe4680e1e42d2e975459b65" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("87e971759a1ce47a514d5c914c392c9018c7c46bc14465554afcdf54a5070c0e" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("649d3034751ea216776bf9a18acc81bc7896118a5197968782dd1fd97d8d5133" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	REQUIRE("3843045583aac6c8c8d9128573e7a9809afb2a0f34ccc36ea9e72f16f6368e3f" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("5740a408ac16b720b84424ae931cbb1fe363d1d0bf4017f1a89f7ea6de77a0b8" == Hash().addData(s7, strlen(s7)).finalize().toString());

	const char s8[] = "1234567890";
	Hash test8;
	for (int i = 0 ; i < 8; ++i)
		test8.addData(s8, strlen(s8));
	REQUIRE("06fdcc7a409548aaf91368c06a6275b553e3f099bf0ea4edfd6778df89a890dd" == test8.finalize().toString());

	const char s9[] = "a";
	Hash test9;
	for (long int i = 0 ; i < 1000000; ++i)
		test9.addData(s9, strlen(s9));
	REQUIRE("ac953744e10e31514c150d4d8d7b677342e33399788296e43ae4850ce4f97978" == test9.finalize().toString());


	// my own tests
	REQUIRE("02ba4c4e5f8ecd1877fc52d64d30e37a2d9774fb1e5d026380ae0168e3c5522d" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("c3b0c2f764ac6d576a6c430fb61a6f2255b4fa833e094b1ba8c1e29b6353036f" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("379e373d9e1b6e71712b8f4a19b8fb125caa3f4ce92a258eb764d721d9a08bad" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("6b7f038ccb610c829f4e7a7dcc07dd3f3d3e2d2869b1ffbbd6a3eda828aba861" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("ac953744e10e31514c150d4d8d7b677342e33399788296e43ae4850ce4f97978"
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
