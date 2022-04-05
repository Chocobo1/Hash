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

#include "../src/tuple_hash.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


#define ARRAY_LENGTH(a) (static_cast<int>(std::extent<decltype(a)>::value))


TEST_CASE("tuple_hash-256")
{
	using Hash = Chocobo1::TupleHash_256;

	// official test suite from NIST
	const uint8_t s1_1[3] = {0, 1, 2};
	const uint8_t s1_2[6] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	REQUIRE("cfb7058caca5e668f81a12a20a2195ce97a925f1dba3e7449a56f82201ec607311ac2696b1ab5ea2352df1423bde7bd4bb78c9aed1a853c78672f9eb23bbe194"
			== Hash(64).nextData(s1_1, ARRAY_LENGTH(s1_1)).nextData(s1_2, ARRAY_LENGTH(s1_2)).finalize().toString());

	const uint8_t s2_1[3] = {0, 1, 2};
	const uint8_t s2_2[6] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	REQUIRE("147c2191d5ed7efd98dbd96d7ab5a11692576f5fe2a5065f3e33de6bba9f3aa1c4e9a068a289c61c95aab30aee1e410b0b607de3620e24a4e3bf9852a1d4367e"
			== Hash(64, "My Tuple App").nextData(s2_1, ARRAY_LENGTH(s2_1)).nextData(s2_2, ARRAY_LENGTH(s2_2)).finalize().toString());

	const uint8_t s3_1[3] = {0, 1, 2};
	const uint8_t s3_2[6] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	const uint8_t s3_3[9] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28};
	REQUIRE("45000be63f9b6bfd89f54717670f69a9bc763591a4f05c50d68891a744bcc6e7d6d5b5e82c018da999ed35b0bb49c9678e526abd8e85c13ed254021db9e790ce"
			== Hash(64, "My Tuple App").nextData(s3_1, ARRAY_LENGTH(s3_1)).nextData(s3_2, ARRAY_LENGTH(s3_2))
				.nextData(s3_3, ARRAY_LENGTH(s3_3)).finalize().toString());


	// my own tests
	REQUIRE(Hash(512) == Hash(512));
	REQUIRE(Hash(512).nextData("123").finalize() != Hash(512).finalize());

	REQUIRE("8bec6f07e2545ac0062f726e8518c4b15cf4296161ce433fee5e6d847636804129204831e4d093430598718874cd7326ba3e0f798a24171cbdc31c52ffcbfb604d873c749b8511c832749f8262c8e48debfc26665e42cf1763956fa81c29eaa160dddf20eccfa50cd952f9c343c3bcd9caf85b1723e4dd97f43d21feabfcaf88fe5f3477d43422ceea809f34836cef8cee92094ee154855b58f0df41b8cb13c8398f289e3cb0cadde2ec4957deefa93a0f6908420cfa146c8771af9e51c3bf13463933b3929185a5b7c7a9726ef788262d51a6cdba8380d8502144aefefa1396f767f89409cee37be32ad0440d6e00dd52e113ad3359692110af077d2efc74e4234cf31495b5f12cf70348c9d5db7faa3d6d4c85eebc6647bc41998520625a13625bc5a26ba0b0c009f9bf124f5fc9c6dec632be32d879c7e69c3985e28567fda322c9718b052db3d75ab271109651a57313fdee965e63ceef3123954169b442cbf00f4a490bc90429ddcccd1e8986f42255a6dba821bcaf794b8273a215c7c1244f3e6c3438f98b62813a6f1e764cce9ffde881ec6d9ed2bea2d46df0db96a9c02ea427b86d553aae8f4c0e0227913b6ef065a841413e4853a7c8b374eedf17179f84b9e0bd4500ee778a5967effb973db44b8d98132ab60197cae4c9d68756d0a332dfe25a33c1b35397764d5ec39194b9652e2b1e5290a8a66ce12c77ef71"
			== Hash(512).finalize().toString());

	const char s3[] = "a";
	Hash test3(512);
	for (long int i = 0 ; i < 72; ++i)
		test3.nextData(s3, strlen(s3));
	REQUIRE("35137df6d50f210bddb05c17b436dbedb225e7085ff23ebcff5871eb3453dd2abd25aedccf740f700e3a4a68e0dc5c7135792e06805ff323c5c9a938fe9d32b806efe80c406040e4b52b5d33178ffa521cf7c159751be028ba436726c7f86e06eff942e7dd9b2d6296f24e5684f82d0aec1545e6a3d028f400a967064f53fb9d5bd5237082627d628b1cb9924916b436de9ebefae3bace3dd6d2f126a0da3901039cca7e4ddda5f8407c9e95521a4f08bd2a89db98ff0fd3216a2bee5d1befe277e8741a9403a8156cd9799aecb76f84f63f3d766c0975b6351cef5604c11ed29f00ff6d84ec275d508f483cfd6f59d0e93ffc87dba3bc65b39886d6cd32db25011cf2023d2ec69a2b11113fa5c611f9e3f7e2808f7013f45e3e8c313187c15527785e03b79f18a514942cf4f4a7085debf495f947fe3f3dea3523de189e7e5147ef653eb396a1fac6c745704687c9caa6061cd6cde194eb41d0f1647e4d1e87f81ca9fb72d9ff5b1553b6daf433552c37cfaf8acd9d2a39f669a3b826aa44c89e4cdf68aa4ce4fe80bd65f1a6778ae194a92c61b0b4207fb075d14c097cf190e19fc9a2e3f823f50630b87389d77e6f33affb0c3f1a827bf5fcf09b3bce1469cc8439d28930e3074fdb84d732214cc7328d2335a4f050eedb04acf0c9527466c5811ff54f13197e0ba06d5dee8752bba98c9da784e6d54f06f99cee9f95fa6e"
			== test3.finalize().toString());

	const std::vector<char> s4(269, '\0');
	REQUIRE("b81bab65f7c5d025eb158d1049fabd2fe7d11f6eae1db15f79cc2e990de6ea29"
			== Hash(32).nextData(s4.data(), s4.size()).finalize().toString());

	const std::vector<char> s5(270, '\0');
	REQUIRE("b81bab65f7c5d025eb158d1049fabd2fe7d11f6eae1db15f79cc2e990de6ea29"
			== Hash(32).nextData(s5.data() + 1, s5.size() - 1).finalize().toString());

	const int s6[2] = {0};
	const char s6_2[8] = {0};
	REQUIRE(Hash(32).nextData(Hash::Span<const int>(s6)).finalize().toString()
			== Hash(32).nextData(s6_2).finalize().toString());

	const unsigned char s7[] = {0x00, 0x0A};
	const auto s7_1 = Hash(32).nextData(s7, 2).finalize().toVector();
	const auto s7_2 = Hash(32).nextData(s7).finalize().toVector();
	REQUIRE(s7_1 == s7_2);

	REQUIRE(0x8bec6f07e2545ac0 == std::hash<Hash> {}(Hash(512).finalize()));

	Hash h18(512);
	Hash h18_2(512);
	h18 = h18_2;
	REQUIRE(h18.finalize().toVector() == h18_2.finalize().toVector());
	h18 = std::move(h18_2);
	REQUIRE(h18.finalize().toVector() == Hash(512).finalize().toVector());

	Hash h19(512);
	Hash::BaseType h19_2(512);
	h19 = h19_2;
	REQUIRE(h19.finalize().toVector() == h19_2.finalize().toVector());
	h19 = std::move(h19_2);
	REQUIRE(h19.finalize().toVector() == Hash::BaseType(512).finalize().toVector());
}


