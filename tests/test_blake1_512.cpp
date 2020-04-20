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

#include "../src/blake1_512.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("blake1-512")
{
	using Hash = Chocobo1::Blake1_512;

	// official test suite from blake1 SHA3-proposal
	const char s1[1] = {0};
	REQUIRE("97961587f6d970faba6d2478045de6d1fabd09b61ae50932054d52bc29d31be4ff9102b9f69e2bbdb83be13d4b9c06091e5fa0b48bd081b634058be0ec49beb3"
			== Hash().addData(s1, sizeof(s1)).finalize().toString());

	const char s2[144] = {0};
	REQUIRE("313717d608e9cf758dcb1eb0f0c3cf9fc150b2d500fb33f51c52afc99d358a2f1374b8a38bba7974e7f6ef79cab16f22ce1e649d6e01ad9589c213045d545dde"
			== Hash().addData(s2, sizeof(s2)).finalize().toString());


	// my own tests
	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("e5d3ecf60cbf8ed8b44307ae6606f4c8eae0a602c2a393f529cf405a1384c791c3ce58f54838a76b89a6455e988d63f98222ea82bf26e11e970516fb7c62b41d"
			== test3.finalize().toString());

	REQUIRE("a8cfbbd73726062df0c6864dda65defe58ef0cc52a5625090fa17601e1eecd1b628e94f396ae402a00acc9eab77b4d4c2e852aaaa25a636d80af3fc7913ef5b8"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("1f7e26f63b6ad25a0896fd978fd050a1766391d2fd0471a77afb975e5034b7ad2d9ccf8dfb47abbbe656e1b82fbc634ba42ce186e8dc5e1ce09a885d41f43451"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("9adee69086e3ec1f7e8bba754ac96135bb9abe283413a90a965d0cc5f6fb78a4b1ae49b3c5534d0bed63b20731e551e8e5d3119f65fd2f009405953aa93dec67"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("0040d5f28a7a2c11c38d2f5628311141e38270f065b9e2968c3af459cedee8e0995aaf96afc8d12d58094ded014214c105c6ed79de7c469f3ad38c568ae6cc91"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(128, 'a');  // length == BLOCK_SIZE
	REQUIRE("e9c1faf06ba46df441c96d4a1eaa97342c6c8e84a32405ef4920f697213f15d8ae9fe8446fa0574108704a853511523bfcc58bf67aafaa6101e2f70e393d542e"
			== Hash().addData(s14.data(), s14.size()).finalize().toString());

	const std::vector<char> s15(129, 'a');
	REQUIRE("e9c1faf06ba46df441c96d4a1eaa97342c6c8e84a32405ef4920f697213f15d8ae9fe8446fa0574108704a853511523bfcc58bf67aafaa6101e2f70e393d542e"
			== Hash().addData(s15.data() + 1, s15.size() - 1).finalize().toString());

	const int s16[2] = {0};
	const char s16_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s16)).finalize().toString()
			== Hash().addData(s16_2).finalize().toString());

	const unsigned char s17[] = {0x00, 0x0A};
	const auto s17_1 = Hash().addData(s17, 2).finalize().toArray();
	const auto s17_2 = Hash().addData(s17).finalize().toArray();
	REQUIRE(s17_1 == s17_2);
}
