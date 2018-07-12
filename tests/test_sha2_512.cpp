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

#include "../src/sha2_512.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


#define ARRAY_LENGTH(a) (std::extent<decltype(a)>::value)


TEST_CASE("sha2-512")
{
	using Hash = Chocobo1::SHA2_512;

	// official test suite in rfc
	const char s1[] = "abc";
	REQUIRE("ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f"
			== Hash().addData(s1, strlen(s1)).finalize().toString());

	const char s2[] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
	REQUIRE("8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909"
			== Hash().addData(s2, strlen(s2)).finalize().toString());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b"
			== test3.finalize().toString());

	const char s4[] = "0123456701234567012345670123456701234567012345670123456701234567";
	Hash test4;
	for (int i = 0 ; i < 10; ++i)
		test4.addData(s4, strlen(s4));
	REQUIRE("89d05ba632c699c31231ded4ffc127d5a894dad412c0e024db872d1abd2ba8141a0f85072a9be1e2aa04cf33c765cb510813a39cd5a84c4acaa64d3f3fb7bae9"
			== test4.finalize().toString());

	const char s6[] = "\xD0";
	REQUIRE("9992202938e882e73e20f6b69e68a0a7149090423d93c81bab3f21678d4aceeee50e4e8cafada4c85a54ea8306826c4ad6e74cece9631bfa8a549b4ab3fbba15"
			== Hash().addData(s6, strlen(s6)).finalize().toString());

	const char s8[] = "\x8d\x4e\x3c\x0e\x38\x89\x19\x14\x91\x81\x6e\x9d\x98\xbf\xf0\xa0";
	REQUIRE("cb0b67a4b8712cd73c9aabc0b199e9269b20844afb75acbdd1c153c9828924c3ddedaafe669c5fdd0bc66f630f6773988213eb1b16f517ad0de4b2f0c95c90f8"
			== Hash().addData(s8, strlen(s8)).finalize().toString());

	const char s10[] = "\xa5\x5f\x20\xc4\x11\xaa\xd1\x32\x80\x7a\x50\x2d\x65\x82\x4e\x31" \
					   "\xa2\x30\x54\x32\xaa\x3d\x06\xd3\xe2\x82\xa8\xd8\x4e\x0d\xe1\xde" \
					   "\x69\x74\xbf\x49\x54\x69\xfc\x7f\x33\x8f\x80\x54\xd5\x8c\x26\xc4" \
					   "\x93\x60\xc3\xe8\x7a\xf5\x65\x23\xac\xf6\xd8\x9d\x03\xe5\x6f\xf2" \
					   "\xf8\x68\x00\x2b\xc3\xe4\x31\xed\xc4\x4d\xf2\xf0\x22\x3d\x4b\xb3" \
					   "\xb2\x43\x58\x6e\x1a\x7d\x92\x49\x36\x69\x4f\xcb\xba\xf8\x8d\x95" \
					   "\x19\xe4\xeb\x50\xa6\x44\xf8\xe4\xf9\x5e\xb0\xea\x95\xbc\x44\x65" \
					   "\xc8\x82\x1a\xac\xd2\xfe\x15\xab\x49\x81\x16\x4b\xbb\x6d\xc3\x2f" \
					   "\x96\x90\x87\xa1\x45\xb0\xd9\xcc\x9c\x67\xc2\x2b\x76\x32\x99\x41" \
					   "\x9c\xc4\x12\x8b\xe9\xa0\x77\xb3\xac\xe6\x34\x06\x4e\x6d\x99\x28" \
					   "\x35\x13\xdc\x06\xe7\x51\x5d\x0d\x73\x13\x2e\x9a\x0d\xc6\xd3\xb1" \
					   "\xf8\xb2\x46\xf1\xa9\x8a\x3f\xc7\x29\x41\xb1\xe3\xbb\x20\x98\xe8" \
					   "\xbf\x16\xf2\x68\xd6\x4f\x0b\x0f\x47\x07\xfe\x1e\xa1\xa1\x79\x1b" \
					   "\xa2\xf3\xc0\xc7\x58\xe5\xf5\x51\x86\x3a\x96\xc9\x49\xad\x47\xd7" \
					   "\xfb\x40\xd2";
	REQUIRE("c665befb36da189d78822d10528cbf3b12b3eef726039909c1a16a270d48719377966b957a878e720584779a62825c18da26415e49a7176a894e7510fd1451f5"
			== Hash().addData(s10, (ARRAY_LENGTH(s10) - 1)).finalize().toString());


	// my own tests
	REQUIRE("cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("91ea1245f20d46ae9a037a989f54f1f790f0a47607eeb8a14d12890cea77a1bbc6c7ed9cf205e67b7f2b8fd4c7dfd3a7a8617e45f3c463d481c7e586c39ac1ed"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("8bbc0670dc3e29c7341035e6110968c878dca505248f09b3380899ed9b3a1aec19282f1d2de75d6c6acc1d3e0b63be33c0c5a731ac00f7d29c02e31c2846cfde"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(1000001, 'a');
	REQUIRE("e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(gsl::span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());
}
