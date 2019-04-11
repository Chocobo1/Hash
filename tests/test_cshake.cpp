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

#include "../src/cshake.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


#define ARRAY_LENGTH(a) (static_cast<gsl::index>(std::extent<decltype(a)>::value))


TEST_CASE("cshake-256")
{
	using Hash = Chocobo1::CSHAKE_256;

	// official test suite from NIST
	const uint8_t s1[4] = {0, 1, 2, 3};
	REQUIRE("d008828e2b80ac9d2218ffee1d070c48b8e4c87bff32c9699d5b6896eee0edd164020e2be0560858d9c00c037e34a96937c561a74c412bb4c746469527281c8c"
			== Hash((512 / 8), "", "Email Signature").addData(s1, ARRAY_LENGTH(s1)).finalize().toString());

	const uint8_t s2[200] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
		0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
		0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
		0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
		0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
		0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
		0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
		0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
		0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7
	};
	REQUIRE("07dc27b11e51fbac75bc7b3c1d983e8b4b85fb1defaf218912ac86430273091727f42b17ed1df63e8ec118f04b23633c1dfb1574c8fb55cb45da8e25afb092bb"
			== Hash((512 / 8), "", "Email Signature").addData(s2, ARRAY_LENGTH(s2)).finalize().toString());


	// my own tests
	using Hash2 = Chocobo1::CSHAKE_256;

	REQUIRE("46b9dd2b0ba88d13233b3feb743eeb243fcd52ea62b81b82b50c27646ed5762fd75dc4ddd8c0f200cb05019d67b592f6fc821c49479ab48640292eacb3b7c4be141e96616fb13957692cc7edd0b45ae3dc07223c8e92937bef84bc0eab862853349ec75546f58fb7c2775c38462c5010d846c185c15111e595522a6bcd16cf86f3d122109e3b1fdd943b6aec468a2d621a7c06c6a957c62b54dafc3be87567d677231395f6147293b68ceab7a9e0c58d864e8efde4e1b9a46cbe854713672f5caaae314ed9083dab4b099f8e300f01b8650f1f4b1d8fcf3f3cb53fb8e9eb2ea203bdc970f50ae55428a91f7f53ac266b28419c3778a15fd248d339ede785fb7f5a1aaa96d313eacc890936c173cdcd0fab882c45755feb3aed96d477ff96390bf9a66d1368b208e21f7c10d04a3dbd4e360633e5db4b602601c14cea737db3dcf722632cc77851cbdde2aaf0a33a07b373445df490cc8fc1e4160ff118378f11f0477de055a81a9eda57a4a2cfb0c83929d310912f729ec6cfa36c6ac6a75837143045d791cc85eff5b21932f23861bcf23a52b5da67eaf7baae0f5fb1369db78f3ac45f8c4ac5671d85735cdddb09d2b1e34a1fc066ff4a162cb263d6541274ae2fcc865f618abe27c124cd8b074ccd516301b91875824d09958f341ef274bdab0bae316339894304e35877b0c28a9b1fd166c796b9cc258a064a8f57e27f2a"
			== Hash2(512).finalize().toString());

	const char s3[] = "a";
	Hash2 test3(512);
	for (long int i = 0 ; i < 72; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("59b187c01a1704d2942526f0ab59866dbf0d81736d37960732c911c9e99cf001315b076e491ad6da9ded9e7388b9675c4dc012a86855622894950849e1698aa789cbc3eb5cfd139bc1bec25271d23cbc770e7395ffd19c06bab2d11ff7b87db0e7feed588af8f4ee59505c749ab4408fcd5568794ad8fc916a3b46ef0ad8b8a6ade24f82a4555abeedeed33862a024efd6605337a40f27a43b5283ef1221c5b77989cc4fc8834657483dd2d89449cf4da6d05b3b48b16c3d6741494443a0fc8a29e6aa59d5fae8c443f9865fe5a0c7b522d891b907af1f53125b817adf40dc10c3062e70692ddd5648828e8b0cb587541cd82a705240af111caca2e0786269e6dd65341653fd3c229804c5ee26af7b9622101b5e7a175999599850b7cba15981466d86c081f8f777e98e4c14d6f38868d0c5d1ba307ff2fe8d56e6fedcc93eae43157146388769a53595e026326bb30d55aa8919e80621577df219f7b93fb3cdb30785f77cf3582ea91bfdd395c8e1c4de25106613a4f7c195ee351fc300eb46e525d9735347fe82fbf98e7c6f77dde6bfd891ca6eabb17dee495aa4e86544352768041e1e4de05658ceda39b0c0491f8bb03f7f14e344618002b9fe9e564eb7bf54cb0b99ed40a8c856d82f9b934f6c7b771364ad132f6152898396944b189c37a4b211058b5b25a05c2623b399d5048541a477f3a08764b1cc46dfdecbf0d7"
			== test3.finalize().toString());

	const std::vector<char> s15(73, 'a');
	REQUIRE("59b187c01a1704d2942526f0ab59866dbf0d81736d37960732c911c9e99cf001315b076e491ad6da9ded9e7388b9675c4dc012a86855622894950849e1698aa789cbc3eb5cfd139bc1bec25271d23cbc770e7395ffd19c06bab2d11ff7b87db0e7feed588af8f4ee59505c749ab4408fcd5568794ad8fc916a3b46ef0ad8b8a6ade24f82a4555abeedeed33862a024efd6605337a40f27a43b5283ef1221c5b77989cc4fc8834657483dd2d89449cf4da6d05b3b48b16c3d6741494443a0fc8a29e6aa59d5fae8c443f9865fe5a0c7b522d891b907af1f53125b817adf40dc10c3062e70692ddd5648828e8b0cb587541cd82a705240af111caca2e0786269e6dd65341653fd3c229804c5ee26af7b9622101b5e7a175999599850b7cba15981466d86c081f8f777e98e4c14d6f38868d0c5d1ba307ff2fe8d56e6fedcc93eae43157146388769a53595e026326bb30d55aa8919e80621577df219f7b93fb3cdb30785f77cf3582ea91bfdd395c8e1c4de25106613a4f7c195ee351fc300eb46e525d9735347fe82fbf98e7c6f77dde6bfd891ca6eabb17dee495aa4e86544352768041e1e4de05658ceda39b0c0491f8bb03f7f14e344618002b9fe9e564eb7bf54cb0b99ed40a8c856d82f9b934f6c7b771364ad132f6152898396944b189c37a4b211058b5b25a05c2623b399d5048541a477f3a08764b1cc46dfdecbf0d7"
			== Hash(512).addData(s15.data() + 1, s15.size() - 1).finalize().toString());

	const int s16[2] = {0};
	const char s16_2[8] = {0};
	REQUIRE(Hash(512).addData(gsl::span<const int>(s16)).finalize().toString()
			== Hash(512).addData(s16_2).finalize().toString());

	const unsigned char s17[] = {0x00, 0x0A};
	const auto s17_1 = Hash(512).addData(s17, 2).finalize().toVector();
	const auto s17_2 = Hash(512).addData(s17).finalize().toVector();
	REQUIRE(s17_1 == s17_2);
}


