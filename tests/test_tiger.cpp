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

#include "../src/tiger.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("tiger1-192")
{
	using Hash = Chocobo1::Tiger1_192;

	// official test suite from tiger website
	const char s1[] = "";
	REQUIRE("3293ac630c13f0245f92bbb1766e16167a4e58492dde73f3" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("77befbef2e7ef8ab2ec8f93bf587a7fc613e247f5f247809" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("2aab1484e8c158f2bfb8c5ff41b57a525129131c957b5f93" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("d981f8cb78201a950dcf3048751e441c517fca1aa55a29f6" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("1714a472eee57d30040412bfcc55032a0b11602ff37beee9" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	REQUIRE("0f7bf9a19b9c58f2b7610df7e84f0ac3a71c631e7b53f78e" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("8dcea680a17583ee502ba38a3c368651890ffbccdc49a8cc" == Hash().addData(s7, strlen(s7)).finalize().toString());

	const char s8[] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
	REQUIRE("1c14795529fd9f207a958f84c52f11e887fa0cabdfd91bfd" == Hash().addData(s8, strlen(s8)).finalize().toString());

	const char s9[] = "a";
	Hash test9;
	for (long int i = 0 ; i < 1000000; ++i)
		test9.addData(s9, strlen(s9));
	REQUIRE("6db0e2729cbead93d715c6a7d36302e9b3cee0d2bc314b41" == test9.finalize().toString());


	// my own tests
	REQUIRE("3293ac630c13f0245f92bbb1766e16167a4e58492dde73f3" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("6d12a41e72e644f017b6f0e2f7b44c6285f06dd5d2c5b075" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("0bf46f237681b35301d46aa08d43c449643408521a263929" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("b68a2822f8f79df4dd12cd7261bce1a022297af27958d304" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("6db0e2729cbead93d715c6a7d36302e9b3cee0d2bc314b41"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(gsl::span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());
}


TEST_CASE("tiger2-192")
{
	using Hash = Chocobo1::Tiger2_192;

	// official test suite from tiger website
	const char s1[] = "";
	REQUIRE("4441be75f6018773c206c22745374b924aa8313fef919f41" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("67e6ae8e9e968999f70a23e72aeaa9251cbc7c78a7916636" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("f68d7bc5af4b43a06e048d7829560d4a9415658bb0b1f3bf" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("e29419a1b5fa259de8005e7de75078ea81a542ef2552462d" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("f5b6b6a78c405c8547e91cd8624cb8be83fc804a474488fd" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	REQUIRE("a6737f3997e8fbb63d20d2df88f86376b5fe2d5ce36646a9" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("ea9ab6228cee7b51b77544fca6066c8cbb5bbae6319505cd" == Hash().addData(s7, strlen(s7)).finalize().toString());

	const char s8[] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
	REQUIRE("d85278115329ebaa0eec85ecdc5396fda8aa3a5820942fff" == Hash().addData(s8, strlen(s8)).finalize().toString());

	const char s9[] = "a";
	Hash test9;
	for (long int i = 0 ; i < 1000000; ++i)
		test9.addData(s9, strlen(s9));
	REQUIRE("e068281f060f551628cc5715b9d0226796914d45f7717cf4" == test9.finalize().toString());


	// my own tests
	REQUIRE("4441be75f6018773c206c22745374b924aa8313fef919f41" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("976abff8062a2e9dcea3a1ace966ed9c19cb85558b4976d8" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("7c19bd4093a3106de04cc99f125ee6ac8d52ade31ae85fc5" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("2ba7384e75cb63b58a52964208fdefc692b932a00c04bcfb" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("e068281f060f551628cc5715b9d0226796914d45f7717cf4"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(gsl::span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());
}
