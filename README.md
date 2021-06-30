# Hash [![AppVeyor_badge]][AppVeyor_link]

[AppVeyor_badge]: https://ci.appveyor.com/api/projects/status/github/Chocobo1/Hash?branch=master&svg=true
[AppVeyor_link]: https://ci.appveyor.com/project/Chocobo1/Hash

C++14 header-only easy-to-use hash library

What is included in this library:

| Name                    | Variants                                 | Website                                                                                   |
| ----------------------- | ---------------------------------------- | ----------------------------------------------------------------------------------------- |
| BLAKE1                  | 224, 256, 384, 512                       | https://131002.net/blake/                                                                 |
| BLAKE2                  | BLAKE2b, BLAKE2s                         | https://blake2.net/                                                                       |
| CRC-32                  |                                          | http://create.stephan-brumme.com/crc32/                                                   |
| HAS-160                 |                                          | https://www.tta.or.kr/eng/new/standardization/eng_ttastddesc.jsp?stdno=TTAS.KO-12.0011/R2 |
| HAS-V (unfinished)      |                                          | https://link.springer.com/chapter/10.1007%2F3-540-44983-3_15                              |
| MD2                     |                                          | https://tools.ietf.org/html/rfc1319                                                       |
| MD4                     |                                          | https://tools.ietf.org/html/rfc1320                                                       |
| MD5                     |                                          | https://tools.ietf.org/html/rfc1321                                                       |
| RIPEMD                  | 128, 160, 256, 320                       | https://homes.esat.kuleuven.be/~bosselae/ripemd160.html                                   |
| SHA-1                   |                                          | https://tools.ietf.org/html/rfc3174                                                       |
| SHA-2                   | 224, 256, 384, 512, 512-224, 512-256     | https://tools.ietf.org/html/rfc6234                                                       |
| SHA-3                   | 224, 256, 384, 512, SHAKE-128, SHAKE-256 | https://keccak.team/index.html                                                            |
| SHA-3 derived functions | CSHAKE-128, CSHAKE-256                   | http://csrc.nist.gov/groups/ST/hash/derived-functions.html                                |
|                         | TupleHash-128, TupleHash-256             |                                                                                           |
| SM3                     |                                          | https://tools.ietf.org/html/draft-sca-cfrg-sm3-02                                         |
| Tiger                   | Tiger1-128, Tiger1-160, Tiger1-192       | https://www.cs.technion.ac.il/~biham/Reports/Tiger/                                       |
|                         | Tiger2-128, Tiger2-160, Tiger2-192       |                                                                                           |
| WHIRLPOOL               |                                          | http://www.larc.usp.br/~pbarreto/WhirlpoolPage.html                                       |

If you are concerned about *security*, *state-of-the-art performance* or *whatsoever* issue,
then **take your time** and go setup OpenSSL or other proven library

Now some features:
* No/none tweaking knobs, it just works!
<br />  Works out-of-the-box on little endian & big endian machines
* Have similar performance to C implementations: see my rudimentary benchmark: [Benchmark.md](./Benchmark.md)

## Prerequisites
* Library only:
  * [C++14 compiler](https://en.cppreference.com/w/cpp/compiler_support)
* Tests & driver program:
  * [Meson build system](https://mesonbuild.com/)
  * [Ninja](https://ninja-build.org/)


## Usage
To use this library in your project:

1. [Download this repository](https://github.com/Chocobo1/Hash/archive/master.zip) and unzip it

2. Use it like:
    ```c++
    // include the header
    #include "pathToLib/sha1.h"

    // If you are using C++14 or C++17, don't forget the "gsl" folder!

    void example()
    {
      // Get an instance
      Chocobo1::SHA1 sha1;

      // Feed data & data length to it
      sha1.addData("hello", 5);

      // ... or just
      sha1.addData("hello");

      // Tell it to wrap it up
      sha1.finalize();

      // Profit! oops, not really...
      std::string result = sha1.toString();

      // A one-liner
      std::string result2 = Chocobo1::SHA1().addData("hello").finalize().toString();

      // In byte array form
      auto bytes = Chocobo1::SHA1().addData("hello").finalize().toArray();  // std::array<uint8_t, 20>

      // Some hash algorithm provides constexpr result under C++17
      constexpr uint8_t data[] = {0x00, 0xFF};
      constexpr auto bytes2 = Chocobo1::SHA1().addData(data).finalize().toArray();
    }
    ```

3. There is also a driver program included, you can find it at "[src/program](./src/program)"

    1. Compile:
        ```shell
        cd src/program
        meson _builddir  # initialize build folder
        cd _builddir
        ninja            # call the `ninja` to build our program
        ```

    2. Run:
        ```shell
        $ ./hash -md5 /path/to/file
        ```


## Run Tests
```shell
cd tests
meson _build
cd _build
ninja
./run_tests
```

## Dependencies
* [Microsoft/GSL](https://github.com/Microsoft/GSL) \
  This library is only requried when compiling with C++14 and C++17. In C++20 or later,
  `std::span` will be used in place of `gsl::span`. \
  No need to download or install it. It's already included!


## License
See [LICENSE](./LICENSE) file


## Like it?
Please consider [donate](https://myprojects.tzou.me/donation/)
