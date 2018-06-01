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

#ifndef CHOCOBO1_SHA1_H
#define CHOCOBO1_SHA1_H

#include "gsl/span"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>


namespace Chocobo1
{
	// Use these!!
	// SHA1();
}


namespace Chocobo1
{
// users should ignore things in this namespace
namespace SHA1_Hash
{
	class SHA1
	{
		// https://tools.ietf.org/html/rfc3174

		public:
			template <typename T>
			using Span = gsl::span<T>;

			typedef uint8_t Byte;


			SHA1();

			void reset();
			SHA1& finalize();  // after this, only `toString()`, `toVector()`, `reset()` are available

			std::string toString() const;
			std::vector<SHA1::Byte> toVector() const;

			SHA1& addData(const Span<const Byte> inData);
			SHA1& addData(const void *ptr, const long int length);

		private:
			void addDataImpl(const Span<const Byte> data);

			const unsigned int BLOCK_SIZE = 64;

			std::vector<Byte> m_buffer;
			uint64_t m_sizeCounter;

			uint32_t m_state[5];
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
				static_assert(std::is_same<T, uint32_t>::value, "");
				// handle specific endianness here
				const uint8_t *ptr = m_ptr + (sizeof(T) * idx);
				return  ( (static_cast<T>(*(ptr + 0)) << 24)
						| (static_cast<T>(*(ptr + 1)) << 16)
						| (static_cast<T>(*(ptr + 2)) <<  8)
						| (static_cast<T>(*(ptr + 3)) <<  0));
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
	SHA1::SHA1()
	{
		m_buffer.reserve(BLOCK_SIZE * 2);  // x2 for paddings
		reset();
	}

	void SHA1::reset()
	{
		m_buffer.clear();
		m_sizeCounter = 0;

		m_state[0] = 0x67452301;
		m_state[1] = 0xEFCDAB89;
		m_state[2] = 0x98BADCFE;
		m_state[3] = 0x10325476;
		m_state[4] = 0xC3D2E1F0;
	}

	SHA1& SHA1::finalize()
	{
		m_sizeCounter += m_buffer.size();

		// append 1 bit
		m_buffer.emplace_back(1 << 7);

		// append paddings
		const size_t len = BLOCK_SIZE - ((m_buffer.size() + 8) % BLOCK_SIZE);
		m_buffer.insert(m_buffer.end(), (len + 8), 0);

		// append size in bits
		const uint64_t sizeCounterBits = m_sizeCounter * 8;
		const uint32_t sizeCounterBitsL = ror<uint32_t>(sizeCounterBits, 0);
		const uint32_t sizeCounterBitsH = ror<uint32_t>(sizeCounterBits, 32);
		for (int i = 0; i < 4; ++i)
		{
			m_buffer[m_buffer.size() - 8 + i] = ror<Byte>(sizeCounterBitsH, (8 * (3 - i)));
			m_buffer[m_buffer.size() - 4 + i] = ror<Byte>(sizeCounterBitsL, (8 * (3 - i)));
		}

		addDataImpl(m_buffer);
		m_buffer.clear();

		return (*this);
	}

	std::string SHA1::toString() const
	{
		std::string ret;
		const auto v = toVector();
		ret.reserve(2 * v.size());
		for (const auto &i : v)
		{
			char buf[3];
			snprintf(buf, sizeof(buf), "%02x", i);
			ret.append(buf);
		}

		return ret;
	}

	std::vector<SHA1::Byte> SHA1::toVector() const
	{
		const Span<const uint32_t> state(m_state);
		const int dataSize = sizeof(decltype(state)::value_type);

		std::vector<Byte> ret;
		ret.reserve(dataSize * state.size());
		for (const auto &i : state)
		{
			for (int j = (dataSize - 1); j >= 0; --j)
				ret.emplace_back(ror<Byte>(i, (j * 8)));
		}

		return ret;
	}

