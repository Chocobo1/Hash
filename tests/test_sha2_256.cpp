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

#include "../src/sha2_256.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


#define ARRAY_LENGTH(a) (static_cast<int>(std::extent<decltype(a)>::value))


TEST_CASE("sha2-256")  // NOLINT
{
	using Hash = Chocobo1::SHA2_256;

	// official test suite in rfc
	const char s1[] = "abc";
	REQUIRE("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" == Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	REQUIRE("248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1" == Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0" == test3.finalize().toString());

	const char s4[] = "0123456701234567012345670123456701234567012345670123456701234567";
	Hash test4;
	for (int i = 0 ; i < 10; ++i)
		test4.addData(s4, strlen(s4));
	REQUIRE("594847328451bdfa85056225462cc1d867d877fb388df0ce35f25ab5562bfbb5" == test4.finalize().toString());

	const char s6[] = "\x19";
	REQUIRE("68aa2e2ee5dff96e3355e6c7ee373e3d6a4e17f75f9518d843709c0c9bc3e3d4" == Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s8[] = "\xe3\xd7\x25\x70\xdc\xdd\x78\x7c\xe3\x88\x7a\xb2\xcd\x68\x46\x52";
	REQUIRE("175ee69b02ba9b58e2b0a5fd13819cea573f3940a94f825128cf4209beabb4e8" == Hash().addData(s8, strlen(s8)).finalize().toString());

	const char s10[] = "\x83\x26\x75\x4e\x22\x77\x37\x2f\x4f\xc1\x2b\x20\x52\x7a\xfe\xf0" \
					   "\x4d\x8a\x05\x69\x71\xb1\x1a\xd5\x71\x23\xa7\xc1\x37\x76\x00\x00" \
					   "\xd7\xbe\xf6\xf3\xc1\xf7\xa9\x08\x3a\xa3\x9d\x81\x0d\xb3\x10\x77" \
					   "\x7d\xab\x8b\x1e\x7f\x02\xb8\x4a\x26\xc7\x73\x32\x5f\x8b\x23\x74" \
					   "\xde\x7a\x4b\x5a\x58\xcb\x5c\x5c\xf3\x5b\xce\xe6\xfb\x94\x6e\x5b" \
					   "\xd6\x94\xfa\x59\x3a\x8b\xeb\x3f\x9d\x65\x92\xec\xed\xaa\x66\xca" \
					   "\x82\xa2\x9d\x0c\x51\xbc\xf9\x33\x62\x30\xe5\xd7\x84\xe4\xc0\xa4" \
					   "\x3f\x8d\x79\xa3\x0a\x16\x5c\xba\xbe\x45\x2b\x77\x4b\x9c\x71\x09" \
					   "\xa9\x7d\x13\x8f\x12\x92\x28\x96\x6f\x6c\x0a\xdc\x10\x6a\xad\x5a" \
					   "\x9f\xdd\x30\x82\x57\x69\xb2\xc6\x71\xaf\x67\x59\xdf\x28\xeb\x39" \
					   "\x3d\x54\xd6";
	REQUIRE("97dbca7df46d62c8a422c941dd7e835b8ad3361763f7e9b2d95f4f0da6e1ccbc" == Hash().addData(s10, (ARRAY_LENGTH(s10) - 1)).finalize().toString());


	// my own tests
	REQUIRE(Hash() == Hash());
	REQUIRE(Hash().addData("123").finalize() != Hash().finalize());

	REQUIRE("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" == Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592" == Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("ef537f25c895bfa782526529a9b63d97aa631564d5d789c2b765448c8635fb6c" == Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("5cfa2bf023f22ac82b00cd883ea96852677ff2ecd777f656146bd22004eb75f2" == Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"
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
	REQUIRE("02779466cdec163811d078815c633f21901413081449002f24aa3e80f0b88ef7"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());

	REQUIRE(0xe3b0c44298fc1c14 == std::hash<Hash> {}(Hash().finalize()));
}
