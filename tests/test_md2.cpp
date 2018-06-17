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

#include "../src/md2.h"

#include "catch/single_include/catch.hpp"

#include <cstring>


TEST_CASE("md2")
{
	using Hash = Chocobo1::MD2;

	// official test suite in rfc
	const char s1[] = "";
	REQUIRE("8350e5a3e24c153df2275c9f80692773" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("32ec01ec4a6dac72c0ab96fb34c0b5d1" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("da853b0d3f88d99b30283a69e6ded6bb" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("ab4f496bfb2a530b219ff33031fe06b0" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("4e8ddff3650292ab5a4108c3aa47940b" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("da33def2a42df13975352846c30338cd" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
	REQUIRE("d5976f79d83d3a0dc9806c3c66f3efd8" == Hash().addData(s7, strlen(s7)).finalize().toString());


	// my own tests
	REQUIRE("8350e5a3e24c153df2275c9f80692773" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("03d85a0d629d2c442e987525319fc471" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("71eaa7e440b611e41a6f0d97384b342a" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("a7e3d94a42f4cf37aca6ef3fda6882c6" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("14db72af1a6b6290199f6be37fd78339"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(gsl::span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());
}
