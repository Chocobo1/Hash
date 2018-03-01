# compiler options
CXX       += -fsanitize=undefined
CXXFLAGS   = -std=c++14 -pipe -Wall -Wextra -Wpedantic -fmax-errors=2 -fdiagnostics-color=auto -O2 -g
#LDFLAGS	   = -s
SRC_NAME   = main \
	test_blake1_224 test_blake1_256 test_blake1_384 test_blake1_512 \
	test_blake2 test_blake2s \
	test_cshake \
	test_crc_32 \
	test_has_160 \
	test_md2 test_md4 test_md5 \
	test_ripemd_128 test_ripemd_160 test_ripemd_256 test_ripemd_320 \
	test_sha1 \
	test_sha2_224 test_sha2_256 test_sha2_384 test_sha2_512 \
	test_sha2_512_224 test_sha2_512_256 \
	test_sha3 test_shake \
	test_sm3 \
	test_tiger \
	test_tuple_hash \
	test_whirlpool
EXECUTABLE = run_tests
SRC_EXT    = cpp


# targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(patsubst %,%.o,$(SRC_NAME))
	$(CXX) $(LDFLAGS) $(patsubst %,%.o,$(SRC_NAME)) -o $(EXECUTABLE)
	@echo

$(patsubst %,%.o,$(SRC_NAME)): %.o : %.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(patsubst %,./%.o,$(SRC_NAME)) ./$(EXECUTABLE)
