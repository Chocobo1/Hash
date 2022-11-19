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

#include "../src/sha2_512_256.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("sha2-512/256")  // NOLINT
{
	using Hash = Chocobo1::SHA2_512_256;

	// official test suite on NIST.gov
	const char s1[] = "abc";
	REQUIRE("53048e2681941ef99b2e29b76b4c7dabe4c2d0c634fc6d46e0e2f13107e7af23"
			== Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
	REQUIRE("3928e184fb8690f840da3988121d31be65cb9d3ef83ee6146feac861e19b563a"
			== Hash().addData(s2, strlen(s2)).finalize().toString());


	// my own tests
	REQUIRE(Hash() == Hash());
	REQUIRE(Hash().addData("123").finalize() != Hash().finalize());

	REQUIRE("c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("dd9d67b371519c339ed8dbd25af90e976a1eeefd4ad3d889005e532fc5bef04d"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("1546741840f8a492b959d9b8b2344b9b0eb51b004bba35c0aebaac86d45264c3"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("35591989d960896a3d36207256da463aa85aeca2401def10f9c190bcba8fc187"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(65, 'a');
	REQUIRE("2c3fa8f196f2aac65f15166666ecc77bd9fe195bae83ef06bb75c7857c163db9"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toArray();
	const auto s16_2 = Hash().addData(s16).finalize().toArray();
	REQUIRE(s16_1 == s16_2);

	const char s17[111] = {0};
	REQUIRE("5192ee5471d8a02ffc34bce87142df77aaef777dde522cc171af66e95a006a15"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());

	REQUIRE(0xc672b8d1ef56ed28 == std::hash<Hash> {}(Hash().finalize()));
}
