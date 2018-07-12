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

#include "../src/sha1.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


#define ARRAY_LENGTH(a) (std::extent<decltype(a)>::value)


TEST_CASE("sha1")
{
	using Hash = Chocobo1::SHA1;

	// official test suite in rfc
	const char s1[] = "abc";
	REQUIRE("a9993e364706816aba3e25717850c26c9cd0d89d" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	REQUIRE("84983e441c3bd26ebaae4aa1f95129e5e54670f1" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("34aa973cd4c4daa4f61eeb2bdbad27316534016f" == test3.finalize().toString());

	const char s4[] = "0123456701234567012345670123456701234567012345670123456701234567";
	Hash test4;
	for (int i = 0 ; i < 10; ++i)
		test4.addData(s4, strlen(s4));
	REQUIRE("dea356a2cddd90c7a7ecedc5ebb563934f460452" == test4.finalize().toString());

	const char s6[] = "\x5e";
	REQUIRE("5e6f80a34a9798cafc6a5db96cc57ba4c4db59c2" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s8[] = "\x9a\x7d\xfd\xf1\xec\xea\xd0\x6e\xd6\x46\xaa\x55\xfe\x75\x71\x46";
	REQUIRE("82abff6605dbe1c17def12a394fa22a82b544a35" == Hash().addData(s8, strlen(s8)).finalize().toString());

	const char s10[] = "\xf7\x8f\x92\x14\x1b\xcd\x17\x0a\xe8\x9b\x4f\xba\x15\xa1\xd5\x9f" \
					   "\x3f\xd8\x4d\x22\x3c\x92\x51\xbd\xac\xbb\xae\x61\xd0\x5e\xd1\x15" \
					   "\xa0\x6a\x7c\xe1\x17\xb7\xbe\xea\xd2\x44\x21\xde\xd9\xc3\x25\x92" \
					   "\xbd\x57\xed\xea\xe3\x9c\x39\xfa\x1f\xe8\x94\x6a\x84\xd0\xcf\x1f" \
					   "\x7b\xee\xad\x17\x13\xe2\xe0\x95\x98\x97\x34\x7f\x67\xc8\x0b\x04" \
					   "\x00\xc2\x09\x81\x5d\x6b\x10\xa6\x83\x83\x6f\xd5\x56\x2a\x56\xca" \
					   "\xb1\xa2\x8e\x81\xb6\x57\x66\x54\x63\x1c\xf1\x65\x66\xb8\x6e\x3b" \
					   "\x33\xa1\x08\xb0\x53\x07\xc0\x0a\xff\x14\xa7\x68\xed\x73\x50\x60" \
					   "\x6a\x0f\x85\xe6\xa9\x1d\x39\x6f\x5b\x5c\xbe\x57\x7f\x9b\x38\x80" \
					   "\x7c\x7d\x52\x3d\x6d\x79\x2f\x6e\xbc\x24\xa4\xec\xf2\xb3\xa4\x27" \
					   "\xcd\xbb\xfb";
	REQUIRE("cb0082c8f197d260991ba6a460e76e202bad27b3" == Hash().addData(s10, (ARRAY_LENGTH(s10) - 1)).finalize().toString());


	// my own tests
	REQUIRE("da39a3ee5e6b4b0d3255bfef95601890afd80709" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("408d94384216f890ff7a0c3528e8bed1e0b01621" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("d42f858ad812fd986fd8dc7216af5f88bcaa1463" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("34aa973cd4c4daa4f61eeb2bdbad27316534016f"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(gsl::span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());
}
