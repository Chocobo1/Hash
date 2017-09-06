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

#include "../src/md4.h"

#include "catch/single_include/catch.hpp"

#include <cstring>


TEST_CASE("md4")
{
	using Hash = Chocobo1::MD4;

	// official test suite in rfc
	const char s1[] = "";
	REQUIRE("31d6cfe0d16ae931b73c59d7e0c089c0" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("bde52cb31de33e46245e05fbdbd6fb24" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("a448017aaf21d8525fc10ae87aa6729d" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("d9130a8164549fe818874806e1c7014b" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("d79e1c308aa5bbcdeea8ed63df412da9" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("043f8582f241db351ce627e153e7f0e4" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
	REQUIRE("e33b4ddc9c38f2199c3e7b164fcc0536" == Hash().addData(s7, strlen(s7)).finalize().toString());


	// collision
	const char k1[] = "\x83\x9c\x7a\x4d\x7a\x92\xcb\x56\x78\xa5\xd5\xb9\xee\xa5\xa7\x57\x3c\x8a\x74\xde\xb3\x66\xc3\xdc\x20\xa0\x83\xb6\x9f\x5d\x2a\x3b\xb3\x71\x9d\xc6\x98\x91\xe9\xf9\x5e\x80\x9f\xd7\xe8\xb2\x3b\xa6\x31\x8e\xdd\x45\xe5\x1f\xe3\x97\x08\xbf\x94\x27\xe9\xc3\xe8\xb9";
	const char k2[] = "\x83\x9c\x7a\x4d\x7a\x92\xcb\xd6\x78\xa5\xd5\x29\xee\xa5\xa7\x57\x3c\x8a\x74\xde\xb3\x66\xc3\xdc\x20\xa0\x83\xb6\x9f\x5d\x2a\x3b\xb3\x71\x9d\xc6\x98\x91\xe9\xf9\x5e\x80\x9f\xd7\xe8\xb2\x3b\xa6\x31\x8e\xdc\x45\xe5\x1f\xe3\x97\x08\xbf\x94\x27\xe9\xc3\xe8\xb9";
	REQUIRE(Hash().addData(k1, strlen(k1)).finalize().toString() == Hash().addData(k2, strlen(k2)).finalize().toString());


	// my own tests
	REQUIRE("31d6cfe0d16ae931b73c59d7e0c089c0" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("1bee69a46ba811185c194762abaeae90" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("2812c6c7136898c51f6f6739ad08750e" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("2edf6465507c0176b90c87d19a7d988e" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("52f5076fabd22680234a3fa9f9dc5732"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());
}
