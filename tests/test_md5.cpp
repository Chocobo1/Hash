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

#include "../src/md5.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("md5")
{
	using Hash = Chocobo1::MD5;

	// official test suite in rfc
	const char s1[] = "";
	REQUIRE("d41d8cd98f00b204e9800998ecf8427e" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "a";
	REQUIRE("0cc175b9c0f1b6a831c399e269772661" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "abc";
	REQUIRE("900150983cd24fb0d6963f7d28e17f72" == Hash().addData(s3, strlen(s3)).finalize().toString());

	const char s4[] = "message digest";
	REQUIRE("f96b697d7cb7938d525a2f31aaf161d0" == Hash().addData(s4, strlen(s4)).finalize().toString());

	const char s5[] = "abcdefghijklmnopqrstuvwxyz";
	REQUIRE("c3fcd3d76192e4007dfb496cca67e13b" == Hash().addData(s5, strlen(s5)).finalize().toString());

	const char s6[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	REQUIRE("d174ab98d277d9f5a5611c2c9f419d9f" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s7[] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
	REQUIRE("57edf4a22be3c955ac49da2e2107b67a" == Hash().addData(s7, strlen(s7)).finalize().toString());


	// my own tests
	REQUIRE("d41d8cd98f00b204e9800998ecf8427e" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("9e107d9d372bb6826bd81d3542a419d6" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("e4d909c290d0fb1ca068ffaddf22cbd0" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("4e67db4a7a406b0cfdadd887cde7888e" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("014842d480b571495a4a0363793f7367"
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
	REQUIRE("c9ea3314b91c9fd4e38f9432064fd1f2"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());

	REQUIRE(0xd41d8cd98f00b204 == std::hash<Hash> {}(Hash().finalize()));
}
