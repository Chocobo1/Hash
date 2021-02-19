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

#include "../src/has_160.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("has-160")
{
	using Hash = Chocobo1::HAS_160;

	// official test suite from has-160 spec
	const char s1[] = "";
	REQUIRE("307964ef34151d37c8047adec7ab50f4ff89762d" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("4872bcbc4cd0f0a9dc7c2f7045e5b43b6c830db8" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("975e810488cf2a3d49838478124afce4b1c78804" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("2338dbc8638d31225f73086246ba529f96710bc6" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("596185c9ab6703d0d0dbb98702bc0f5729cd1d3c" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("cb5d7efbca2f02e0fb7167cabb123af5795764e5" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "1234567890";
	Hash test7;
	for (int i = 0 ; i < 8; ++i)
		test7.addData(s7, strlen(s7));
	REQUIRE("07f05c8c0773c55ca3a5a695ce6aca4c438911b5" == test7.finalize().toString());

	const char s8[] = "a";
	Hash test8;
	for (long int i = 0 ; i < 1000000; ++i)
		test8.addData(s8, strlen(s8));
	REQUIRE("d6ad6f0608b878da9b87999c2525cc84f4c9f18d" == test8.finalize().toString());


	// my own tests
	REQUIRE("307964ef34151d37c8047adec7ab50f4ff89762d" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("abe2b8c711f9e8579aa8eb40757a27b4ef14a7ea" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("5f335d8b5debe5618e1e7b4f267eb8a6dececedb" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("8a2fb9250c69ef66db41b65f06649554f565c24b" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("d98a869c1f27711aec9f06d93450e6318db1ef64"
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
	REQUIRE("5d8db63ebe00ecca2ec655ffcf4c2b0c585cf3e8"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());
}

