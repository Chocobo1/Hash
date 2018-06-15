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

#ifndef CHOCOBO1_SHA3_H
#define CHOCOBO1_SHA3_H

#include "gsl/span"

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <type_traits>
#include <vector>


namespace Chocobo1
{
	// Use these!!
	// SHA3_224();
	// SHA3_256();
	// SHA3_384();
	// SHA3_512();

	// SHAKE_128(const unsigned int digestLengthInBytes);
	// SHAKE_256(const unsigned int digestLengthInBytes);
}


namespace Chocobo1
{
// users should ignore things in this namespace

namespace Hash
{
#ifndef CHOCOBO1_HASH_BUFFER_IMPL
#define CHOCOBO1_HASH_BUFFER_IMPL
	template <typename T, std::size_t N>
	class Buffer
	{
		public:
			using value_type = T;
			using size_type = std::size_t;
			using reference = T&;
			using iterator = T*;
			using const_iterator = const T*;

			constexpr Buffer() = default;
			constexpr explicit Buffer(const Buffer &) = default;

			constexpr Buffer(const std::initializer_list<T> initList)
			{
#if !defined(NDEBUG)
				// check if out-of-bounds
				m_array.at(m_dataEndIdx + initList.size() - 1);
#endif

				for (const auto &i : initList)
				{
					m_array[m_dataEndIdx] = i;
					++m_dataEndIdx;
				}
			}

			template <typename InputIt>
			constexpr Buffer(const InputIt first, const InputIt last)
			{
				for (InputIt iter = first; iter != last; ++iter)
				{
					this->fill(*iter);
				}
			}

			constexpr T& operator[](const size_type pos)
			{
				return m_array[pos];
			}

			constexpr T operator[](const size_type pos) const
			{
				return m_array[pos];
			}

			constexpr void fill(const T &value, const size_type count = 1)
			{
#if !defined(NDEBUG)
				// check if out-of-bounds
				m_array.at(m_dataEndIdx + count - 1);
#endif

				for (size_type i = 0; i < count; ++i)
				{
					m_array[m_dataEndIdx] = value;
					++m_dataEndIdx;
				}
			}

			template <typename InputIt>
			constexpr void push_back(const InputIt first, const InputIt last)
			{
				for (InputIt iter = first; iter != last; ++iter)
				{
					this->fill(*iter);
				}
			}

			constexpr void clear()
			{
				m_array = {};
				m_dataEndIdx = 0;
			}

			constexpr bool empty() const
			{
				return (m_dataEndIdx == 0);
			}

			constexpr size_type size() const
			{
				return m_dataEndIdx;
			}

			constexpr const T* data() const
			{
				return m_array.data();
			}

			constexpr iterator begin()
			{
				return m_array.data();
			}

			constexpr const_iterator begin() const
			{
				return m_array.data();
			}

			constexpr iterator end()
			{
				if (N == 0)
					return m_array.data();
				return &m_array[m_dataEndIdx];
			}

			constexpr const_iterator end() const
			{
				if (N == 0)
					return m_array.data();
				return &m_array[m_dataEndIdx];
			}

		private:
			std::array<T, N> m_array {};
			size_type m_dataEndIdx = 0;
	};
#endif


namespace SHA3_NS
{
	template<int R, unsigned int P>  // `R`: see m_params. `P`: suffix + padding
	class Keccak
	{
		// https://dx.doi.org/10.6028/NIST.FIPS.202

		public:
			using Byte = uint8_t;

			template <typename T>
			using Span = gsl::span<T>;


			constexpr explicit Keccak(const unsigned int digestLength);

			constexpr void reset();
			Keccak& finalize();  // after this, only `toString()`, `toVector()`, `reset()` are available

			std::string toString() const;
			std::vector<Byte> toVector() const;

			constexpr Keccak& addData(const Span<const Byte> inData);
			constexpr Keccak& addData(const void *ptr, const long int length);

		private:
			constexpr void addDataImpl(const Span<const Byte> data);
			std::vector<typename Keccak::Byte> stateToVector() const;

			struct
			{
				// Keccak-512(M) = KECCAK[1024](M || 01, 512)
				//                 KECCAK[ c  ](   _   ,  d )