TEST_CASE("cshake-128")
{
	using Hash = Chocobo1::CSHAKE_128;

	// official test suite from NIST
	const uint8_t s1[4] = {0, 1, 2, 3};
	REQUIRE("c1c36925b6409a04f1b504fcbca9d82b4017277cb5ed2b2065fc1d3814d5aaf5"
			== Hash((256 / 8), "", "Email Signature").addData(s1, ARRAY_LENGTH(s1)).finalize().toString());

	const uint8_t s2[200] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
		0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
		0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
		0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
		0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
		0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
		0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
		0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
		0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7
	};
	REQUIRE("c5221d50e4f822d96a2e8881a961420f294b7b24fe3d2094baed2c6524cc166b"
			== Hash((256 / 8), "", "Email Signature").addData(s2, ARRAY_LENGTH(s2)).finalize().toString());


	// my own tests
	using Hash2 = Chocobo1::CSHAKE_128;

	REQUIRE("7f9c2ba4e88f827d616045507605853ed73b8093f6efbc88eb1a6eacfa66ef263cb1eea988004b93103cfb0aeefd2a686e01fa4a58e8a3639ca8a1e3f9ae57e235b8cc873c23dc62b8d260169afa2f75ab916a58d974918835d25e6a435085b2badfd6dfaac359a5efbb7bcc4b59d538df9a04302e10c8bc1cbf1a0b3a5120ea17cda7cfad765f5623474d368ccca8af0007cd9f5e4c849f167a580b14aabdefaee7eef47cb0fca9767be1fda69419dfb927e9df07348b196691abaeb580b32def58538b8d23f87732ea63b02b4fa0f4873360e2841928cd60dd4cee8cc0d4c922a96188d032675c8ac850933c7aff1533b94c834adbb69c6115bad4692d8619f90b0cdf8a7b9c264029ac185b70b83f2801f2f4b3f70c593ea3aeeb613a7f1b1de33fd75081f592305f2e4526edc09631b10958f464d889f31ba010250fda7f1368ec2967fc84ef2ae9aff268e0b1700affc6820b523a3d917135f2dff2ee06bfe72b3124721d4a26c04e53a75e30e73a7a9c4a95d91c55d495e9f51dd0b5e9d83c6d5e8ce803aa62b8d654db53d09b8dcff273cdfeb573fad8bcd45578bec2e770d01efde86e721a3f7c6cce275dabe6e2143f1af18da7efddc4c7b70b5e345db93cc936bea323491ccb38a388f546a9ff00dd4e1300b9b2153d2041d205b443e41b45a653f2a5c4492c1add544512dda2529833462b71a41a45be97290b6f"
			== Hash2(512).finalize().toString());

	const char s3[] = "a";
	Hash2 test3(512);
	for (long int i = 0 ; i < 72; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("7bfe6348bd840ae789325524a6865557bf4b3d30f1f895f8d5210e79ed82e2c59130e38b78e3db0bd46ca2babc7d688c8340624f38477df62982591f0dc929582bc153c75aa06df7f182cf278b63f7667d8b967281b03fd1cf6ad4c6261b3c33f120dbfe5768680eb5e47fea8bd41026bf004bf7453b4739a73d8b984b7e4a2f186e3149d065752c32f075f4907ea12d3cc1c6fff3c78d53adca00825e400c7be807cecf2141ac80dd3afb2d4011cb0e04dbd3e568f8fb6242b6b232ea174da9be7d2f85952d49c7c9d07befb003988a7d7db0abec1dbb5876bb51e782feb68e22feb0638fd8b60996f90a737bee649728eb4df376a846b550992f357c17018c1ca10a4224b77d72530bdea3b85e2664d21b6e5a7e1076ba508e95e2f8a9fce147aaa970e983cf2e98123d5c08e068fdcd2c46c13875a060eb71a2ecf622f68fe9d8aacc01d139674b8a89e727a9c8b1226391e41ec248d571d10035ffe1c66cc62ed28bcd242782e6ec4c7f790b863b6585aa25676307fa1d66cb2ba313edc1b55ccbfbbba0be6a6ead635f12bc93cf9a932f459b7d9a90cf96729590cb4ebd381cf03ee855762f536fd684854a28d68f5344d8a6265bff31e75f23faf254470574fd97528c58c50ac0be48ca7d6b22ada85b66b3d209c40070978ba0ec152e3042b703e47e3a93822f6a167fe42d6e7acba9e573f5e84cb12feda9399e4bf2"
			== test3.finalize().toString());

	const std::vector<char> s15(73, 'a');
	REQUIRE("7bfe6348bd840ae789325524a6865557bf4b3d30f1f895f8d5210e79ed82e2c59130e38b78e3db0bd46ca2babc7d688c8340624f38477df62982591f0dc929582bc153c75aa06df7f182cf278b63f7667d8b967281b03fd1cf6ad4c6261b3c33f120dbfe5768680eb5e47fea8bd41026bf004bf7453b4739a73d8b984b7e4a2f186e3149d065752c32f075f4907ea12d3cc1c6fff3c78d53adca00825e400c7be807cecf2141ac80dd3afb2d4011cb0e04dbd3e568f8fb6242b6b232ea174da9be7d2f85952d49c7c9d07befb003988a7d7db0abec1dbb5876bb51e782feb68e22feb0638fd8b60996f90a737bee649728eb4df376a846b550992f357c17018c1ca10a4224b77d72530bdea3b85e2664d21b6e5a7e1076ba508e95e2f8a9fce147aaa970e983cf2e98123d5c08e068fdcd2c46c13875a060eb71a2ecf622f68fe9d8aacc01d139674b8a89e727a9c8b1226391e41ec248d571d10035ffe1c66cc62ed28bcd242782e6ec4c7f790b863b6585aa25676307fa1d66cb2ba313edc1b55ccbfbbba0be6a6ead635f12bc93cf9a932f459b7d9a90cf96729590cb4ebd381cf03ee855762f536fd684854a28d68f5344d8a6265bff31e75f23faf254470574fd97528c58c50ac0be48ca7d6b22ada85b66b3d209c40070978ba0ec152e3042b703e47e3a93822f6a167fe42d6e7acba9e573f5e84cb12feda9399e4bf2"
			== Hash(512).addData(s15.data() + 1, s15.size() - 1).finalize().toString());

	const int s16[2] = {0};
	const char s16_2[8] = {0};
	REQUIRE(Hash(512).addData(gsl::span<const int>(s16)).finalize().toString()
			== Hash(512).addData(s16_2).finalize().toString());

	const unsigned char s17[] = {0x00, 0x0A};
	const auto s17_1 = Hash(512).addData(s17, 2).finalize().toVector();
	const auto s17_2 = Hash(512).addData(s17).finalize().toVector();
	REQUIRE(s17_1 == s17_2);
}
