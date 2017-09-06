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

#include "../src/sha2_224.h"

#include "catch/single_include/catch.hpp"

#include <cstring>


#define ARRAY_LENGTH(a) (std::extent<decltype(a)>::value)


TEST_CASE("sha2-224")
{
	using Hash = Chocobo1::SHA2_224;

	// official test suite in rfc
	const char s1[] = "abc";
	REQUIRE("23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	REQUIRE("75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67" == test3.finalize().toString());

	const char s4[] = "0123456701234567012345670123456701234567012345670123456701234567";
	Hash test4;
	for (int i = 0 ; i < 10; ++i)
		test4.addData(s4, strlen(s4));
	REQUIRE("567f69f168cd7844e65259ce658fe7aadfa25216e68eca0eb7ab8262" == test4.finalize().toString());

	const char s6[] = "\x07";
	REQUIRE("00ecd5f138422b8ad74c9799fd826c531bad2fcabc7450bee2aa8c2a" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s8[] = "\x18\x80\x40\x05\xdd\x4f\xbd\x15\x56\x29\x9d\x6f\x9d\x93\xdf\x62";
	REQUIRE("df90d78aa78821c99b40ba4c966921accd8ffb1e98ac388e56191db1" == Hash().addData(s8, strlen(s8)).finalize().toString());

	const char s10[] = "\x55\xb2\x10\x07\x9c\x61\xb5\x3a\xdd\x52\x06\x22\xd1\xac\x97\xd5" \
					   "\xcd\xbe\x8c\xb3\x3a\xa0\xae\x34\x45\x17\xbe\xe4\xd7\xba\x09\xab" \
					   "\xc8\x53\x3c\x52\x50\x88\x7a\x43\xbe\xbb\xac\x90\x6c\x2e\x18\x37" \
					   "\xf2\x6b\x36\xa5\x9a\xe3\xbe\x78\x14\xd5\x06\x89\x6b\x71\x8b\x2a" \
					   "\x38\x3e\xcd\xac\x16\xb9\x61\x25\x55\x3f\x41\x6f\xf3\x2c\x66\x74" \
					   "\xc7\x45\x99\xa9\x00\x53\x86\xd9\xce\x11\x12\x24\x5f\x48\xee\x47" \
					   "\x0d\x39\x6c\x1e\xd6\x3b\x92\x67\x0c\xa5\x6e\xc8\x4d\xee\xa8\x14" \
					   "\xb6\x13\x5e\xca\x54\x39\x2b\xde\xdb\x94\x89\xbc\x9b\x87\x5a\x8b" \
					   "\xaf\x0d\xc1\xae\x78\x57\x36\x91\x4a\xb7\xda\xa2\x64\xbc\x07\x9d" \
					   "\x26\x9f\x2c\x0d\x7e\xdd\xd8\x10\xa4\x26\x14\x5a\x07\x76\xf6\x7c" \
					   "\x87\x82\x73";
	REQUIRE("0b31894ec8937ad9b91bdfbcba294d9adefaa18e09305e9f20d5c3a4" == Hash().addData(s10, (ARRAY_LENGTH(s10) - 1)).finalize().toString());


	// my own tests
	REQUIRE("d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("730e109bd7a8a32b1cb9d9a09aa2325d2430587ddbc0c38bad911525" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("619cba8e8e05826e9b8c519c0a5c68f4fb653e8a3d8aa04bb2c8cd4c" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("6d2713ab2f16295926fddb9cc3d06c915858ad898396cc886155e244" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());
}
