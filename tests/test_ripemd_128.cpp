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

#include "../src/ripemd_128.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("ripemd-128")
{
	using Hash = Chocobo1::RIPEMD_128;

	// official test suite from ripemd-160 webiste
	const char s1[] = "";
	REQUIRE("cdf26213a150dc3ecb610f18f6b38b46" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("86be7afa339d0fc7cfc785e72f578d33" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("c14a12199c66e4ba84636b0f69144c77" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("9e327b3d6e523062afc1132d7df9d1b8" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("fd2aa607f71dc8f510714922b371834e" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	REQUIRE("a1aa0689d0fafa2ddc22e88b49133a06" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("d1e959eb179c911faea4624c60c5c702" == Hash().addData(s7, strlen(s7)).finalize().toString());

	const char s8[] = "1234567890";
	Hash test8;
	for (int i = 0 ; i < 8; ++i)
		test8.addData(s8, strlen(s8));
	REQUIRE("3f45ef194732c2dbb2c4a2c769795fa3" == test8.finalize().toString());

	const char s9[] = "a";
	Hash test9;
	for (long int i = 0 ; i < 1000000; ++i)
		test9.addData(s9, strlen(s9));
	REQUIRE("4a7f5723f954eba1216c9d8f6320431f" == test9.finalize().toString());


	// my own tests
	REQUIRE("cdf26213a150dc3ecb610f18f6b38b46" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("3fa9b57f053c053fbe2735b2380db596" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("f39288a385e5996c6fb2c01f7f8fbf2f" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("508b5dac78e7aa731d8a4fb8ac4e6d48" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("4a7f5723f954eba1216c9d8f6320431f"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(gsl::span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());
}
