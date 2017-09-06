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

#include "../src/blake2.h"

#include "catch/single_include/catch.hpp"

#include <cstring>


TEST_CASE("blake2")
{
	using Hash = Chocobo1::Blake2;

	// official test suite in rfc
	const char s1[] = "abc";
	REQUIRE("ba80a53f981c4d0d6a2797b69f12f6e94c212f14685ac4b74b12bb6fdbffa2d17d87c5392aab792dc252d5de4533cc9518d38aa8dbf1925ab92386edd4009923"
			== Hash().addData(s1, strlen(s1)).finalize().toString());


	// my own tests
	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("98fb3efb7206fd19ebf69b6f312cf7b64e3b94dbe1a17107913975a793f177e1d077609d7fba363cbba00d05f7aa4e4fa8715d6428104c0a75643b0ff3fd3eaf"
			== test3.finalize().toString());

	REQUIRE("786a02f742015903c6c6fd852552d272912f4740e15847618a86e217f71f5419d25e1031afee585313896444934eb04b903a685b1448b755d56f701afe9be2ce"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("a8add4bdddfd93e4877d2746e62817b116364a1fa7bc148d95090bc7333b3673f82401cf7aa2e4cb1ecd90296e3f14cb5413f8ed77be73045b13914cdcd6a918"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("87af9dc4afe5651b7aa89124b905fd214bf17c79af58610db86a0fb1e0194622a4e9d8e395b352223a8183b0d421c0994b98286cbf8c68a495902e0fe6e2bda2"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("15266a69c93c7b0db22fbb8b1f0846cfcf5f4f0a2d4bfe57a75a82aff2c22f0f688b16109e110279981b5fbaa02ad82ea9990423b13aa7124411ad8eae9be21a"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(128, 'a');  // length == BLOCK_SIZE
	REQUIRE("fc6c71f688f43ea7d60817478808f3cac753e61571865c95adbc2d9122c943a76b92c2cb1047ef3fe7bf6e436ec1d0a99a9e5b216780bf7fed9d7ca91d3a8f3b"
			== Hash().addData(s14.data(), s14.size()).finalize().toString());

	const std::vector<char> s15(129, 'a');
	REQUIRE("fc6c71f688f43ea7d60817478808f3cac753e61571865c95adbc2d9122c943a76b92c2cb1047ef3fe7bf6e436ec1d0a99a9e5b216780bf7fed9d7ca91d3a8f3b"
			== Hash().addData(s15.data() + 1, s15.size() - 1).finalize().toString());
}
