# Benchmark.md

* CPU: Intel Haskell i7

* The input file is created via
    ```shell
    $ dd if=/dev/urandom of=/tmp/test count=1024000
    1024000+0 records in
    1024000+0 records out
    524288000 bytes (524 MB, 500 MiB) copied, 4.64584 s, 113 MB/s
    ```

* Compiler:
    ```shell
    $ gcc --version
    gcc (GCC) 7.1.1 20170630
    ```

* Version (x86-64):
  * [Chocobo1/Hash](https://github.com/Chocobo1/Hash) git master, configured with `-O2`
  * [Coreutils](https://www.gnu.org/software/coreutils/coreutils.html) 8.28, configured with `-O2 --with-openssl=no`
  * [RHash](https://sourceforge.net/projects/rhash/) 1.3.5, configured with `-O2`

* Results:

| Hash      | Chocobo1/Hash        | Coreutils            | %               |
| --------- | -------------------- | -------------------- | --------------- |
| BLAKE2b   | 1.194s / 418.8 MiB/s | 1.151s / 434.4 MiB/s | 3.7% slower     |
| MD5       | 1.321s / 378.5 MiB/s | 1.300s / 384.6 MiB/s | 1.6% slower     |
| SHA-1     | 1.470s / 340.1 MiB/s | 1.512s / 330.7 MiB/s | **2.8% faster** |
| SHA-2-256 | 3.532s / 141.6 MiB/s | 3.790s / 131.9 MiB/s | **6.8% faster** |

| Hash       | Chocobo1/Hash        | RHash                | %                |
| ---------- | -------------------- | -------------------- | ---------------- |
| SHA-3-512  | 9.387s /  53.3 MiB/s | 9.716s /  51.5 MiB/s | **3.4% faster**  |
| Tiger1-192 | 1.422s / 351.6 MiB/s | 1.440s / 347.2 MiB/s | **1.3% faster**  |
| RIPEMD-160 | 1.924s / 259.9 MiB/s | 3.010s / 166.1 MiB/s | **36.1% faster** |
| WHIRLPOOL  | 5.018s /  99.6 MiB/s | 5.859s /  85.3 MiB/s | **14.4% faster** |

Note: result will vary for different compilers, depends on how good the compiler optimizer are. So far gcc has the best results