	SHA1& SHA1::addData(const Span<const Byte> inData)
	{
		Span<const Byte> data = inData;

		if (!m_buffer.empty())
		{
			const size_t len = std::min<size_t>((BLOCK_SIZE - m_buffer.size()), data.size());  // try fill to BLOCK_SIZE bytes
			m_buffer.insert(m_buffer.end(), data.begin(), data.begin() + len);

			if (m_buffer.size() < BLOCK_SIZE)  // still doesn't fill the buffer
				return (*this);

			addDataImpl(m_buffer);
			m_buffer.clear();

			data = data.subspan(len);
		}

		const size_t dataSize = data.size();
		if (dataSize < BLOCK_SIZE)
		{
			m_buffer = {data.begin(), data.end()};
			return (*this);
		}

		const size_t len = dataSize - (dataSize % BLOCK_SIZE);  // align on BLOCK_SIZE bytes
		addDataImpl(data.first(len));

		if (len < dataSize)  // didn't consume all data
			m_buffer = {data.begin() + len, data.end()};

		return (*this);
	}

	SHA1& SHA1::addData(const void *ptr, const long int length)
	{
		// gsl::span::index_type = long int
		return addData({reinterpret_cast<const Byte*>(ptr), length});
	}

	void SHA1::addDataImpl(const Span<const Byte> data)
	{
		assert((data.size() % BLOCK_SIZE) == 0);

		m_sizeCounter += data.size();

		for (size_t i = 0, iend = static_cast<size_t>(data.size() / BLOCK_SIZE); i < iend; ++i)
		{
			const Loader<uint32_t> m(reinterpret_cast<const Byte *>(data.data() + (i * BLOCK_SIZE)));

			uint32_t a = m_state[0];
			uint32_t b = m_state[1];
			uint32_t c = m_state[2];
			uint32_t d = m_state[3];
			uint32_t e = m_state[4];

			uint32_t wTable[80];

			#ifdef sha1Round1
			#error "macro name clash"
			#else
			#define sha1Round1(a, b, c, d, e, t) \
				wTable[t] = m[t]; \
				e = rotl(a, 5) + ((b & (c ^ d)) ^ d) + e + wTable[t] + 0x5A827999;  /* alternative f */ \
				b = rotl(b, 30);

			sha1Round1(a, b, c, d, e, 0);
			sha1Round1(e, a, b, c, d, 1);
			sha1Round1(d, e, a, b, c, 2);
			sha1Round1(c, d, e, a, b, 3);
			sha1Round1(b, c, d, e, a, 4);
			sha1Round1(a, b, c, d, e, 5);
			sha1Round1(e, a, b, c, d, 6);
			sha1Round1(d, e, a, b, c, 7);
			sha1Round1(c, d, e, a, b, 8);
			sha1Round1(b, c, d, e, a, 9);
			sha1Round1(a, b, c, d, e, 10);
			sha1Round1(e, a, b, c, d, 11);
			sha1Round1(d, e, a, b, c, 12);
			sha1Round1(c, d, e, a, b, 13);
			sha1Round1(b, c, d, e, a, 14);
			sha1Round1(a, b, c, d, e, 15);
			#undef sha1Round1
			#endif

			#ifdef sha1Round1a
			#error "macro name clash"
			#else
			#define sha1Round1a(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 3] ^ wTable[t - 8] ^ wTable[t - 14] ^ wTable[t - 16]), 1); \
				e = rotl(a, 5) + ((b & (c ^ d)) ^ d) + e + wTable[t] + 0x5A827999;  /* alternative f */ \
				b = rotl(b, 30);

			sha1Round1a(e, a, b, c, d, 16);
			sha1Round1a(d, e, a, b, c, 17);
			sha1Round1a(c, d, e, a, b, 18);
			sha1Round1a(b, c, d, e, a, 19);
			#undef sha1Round1a
			#endif

			#ifdef sha1Round2
			#error "macro name clash"
			#else
			#define sha1Round2(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 3] ^ wTable[t - 8] ^ wTable[t - 14] ^ wTable[t - 16]), 1); \
				e = rotl(a, 5) + (b ^ c ^ d) + e + wTable[t] + 0x6ED9EBA1; \
				b = rotl(b, 30);

			sha1Round2(a, b, c, d, e, 20);
			sha1Round2(e, a, b, c, d, 21);
			sha1Round2(d, e, a, b, c, 22);
			sha1Round2(c, d, e, a, b, 23);
			sha1Round2(b, c, d, e, a, 24);
			sha1Round2(a, b, c, d, e, 25);
			sha1Round2(e, a, b, c, d, 26);
			sha1Round2(d, e, a, b, c, 27);
			sha1Round2(c, d, e, a, b, 28);
			sha1Round2(b, c, d, e, a, 29);
			sha1Round2(a, b, c, d, e, 30);
			sha1Round2(e, a, b, c, d, 31);
			#undef sha1Round2
			#endif

			#ifdef sha1Round2a
			#error "macro name clash"
			#else
			#define sha1Round2a(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 6] ^ wTable[t - 16] ^ wTable[t - 28] ^ wTable[t - 32]), 2);  /* alternative */ \
				e = rotl(a, 5) + (b ^ c ^ d) + e + wTable[t] + 0x6ED9EBA1; \
				b = rotl(b, 30);

			sha1Round2a(d, e, a, b, c, 32);
			sha1Round2a(c, d, e, a, b, 33);
			sha1Round2a(b, c, d, e, a, 34);
			sha1Round2a(a, b, c, d, e, 35);
			sha1Round2a(e, a, b, c, d, 36);
			sha1Round2a(d, e, a, b, c, 37);
			sha1Round2a(c, d, e, a, b, 38);
			sha1Round2a(b, c, d, e, a, 39);
			#undef sha1Round2a
			#endif

			#ifdef sha1Round3
			#error "macro name clash"
			#else
			#define sha1Round3(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 6] ^ wTable[t - 16] ^ wTable[t - 28] ^ wTable[t - 32]), 2);  /* alternative */ \
				e = rotl(a, 5) + ((b & c) | (d & (b | c))) + e + wTable[t] + 0x8F1BBCDC; \
				b = rotl(b, 30);

			sha1Round3(a, b, c, d, e, 40);
			sha1Round3(e, a, b, c, d, 41);
			sha1Round3(d, e, a, b, c, 42);
			sha1Round3(c, d, e, a, b, 43);
			sha1Round3(b, c, d, e, a, 44);
			sha1Round3(a, b, c, d, e, 45);
			sha1Round3(e, a, b, c, d, 46);
			sha1Round3(d, e, a, b, c, 47);
			sha1Round3(c, d, e, a, b, 48);
			sha1Round3(b, c, d, e, a, 49);
			sha1Round3(a, b, c, d, e, 50);
			sha1Round3(e, a, b, c, d, 51);
			sha1Round3(d, e, a, b, c, 52);
			sha1Round3(c, d, e, a, b, 53);
			sha1Round3(b, c, d, e, a, 54);
			sha1Round3(a, b, c, d, e, 55);
			sha1Round3(e, a, b, c, d, 56);
			sha1Round3(d, e, a, b, c, 57);
			sha1Round3(c, d, e, a, b, 58);
			sha1Round3(b, c, d, e, a, 59);
			#undef sha1Round3
			#endif

			#ifdef sha1Round4
			#error "macro name clash"
			#else
			#define sha1Round4(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 6] ^ wTable[t - 16] ^ wTable[t - 28] ^ wTable[t - 32]), 2);  /* alternative */ \
				e = rotl(a, 5) + (b ^ c ^ d) + e + wTable[t] + 0xCA62C1D6; \
				b = rotl(b, 30);

			sha1Round4(a, b, c, d, e, 60);
			sha1Round4(e, a, b, c, d, 61);
			sha1Round4(d, e, a, b, c, 62);
			sha1Round4(c, d, e, a, b, 63);
			sha1Round4(b, c, d, e, a, 64);
			sha1Round4(a, b, c, d, e, 65);
			sha1Round4(e, a, b, c, d, 66);
			sha1Round4(d, e, a, b, c, 67);
			sha1Round4(c, d, e, a, b, 68);
			sha1Round4(b, c, d, e, a, 69);
			sha1Round4(a, b, c, d, e, 70);
			sha1Round4(e, a, b, c, d, 71);
			sha1Round4(d, e, a, b, c, 72);
			sha1Round4(c, d, e, a, b, 73);
			sha1Round4(b, c, d, e, a, 74);
			sha1Round4(a, b, c, d, e, 75);
			sha1Round4(e, a, b, c, d, 76);
			sha1Round4(d, e, a, b, c, 77);
			sha1Round4(c, d, e, a, b, 78);
			sha1Round4(b, c, d, e, a, 79);
			#undef sha1Round4
			#endif

			// Let H0 = H0 + A, H1 = H1 + B, H2 = H2 + C, H3 = H3 + D, H4 = H4 + E.
			m_state[0] += a;
			m_state[1] += b;
			m_state[2] += c;
			m_state[3] += d;
			m_state[4] += e;
		}
	}
}
	using SHA1 = SHA1_Hash::SHA1;
}

#endif  // CHOCOBO1_SHA1_H
