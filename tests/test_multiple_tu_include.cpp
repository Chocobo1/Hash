/*
 *  Chocobo1/Hash
 *
 *   Copyright 2017-2024 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

// Test headers included in different Translation Units (TU) can be linked together successfully

#include "../src/blake1_224.h"
#include "../src/blake1_256.h"
#include "../src/blake1_384.h"
#include "../src/blake1_512.h"
#include "../src/blake2.h"
#include "../src/blake2s.h"
#include "../src/crc_32.h"
#include "../src/cshake.h"
#include "../src/fnv.h"
#include "../src/has_160.h"
#include "../src/md2.h"
#include "../src/md4.h"
#include "../src/md5.h"
#include "../src/ripemd_128.h"
#include "../src/ripemd_160.h"
#include "../src/ripemd_256.h"
#include "../src/ripemd_320.h"
#include "../src/siphash.h"
#include "../src/sha1.h"
#include "../src/sha2_224.h"
#include "../src/sha2_256.h"
#include "../src/sha2_384.h"
#include "../src/sha2_512.h"
#include "../src/sha2_512_224.h"
#include "../src/sha2_512_256.h"
#include "../src/sha3.h"
#include "../src/sm3.h"
#include "../src/tiger.h"
#include "../src/tuple_hash.h"
#include "../src/whirlpool.h"