				// loosely related to the spec
//				unsigned int b = 1600;  // fixed for all Keccak instances
//				unsigned int c = 1024;
//				unsigned int d = 512 / 8;  // digest size
//				unsigned int r = 576 / 8;  // (b - c)  // IOW, BLOCK_SIZE
				unsigned int w = 64;       // (b / 25)
			} const m_params;
			const unsigned int m_digestLength;

			Buffer<Byte, R> m_buffer;
			std::vector<Byte> m_final;

			uint64_t m_state[5][5] = {};  // [y][x]
	};


	// helpers
	template <typename T>
	class Loader
	{
		// this class workaround loading data from unaligned memory boundaries
		// also eliminate endianness issues
		public:
			explicit constexpr Loader(const void *ptr)
				: m_ptr(static_cast<const uint8_t *>(ptr))
			{
			}

			constexpr T operator[](const size_t idx) const
			{
				static_assert(std::is_same<T, uint64_t>::value, "");
				// handle specific endianness here
				const uint8_t *ptr = m_ptr + (sizeof(T) * idx);
				return  ( (static_cast<T>(*(ptr + 0)) <<  0)
						| (static_cast<T>(*(ptr + 1)) <<  8)
						| (static_cast<T>(*(ptr + 2)) << 16)
						| (static_cast<T>(*(ptr + 3)) << 24)
						| (static_cast<T>(*(ptr + 4)) << 32)
						| (static_cast<T>(*(ptr + 5)) << 40)
						| (static_cast<T>(*(ptr + 6)) << 48)
						| (static_cast<T>(*(ptr + 7)) << 56));
			}

		private:
			const uint8_t *m_ptr;
	};

	template <typename R, typename T>
	constexpr R ror(const T x, const unsigned int s)
	{
		static_assert(std::is_unsigned<R>::value, "");
		const R mask = -1;
		return ((x >> s) & mask);
	}

	template <typename T>
	constexpr T rotl(const T x, const unsigned int s)
	{
		static_assert(std::is_unsigned<T>::value, "");
		if (s == 0)
			return x;
		return ((x << s) | (x >> ((sizeof(T) * 8) - s)));
	}

	//
	template <int R, unsigned int P>
	constexpr Keccak<R, P>::Keccak(const unsigned int digestLength)
		: m_params()
		, m_digestLength(digestLength)
	{
		m_final.reserve(m_digestLength);
		reset();
	}

	template <int R, unsigned int P>
	constexpr void Keccak<R, P>::reset()
	{
		m_buffer.clear();
		m_final.clear();

		for (int y = 0; y < 5; ++y)
			for (int x = 0; x < 5; ++x)
				m_state[y][x] = 0;
	}

	template <int R, unsigned int P>
	Keccak<R, P>& Keccak<R, P>::finalize()
	{
		// add padding
		// the padding is reversed due to "B.1 Conversion Functions - Algorithm 11: b2h(S)"
		m_buffer.fill(P);

		const size_t len = R - (m_buffer.size() % R);
		m_buffer.fill(0, len);
		m_buffer[m_buffer.size() - 1] |= (1 << 7);

		addDataImpl({m_buffer.begin(), m_buffer.end()});
		m_buffer.clear();

		// squish out
		while (true)
		{
			const std::vector<Byte> state = stateToVector();
			m_final.insert(m_final.end(), state.begin(), (state.begin() + R));
			if (m_final.size() >= m_digestLength)
				break;

			addDataImpl(std::array<Byte, R> {});
		}

		m_final.resize(m_digestLength);
		return (*this);
	}

	template <int R, unsigned int P>
	std::string Keccak<R, P>::toString() const
	{
		const auto v = toVector();
		std::string ret;
		ret.reserve(2 * v.size());
		for (const auto c : v)
		{
			char buf[3];
			snprintf(buf, sizeof(buf), "%02x", c);
			ret.append(buf);
		}

		return ret;
	}

	template <int R, unsigned int P>
	std::vector<typename Keccak<R, P>::Byte> Keccak<R, P>::toVector() const
	{
		return m_final;
	}

