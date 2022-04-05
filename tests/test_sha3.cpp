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

#include "../src/sha3.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("sha3-512")
{
	using Hash = Chocobo1::SHA3_512;

	// official test suite from NIST
	const char s1[] = "";
	REQUIRE("a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26"
			== Hash().addData(s1, strlen(s1)).finalize().toString());

	const unsigned char s2 = 0xa3;
	Hash test2;
	for (long int i = 0 ; i < 200; ++i)
		test2.addData(&s2, 1);
	REQUIRE("e76dfad22084a8b1467fcf2ffa58361bec7628edf5f3fdc0e4805dc48caeeca81b7c13c30adf52a3659584739a2df46be589c51ca1a4a8416df6545a1ce8ba00"
			== test2.finalize().toString());


	// my own tests
	REQUIRE(Hash() == Hash());
	REQUIRE(Hash().addData("123").finalize() != Hash().finalize());

	REQUIRE("a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26"
			== Hash().finalize().toString());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 72; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("a8ae722a78e10cbbc413886c02eb5b369a03f6560084aff566bd597bb7ad8c1ccd86e81296852359bf2faddb5153c0a7445722987875e74287adac21adebe952"
			== test3.finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("01dedd5de4ef14642445ba5f5b97c15e47b9ad931326e4b0727cd94cefc44fff23f07bf543139939b49128caf436dc1bdee54fcb24023a08d9403f9b4bf0d450"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("18f4f4bd419603f95538837003d9d254c26c23765565162247483f65c50303597bc9ce4d289f21d1c2f1f458828e33dc442100331b35e7eb031b5d38ba6460f8"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("3cda341b539bd2e963e0221fc022b9411ee15dbb2fb64a96a1a75d233c1f34566ddf4a8879f490ea6f59b538155fa38c803f8e69d9ef4fd1ecd9e52744690aa3"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(73, 'a');
	REQUIRE("a8ae722a78e10cbbc413886c02eb5b369a03f6560084aff566bd597bb7ad8c1ccd86e81296852359bf2faddb5153c0a7445722987875e74287adac21adebe952"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toVector();
	const auto s16_2 = Hash().addData(s16).finalize().toVector();
	REQUIRE(s16_1 == s16_2);

	const char s17[71] = {0};
	REQUIRE("cd87417194c917561a59c7f2eb4b95145971e32e8e4ef3b23b0f190bfd29e3692cc7975275750a27df95d5c6a99b7a341e1b8a38a750a51aca5b77bae41fbbfc"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());

	REQUIRE(0xa69f73cca23a9ac5 == std::hash<Hash> {}(Hash().finalize()));

	Hash h18;
	Hash h18_2;
	h18 = h18_2;
	REQUIRE(h18.finalize().toVector() == h18_2.finalize().toVector());
	h18 = std::move(h18_2);
	REQUIRE(h18.finalize().toVector() == Hash().finalize().toVector());

	Hash h19;
	Hash::BaseType h19_2(512 / 8);
	h19 = h19_2;
	REQUIRE(h19.finalize().toVector() == h19_2.finalize().toVector());
	h19 = std::move(h19_2);
	REQUIRE(h19.finalize().toVector() == Hash::BaseType(512 / 8).finalize().toVector());
}


TEST_CASE("sha3-384")
{
	using Hash = Chocobo1::SHA3_384;

	// official test suite from NIST
	const char s1[] = "";
	REQUIRE("0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2ac3713831264adb47fb6bd1e058d5f004"
			== Hash().addData(s1, strlen(s1)).finalize().toString());

	const unsigned char s2 = 0xa3;
	Hash test2;
	for (long int i = 0 ; i < 200; ++i)
		test2.addData(&s2, 1);
	REQUIRE("1881de2ca7e41ef95dc4732b8f5f002b189cc1e42b74168ed1732649ce1dbcdd76197a31fd55ee989f2d7050dd473e8f"
			== test2.finalize().toString());


	// my own tests
	REQUIRE(Hash() == Hash());
	REQUIRE(Hash().addData("123").finalize() != Hash().finalize());

	REQUIRE("0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2ac3713831264adb47fb6bd1e058d5f004"
			== Hash().finalize().toString());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 72; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("36ed8cf277398426c25d4c8cdbf29dd22db8c1298ce34882a7bcc633d65b8db249c2bd6c64e5a4f2edc5a5fc5adc8140"
			== test3.finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("7063465e08a93bce31cd89d2e3ca8f602498696e253592ed26f07bf7e703cf328581e1471a7ba7ab119b1a9ebdf8be41"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("1a34d81695b622df178bc74df7124fe12fac0f64ba5250b78b99c1273d4b080168e10652894ecad5f1f4d5b965437fb9"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("bce1cba82694f64110464b677685ba62a3e410ea08e2b15e0d8b54e60fe1449574a41a97a4fee9daa158a51db4931af6"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(73, 'a');
	REQUIRE("36ed8cf277398426c25d4c8cdbf29dd22db8c1298ce34882a7bcc633d65b8db249c2bd6c64e5a4f2edc5a5fc5adc8140"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toVector();
	const auto s16_2 = Hash().addData(s16).finalize().toVector();
	REQUIRE(s16_1 == s16_2);

	const char s17[103] = {0};
	REQUIRE("11c556552dda63418669716bad02e4125f4973f3ceea99ee50b6ff117e9f7a3fed0360abb5eff4ac8e954205c01981d2"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());

	REQUIRE(0xc63a75b845e4f7d == std::hash<Hash> {}(Hash().finalize()));

	Hash h18;
	Hash h18_2;
	h18 = h18_2;
	REQUIRE(h18.finalize().toVector() == h18_2.finalize().toVector());
	h18 = std::move(h18_2);
	REQUIRE(h18.finalize().toVector() == Hash().finalize().toVector());

	Hash h19;
	Hash::BaseType h19_2(384 / 8);
	h19 = h19_2;
	REQUIRE(h19.finalize().toVector() == h19_2.finalize().toVector());
	h19 = std::move(h19_2);
	REQUIRE(h19.finalize().toVector() == Hash::BaseType(384 / 8).finalize().toVector());
}


TEST_CASE("sha3-256")
{
	using Hash = Chocobo1::SHA3_256;

	// official test suite from NIST
	const char s1[] = "";
	REQUIRE("a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a"
			== Hash().addData(s1, strlen(s1)).finalize().toString());

	const unsigned char s2 = 0xa3;
	Hash test2;
	for (long int i = 0 ; i < 200; ++i)
		test2.addData(&s2, 1);
	REQUIRE("79f38adec5c20307a98ef76e8324afbfd46cfd81b22e3973c65fa1bd9de31787"
			== test2.finalize().toString());


	// my own tests
	REQUIRE(Hash() == Hash());
	REQUIRE(Hash().addData("123").finalize() != Hash().finalize());

	REQUIRE("a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a"
			== Hash().finalize().toString());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 72; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("faf7e2ca748a48eff17f1f0c6b495ab3f2c3dd34c8d335aee79ceff5fe780a01"
			== test3.finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("69070dda01975c8c120c3aada1b282394e7f032fa9cf32f4cb2259a0897dfc04"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("a80f839cd4f83f6c3dafc87feae470045e4eb0d366397d5c6ce34ba1739f734d"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("8d1054aa828ad5c050aab35516aff6285a08b1cbdc1ccc312bd4b4d2ac423ad6"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(73, 'a');
	REQUIRE("faf7e2ca748a48eff17f1f0c6b495ab3f2c3dd34c8d335aee79ceff5fe780a01"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toVector();
	const auto s16_2 = Hash().addData(s16).finalize().toVector();
	REQUIRE(s16_1 == s16_2);

	const char s17[135] = {0};
	REQUIRE("7d080d7ba978a75c8a7d1f9be566c859084509c9c2b4928435c225d5777d98e3"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());

	REQUIRE(0xa7ffc6f8bf1ed766 == std::hash<Hash> {}(Hash().finalize()));

	Hash h18;
	Hash h18_2;
	h18 = h18_2;
	REQUIRE(h18.finalize().toVector() == h18_2.finalize().toVector());
	h18 = std::move(h18_2);
	REQUIRE(h18.finalize().toVector() == Hash().finalize().toVector());

	Hash h19;
	Hash::BaseType h19_2(256 / 8);
	h19 = h19_2;
	REQUIRE(h19.finalize().toVector() == h19_2.finalize().toVector());
	h19 = std::move(h19_2);
	REQUIRE(h19.finalize().toVector() == Hash::BaseType(256 / 8).finalize().toVector());
}


TEST_CASE("sha3-224")
{
	using Hash = Chocobo1::SHA3_224;

	// official test suite from NIST
	const char s1[] = "";
	REQUIRE("6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7"
			== Hash().addData(s1, strlen(s1)).finalize().toString());

	const unsigned char s2 = 0xa3;
	Hash test2;
	for (long int i = 0 ; i < 200; ++i)
		test2.addData(&s2, 1);
	REQUIRE("9376816aba503f72f96ce7eb65ac095deee3be4bf9bbc2a1cb7e11e0"
			== test2.finalize().toString());


	// my own tests
	REQUIRE(Hash() == Hash());
	REQUIRE(Hash().addData("123").finalize() != Hash().finalize());

	REQUIRE("6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7"
			== Hash().finalize().toString());

	const char s3[] = "a";
	Hash test3;
	for (long int i = 0 ; i < 72; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("0b98a46d4e243d8c5bfcdd957629fc435c671b15c0d195e39de8efd1"
			== test3.finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("d15dadceaa4d5d7bb3b48f446421d542e08ad8887305e28d58335795"
			== Hash().addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("2d0708903833afabdd232a20201176e8b58c5be8a6fe74265ac54db0"
			== Hash().addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("25f24a991d04586a87330f45e52fc8bdc22ec0d33ab95b2d8e05987c"
			== Hash().addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(73, 'a');
	REQUIRE("0b98a46d4e243d8c5bfcdd957629fc435c671b15c0d195e39de8efd1"
			== Hash().addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash().addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash().addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash().addData(s16, 2).finalize().toVector();
	const auto s16_2 = Hash().addData(s16).finalize().toVector();
	REQUIRE(s16_1 == s16_2);

	const char s17[143] = {0};
	REQUIRE("b6b709fdb9852b8c7439a33595d42dba2940f44c10c3ce09f8b6a87a"
			== Hash().addData(s17, sizeof(s17)).finalize().toString());

	REQUIRE(0x6b4e03423667dbb7 == std::hash<Hash> {}(Hash().finalize()));

	Hash h18;
	Hash h18_2;
	h18 = h18_2;
	REQUIRE(h18.finalize().toVector() == h18_2.finalize().toVector());
	h18 = std::move(h18_2);
	REQUIRE(h18.finalize().toVector() == Hash().finalize().toVector());

	Hash h19;
	Hash::BaseType h19_2(224 / 8);
	h19 = h19_2;
	REQUIRE(h19.finalize().toVector() == h19_2.finalize().toVector());
	h19 = std::move(h19_2);
	REQUIRE(h19.finalize().toVector() == Hash::BaseType(224 / 8).finalize().toVector());
}
