project('hash', 'cpp',
        default_options: ['buildtype=debugoptimized',
                          'cpp_std=c++14',
                          'warning_level=1',
                          'werror=true',
                          'strip=true',
                          'b_sanitize=address,undefined'
                         ])

CXXFLAGS = ''
CXXFLAGS = CXXFLAGS.split(' ')

LDFLAGS = ''
LDFLAGS = LDFLAGS.split(' ')

sources = files('main.cpp',
                'test_blake1_224.cpp', 'test_blake1_256.cpp',
                'test_blake1_384.cpp', 'test_blake1_512.cpp',
                'test_blake2.cpp', 'test_blake2s.cpp',
                'test_crc_32.cpp',
                'test_cshake.cpp',
                'test_fnv.cpp',
                'test_has_160.cpp',
                'test_md2.cpp', 'test_md4.cpp', 'test_md5.cpp',
                'test_multiple_tu_include.cpp',
                'test_ripemd_128.cpp', 'test_ripemd_160.cpp',
                'test_ripemd_256.cpp', 'test_ripemd_320.cpp',
                'test_siphash.cpp',
                'test_sha1.cpp',
                'test_sha2_224.cpp', 'test_sha2_256.cpp',
                'test_sha2_384.cpp', 'test_sha2_512.cpp',
                'test_sha2_512_224.cpp', 'test_sha2_512_256.cpp',
                'test_sha3.cpp', 'test_shake.cpp',
                'test_sm3.cpp',
                'test_tiger.cpp',
                'test_tuple_hash.cpp',
                'test_whirlpool.cpp'
               )

exe = executable('run_tests', sources,
                 #cpp_args: CXXFLAGS,
                 #link_args: LDFLAGS
                )

test('execute_test', exe)