	template <int R, unsigned int P>
	constexpr Keccak<R, P>& Keccak<R, P>::addData(const Span<const Byte> inData)
	{
		Span<const Byte> data = inData;

		if (!m_buffer.empty())
		{
			const size_t len = std::min<size_t>((R - m_buffer.size()), data.size());  // try fill to BLOCK_SIZE bytes
			m_buffer.push_back(data.begin(), (data.begin() + len));

			if (m_buffer.size() < R)  // still doesn't fill the buffer
				return (*this);

			addDataImpl({m_buffer.begin(), m_buffer.end()});
			m_buffer.clear();

			data = data.subspan(len);
		}

		const size_t dataSize = data.size();
		if (dataSize < R)
		{
			m_buffer = {data.begin(), data.end()};
			return (*this);
		}

		const size_t len = dataSize - (dataSize % R);  // align on BLOCK_SIZE bytes
		addDataImpl(data.first(len));

		if (len < dataSize)  // didn't consume all data
			m_buffer = {(data.begin() + len), data.end()};

		return (*this);
	}

	template <int R, unsigned int P>
	constexpr Keccak<R, P>& Keccak<R, P>::addData(const void *ptr, const long int length)
	{
		// gsl::span::index_type = long int
		return addData({reinterpret_cast<const Byte*>(ptr), length});
	}

	template <int R, unsigned int P>
	constexpr void Keccak<R, P>::addDataImpl(const Span<const Byte> data)
	{
		assert((data.size() % R) == 0);

		for (size_t iter = 0, iend = static_cast<size_t>(data.size() / R); iter < iend; ++iter)
		{
			const Loader<uint64_t> m(reinterpret_cast<const Byte *>(data.data() + (iter * R)));
			for (int i = 0; i < (R / 8); ++i)
			{
 				uint64_t *statePtr = reinterpret_cast<uint64_t *>(m_state);
 				statePtr[i] ^= m[i];
			}

			const auto roundFunction = [this](const int indexRound)
			{
				const auto theta = [this]()
				{
					const uint64_t c[5] =
					{
						(m_state[0][0] ^ m_state[1][0] ^ m_state[2][0] ^ m_state[3][0] ^ m_state[4][0]),
						(m_state[0][1] ^ m_state[1][1] ^ m_state[2][1] ^ m_state[3][1] ^ m_state[4][1]),
						(m_state[0][2] ^ m_state[1][2] ^ m_state[2][2] ^ m_state[3][2] ^ m_state[4][2]),
						(m_state[0][3] ^ m_state[1][3] ^ m_state[2][3] ^ m_state[3][3] ^ m_state[4][3]),
						(m_state[0][4] ^ m_state[1][4] ^ m_state[2][4] ^ m_state[3][4] ^ m_state[4][4])
					};
					const uint64_t d[5] =
					{
						c[4] ^ rotl(c[1], 1),
						c[0] ^ rotl(c[2], 1),
						c[1] ^ rotl(c[3], 1),
						c[2] ^ rotl(c[4], 1),
						c[3] ^ rotl(c[0], 1)
					};

					for (int y = 0; y < 5; ++y)
					{
						m_state[y][0] ^= d[0];
						m_state[y][1] ^= d[1];
						m_state[y][2] ^= d[2];
						m_state[y][3] ^= d[3];
						m_state[y][4] ^= d[4];
					}
				};
				const auto rhoPiChi = [this]()
				{
					// rho, pi combined
					const uint64_t tmp[5][5] =
					{
						{
							rotl(m_state[0][0], (  0 % m_params.w)),
							rotl(m_state[1][1], (300 % m_params.w)),
							rotl(m_state[2][2], (171 % m_params.w)),
							rotl(m_state[3][3], ( 21 % m_params.w)),
							rotl(m_state[4][4], ( 78 % m_params.w))
						},
						{
							rotl(m_state[0][3], ( 28 % m_params.w)),
							rotl(m_state[1][4], (276 % m_params.w)),
							rotl(m_state[2][0], (  3 % m_params.w)),
							rotl(m_state[3][1], ( 45 % m_params.w)),
							rotl(m_state[4][2], (253 % m_params.w))
						},
						{
							rotl(m_state[0][1], (  1 % m_params.w)),
							rotl(m_state[1][2], (  6 % m_params.w)),
							rotl(m_state[2][3], (153 % m_params.w)),
							rotl(m_state[3][4], (136 % m_params.w)),
							rotl(m_state[4][0], (210 % m_params.w))
						},
						{
							rotl(m_state[0][4], ( 91 % m_params.w)),
							rotl(m_state[1][0], ( 36 % m_params.w)),
							rotl(m_state[2][1], ( 10 % m_params.w)),
							rotl(m_state[3][2], ( 15 % m_params.w)),
							rotl(m_state[4][3], (120 % m_params.w))
						},
						{
							rotl(m_state[0][2], (190 % m_params.w)),
							rotl(m_state[1][3], ( 55 % m_params.w)),
							rotl(m_state[2][4], (231 % m_params.w)),
							rotl(m_state[3][0], (105 % m_params.w)),
							rotl(m_state[4][1], ( 66 % m_params.w))
						}
					};
					// chi
					for (int y = 0; y < 5; ++y)
					{
						m_state[y][0] = tmp[y][0] ^ ((~tmp[y][1]) & tmp[y][2]);
						m_state[y][1] = tmp[y][1] ^ ((~tmp[y][2]) & tmp[y][3]);
						m_state[y][2] = tmp[y][2] ^ ((~tmp[y][3]) & tmp[y][4]);
						m_state[y][3] = tmp[y][3] ^ ((~tmp[y][4]) & tmp[y][0]);
						m_state[y][4] = tmp[y][4] ^ ((~tmp[y][0]) & tmp[y][1]);
					}
				};
				const auto iota = [this](const int indexRound)
				{
					static const uint64_t roundConstantTable[24] =
					{
						0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000, 0x000000000000808B, 0x0000000080000001,
						0x8000000080008081, 0x8000000000008009, 0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
						0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
						0x000000000000800A, 0x800000008000000A, 0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
					};

					m_state[0][0] ^= roundConstantTable[indexRound];
				};

				theta();
				rhoPiChi();
				iota(indexRound);
			};

			for (int i = 0; i < 24; ++i)
				roundFunction(i);
		}
	}

