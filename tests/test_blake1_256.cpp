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

#include "../src/blake1_256.h"

#include "catch/single_include/catch.hpp"

#include <cstring>


TEST_CASE("blake1-256")
{
	using Hash = Chocobo1::Blake1_256;

	// official test suite from blake1 SHA3-proposal
	const char s1[1] = {0};
	REQUIRE("0ce8d4ef4dd7cd8d62dfded9d4edb0a774ae6a41929a74da23109e8f11139c87"
			== Hash().addData(s1, sizeof(s1)).finalize().toString());

	const char s2[72] = {0};
	REQUIRE("d419bad32d504fb7d44d460c42c5593fe544fa4c135dec31e21bd9abdcc22d41"
			== Hash().addData(s2, sizeof(s2)).finalize().toString());


	// my own tests
	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 1000000; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("22be6de4aa4214c9403f10598f0a6b0e834570251a13bc27589437f7139a5d44"
			== test3.finalize().toString());

	REQUIRE("716f6e863f744b9ac22c97ec7b76ea5f5908bc5b2f67c61510bfc4751384ea7a"
			== Hash().finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("7576698ee9cad30173080678e5965916adbb11cb5245d386bf1ffda1cb26c9d7"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("13af722eafeab6bb2ed498129044e6782c84a7604bba9988b135d98158fbe816"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("afeb40bca1a092fda935ed49bbae0074eb9e5d2e9ef00ecb89839e643b018d56"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const char s14[] = "a";
	Hash test14;
	for (long int i = 0 ; i < 64; ++i)
		test14.addData(s14, strlen(s14));
	REQUIRE("84d7f3bbf2cfc3ee940ddb6d25045c6d3f756c4b2077a8128e171d5d165be170"
			== test14.finalize().toString());

	const std::vector<char> s15(65, 'a');
	REQUIRE("84d7f3bbf2cfc3ee940ddb6d25045c6d3f756c4b2077a8128e171d5d165be170"
			== Hash().addData(s15.data() + 1, s15.size() - 1).finalize().toString());
}