TEST_CASE("tuple_hash-128")
{
	using Hash = Chocobo1::TupleHash_128;

	// official test suite from NIST
	const uint8_t s1_1[3] = {0, 1, 2};
	const uint8_t s1_2[6] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	REQUIRE("c5d8786c1afb9b82111ab34b65b2c0048fa64e6d48e263264ce1707d3ffc8ed1"
			== Hash(32).nextData(s1_1, ARRAY_LENGTH(s1_1)).nextData(s1_2, ARRAY_LENGTH(s1_2)).finalize().toString());

	const uint8_t s2_1[3] = {0, 1, 2};
	const uint8_t s2_2[6] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	REQUIRE("75cdb20ff4db1154e841d758e24160c54bae86eb8c13e7f5f40eb35588e96dfb"
			== Hash(32, "My Tuple App").nextData(s2_1, ARRAY_LENGTH(s2_1)).nextData(s2_2, ARRAY_LENGTH(s2_2)).finalize().toString());

	const uint8_t s3_1[3] = {0, 1, 2};
	const uint8_t s3_2[6] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	const uint8_t s3_3[9] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28};
	REQUIRE("e60f202c89a2631eda8d4c588ca5fd07f39e5151998deccf973adb3804bb6e84"
			== Hash(32, "My Tuple App").nextData(s3_1, ARRAY_LENGTH(s3_1)).nextData(s3_2, ARRAY_LENGTH(s3_2))
				.nextData(s3_3, ARRAY_LENGTH(s3_3)).finalize().toString());


	// my own tests
	REQUIRE(Hash(512) == Hash(512));
	REQUIRE(Hash(512).nextData("123").finalize() != Hash(512).finalize());

	REQUIRE("dc32e64d12751cede925e6793bc4ae6527e40b3bd2100d1c3daaf896c868911cbec37ef94139f20dc0f62ee322aec3d52e912ca42a4262b76631f8490c5327b4ae0cbd9383834dd0ed4dca9470beb7c58bcf5af3301e384a14da18ebcd2d2f5ee9e21565c291726b7af4136cb98253dc694c2ebc8948bab010532403dae38f466398390538f63f0c3908ac385eae7374253e9f0a3ba9ef2deedfd62dc6228de88ae6fcb287de025878229c378004a411fa9f13fa3d1032f3cd3a95a609f10fe3a4f2d024e22d6dd523cbe2210f5e36107d95914b29ed32ff6491137a462f899f1064606bf4383a41b50c67f7ec7f281ca68009e57aa53746b57e8f99752cc43a7f5ef2c913c31306c36ace7896986421269aff86a3ac7076b8dcfd117408284b47c6b3b26659f324cf0d0313cac92a1e2f69c2a8ea3d2ff36a5e7e7cacb2d48d990a8edde489899c700db45b08d62822fe15278831dbb431705403914bc6bc10eef0f0f2ce2da239133041530a115ee7fb54c6395728ea6622aff829e7f00a675dec82bedebbd7f7bd3daa2130880df2fb8ab70f5cb5ce1a3062bca0be9034187353bc00a98306230d7e8188feee3eda42a5c80dfbaf22d843c16f06a63ff93908f287a444c33f2005677ec492b655299f7fee4920d8bf6df947509e3b6c2bf7be75b0b3ed44dcea161b1ee043cf0d8341ef61b8b26ef17eeb062a4dc09fe9d9"
			== Hash(512).finalize().toString());

	const char s3[] = "a";
	Hash test3(512);
	for (long int i = 0 ; i < 72; ++i)
		test3.nextData(s3, strlen(s3));
	REQUIRE("7f357312e6b56e2c7be02b89b2c3e5fd411c5640083abdbf67ebb411710168f3d4766d230ae159b4738a16b2cfbff8b0be462d606cbe72b60fc92962c4d2a864cd74e9415a29df3993540595cf5ac002839cdb474db1ffe0c9ebb6864997ca57a95b862538b37fa80fe3e0d926ecccd95e5f2c1323e83f4786f88dab9465c6a5909da44ccd1010206c8391faed8de72e9ab3cfb925febb570bcf9869502f4b436ecba91aa151e845336c6fb912d0456e7a59c62ce80315dd7a68e9342862b3726d783da8675fa929990f9dcd2dd6f6bfe35aaa94915294fe41c6168ede13e5129311d1223deb7421840bda2ae2f7cb43f1d268c0294871526ce488ded7088b52473a7590277a8d0cd6cdf66d807e23ba610af3a9a3a4ab514db359c8c1b5c9eb031be67e2f47ebae0a6c402694bfa3b8aadc142585721e507137a08aab8ab7a2dbddd78831c8f28edd5049d19cb2b6bea23233bd09c73ea60502196790e1ab7f72974cd72fc274ce5d92de1f631888ab685e4e4a201eb86aa5df0b61eb006da3892c8044a5bd507db9024f1dace542362e5f9f246ac9c4ea407ee6cbb172049a8fd5659ede35b17a9a0a63bb8f2bf806209aa49b3a305f754fc22872a45babb39f3c1220eec11a8d4eecca7999dfe526afa3a5a7f5999604017cb92524d4208f690436a5633901be39b6160db7a34b794a70823f2dd4de8d91a9d186742468b7"
			== test3.finalize().toString());

	const std::vector<char> s4(269, '\0');
	REQUIRE("f9cd41bd6216be4c01a8366e752cfa83990a131ad776dc40a5463bca166933a2"
			== Hash(32).nextData(s4.data(), s4.size()).finalize().toString());

	const std::vector<char> s5(270, '\0');
	REQUIRE("f9cd41bd6216be4c01a8366e752cfa83990a131ad776dc40a5463bca166933a2"
			== Hash(32).nextData(s5.data() + 1, s5.size() - 1).finalize().toString());

	const int s6[2] = {0};
	const char s6_2[8] = {0};
	REQUIRE(Hash(32).nextData(Hash::Span<const int>(s6)).finalize().toString()
			== Hash(32).nextData(s6_2).finalize().toString());

	const unsigned char s7[] = {0x00, 0x0A};
	const auto s7_1 = Hash(32).nextData(s7, 2).finalize().toVector();
	const auto s7_2 = Hash(32).nextData(s7).finalize().toVector();
	REQUIRE(s7_1 == s7_2);

	REQUIRE(0xdc32e64d12751ced == std::hash<Hash> {}(Hash(512).finalize()));

	Hash h18(512);
	Hash h18_2(512);
	h18 = h18_2;
	REQUIRE(h18.finalize().toVector() == h18_2.finalize().toVector());
	h18 = std::move(h18_2);
	REQUIRE(h18.finalize().toVector() == Hash(512).finalize().toVector());

	Hash h19(512);
	Hash::BaseType h19_2(512);
	h19 = h19_2;
	REQUIRE(h19.finalize().toVector() == h19_2.finalize().toVector());
	h19 = std::move(h19_2);
	REQUIRE(h19.finalize().toVector() == Hash::BaseType(512).finalize().toVector());
}