	template <int R, unsigned int P>
	std::vector<typename Keccak<R, P>::Byte> Keccak<R, P>::stateToVector() const
	{
		const Span<const uint64_t> state(reinterpret_cast<const uint64_t *>(m_state), 25);
		const int dataSize = sizeof(typename decltype(state)::value_type);

		std::vector<Byte> ret;
		ret.reserve(dataSize * state.size());
		for (const auto i : state)
		{
			for (int j = 0; j < dataSize; ++j)
				ret.emplace_back(ror<Byte>(i, (j * 8)));
		}

		return ret;
	}
}
}
	struct SHA3_224 : Hash::SHA3_NS::Keccak<(1152 / 8), 0x06> { SHA3_224() : Hash::SHA3_NS::Keccak<(1152 / 8), 0x06>(224 / 8) {} };
	struct SHA3_256 : Hash::SHA3_NS::Keccak<(1088 / 8), 0x06> { SHA3_256() : Hash::SHA3_NS::Keccak<(1088 / 8), 0x06>(256 / 8) {} };
	struct SHA3_384 : Hash::SHA3_NS::Keccak<( 832 / 8), 0x06> { SHA3_384() : Hash::SHA3_NS::Keccak<( 832 / 8), 0x06>(384 / 8) {} };
	struct SHA3_512 : Hash::SHA3_NS::Keccak<( 576 / 8), 0x06> { SHA3_512() : Hash::SHA3_NS::Keccak<( 576 / 8), 0x06>(512 / 8) {} };
	struct SHAKE_128 : Hash::SHA3_NS::Keccak<(1344 / 8), 0x1F> { explicit SHAKE_128(const unsigned int d) : Hash::SHA3_NS::Keccak<(1344 / 8), 0x1F>(d) {} };
	struct SHAKE_256 : Hash::SHA3_NS::Keccak<(1088 / 8), 0x1F> { explicit SHAKE_256(const unsigned int d) : Hash::SHA3_NS::Keccak<(1088 / 8), 0x1F>(d) {} };
}

#endif  // CHOCOBO1_SHA3_H
