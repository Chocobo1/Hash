/*
 *  Chocobo1/Hash
 *
 *   Copyright 2021 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

#ifndef CHOCOBO1_SIPHASH_H
#define CHOCOBO1_SIPHASH_H

#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <type_traits>
#include <vector>

#if (__cplusplus > 201703L)
#include <version>
#endif

#ifndef USE_STD_SPAN_CHOCOBO1_HASH
#if (__cpp_lib_span >= 202002L)
#define USE_STD_SPAN_CHOCOBO1_HASH 1
#else
#define USE_STD_SPAN_CHOCOBO1_HASH 0
#endif
#endif

#if (USE_STD_SPAN_CHOCOBO1_HASH == 1)
#include <span>
#else
#include "gsl/span"
#endif


namespace Chocobo1
{
	// Use these!!
	// SipHash(const Span<const Byte> key);
}


namespace Chocobo1
{
// users should ignore things in this namespace

namespace Hash
{
#ifndef CONSTEXPR_CPP17_CHOCOBO1_HASH
#if __cplusplus >= 201703L
#define CONSTEXPR_CPP17_CHOCOBO1_HASH constexpr
#else
#define CONSTEXPR_CPP17_CHOCOBO1_HASH
#endif
#endif

#if (USE_STD_SPAN_CHOCOBO1_HASH == 1)
	using IndexType = std::size_t;
#else
	using IndexType = gsl::index;
#endif

#ifndef CHOCOBO1_HASH_BUFFER_IMPL
#define CHOCOBO1_HASH_BUFFER_IMPL
	template <typename T, IndexType N>
	class Buffer
	{
		public:
			using value_type = T;
			using index_type = IndexType;
			using size_type = std::size_t;

			constexpr Buffer() = default;

			CONSTEXPR_CPP17_CHOCOBO1_HASH Buffer(const std::initializer_list<T> initList)
			{
#if !defined(NDEBUG)
				// check if out-of-bounds
				static_cast<void>(m_array.at(m_dataEndIdx + initList.size() - 1));
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

			constexpr T& operator[](const index_type pos)
			{
				return m_array[pos];
			}

			constexpr T operator[](const index_type pos) const
			{
				return m_array[pos];
			}

			CONSTEXPR_CPP17_CHOCOBO1_HASH void fill(const T &value, const index_type count = 1)
			{
#if !defined(NDEBUG)
				// check if out-of-bounds
				static_cast<void>(m_array.at(m_dataEndIdx + count - 1));
#endif

				for (index_type i = 0; i < count; ++i)
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

		private:
			std::array<T, N> m_array {};
			index_type m_dataEndIdx = 0;
	};
#endif

#ifndef CHOCOBO1_HASH_ROR_IMPL
#define CHOCOBO1_HASH_ROR_IMPL
	template <typename R, typename T>
	constexpr R ror(const T x, const unsigned int s)
	{
		static_assert(std::is_unsigned<R>::value, "");
		static_assert(std::is_unsigned<T>::value, "");
		return static_cast<R>(x >> s);
	}
#endif

#ifndef CHOCOBO1_HASH_ROTL_IMPL
#define CHOCOBO1_HASH_ROTL_IMPL
	template <typename T>
	constexpr T rotl(const T x, const unsigned int s)
	{
		static_assert(std::is_unsigned<T>::value, "");
		if (s == 0)
			return x;
		return ((x << s) | (x >> ((sizeof(T) * 8) - s)));
	}
#endif


namespace SIPHASH_NS
{
	template <int C, int D>  // <compression rounds, finalization rounds>
	class SipHash
	{
		// https://github.com/veorq/SipHash

		public:
			using Byte = uint8_t;
			using ResultArrayType = std::array<Byte, 8>;

#if (USE_STD_SPAN_CHOCOBO1_HASH == 1)
			template <typename T, std::size_t Extent = std::dynamic_extent>
			using Span = std::span<T, Extent>;
#else
			template <typename T, std::size_t Extent = gsl::dynamic_extent>
			using Span = gsl::span<T, Extent>;
#endif


			constexpr SipHash(Span<const Byte> key);

			constexpr void reset();
			CONSTEXPR_CPP17_CHOCOBO1_HASH SipHash& finalize();  // after this, only `operator T()`, `reset()`, `toArray()`, `toString()`, `toVector()` are available

			std::string toString() const;
			std::vector<Byte> toVector() const;
			CONSTEXPR_CPP17_CHOCOBO1_HASH ResultArrayType toArray() const;
			template <typename T>
			CONSTEXPR_CPP17_CHOCOBO1_HASH operator T() const noexcept;

			constexpr SipHash& addData(Span<const Byte> inData);
			constexpr SipHash& addData(const void *ptr, std::size_t length);
			template <std::size_t N>
			constexpr SipHash& addData(const Byte (&array)[N]);
			template <typename T, std::size_t N>
			SipHash& addData(const T (&array)[N]);
			template <typename T>
			SipHash& addData(Span<T> inSpan);

			friend constexpr bool operator==(const SipHash &left, const SipHash &right)
			{
				for (int i = 0; i < 4; ++i)
				{
					if (left.m_state[i] != right.m_state[i])
						return false;
				}
				return true;
			}
			friend constexpr bool operator!=(const SipHash &left, const SipHash &right)
			{
				return !(left == right);
			}

		private:
			constexpr void addDataImpl(Span<const Byte> data);
			constexpr void sipRound();

			static constexpr int BLOCK_SIZE = 8;

			Buffer<Byte, (BLOCK_SIZE * 2)> m_buffer;  // x2 for paddings
			uint8_t m_sizeCounter = 0;

			uint64_t m_key[2] = {};
			uint64_t m_state[4] = {};
	};


	// helpers
	template <typename T>
	class Loader
	{
		// this class workaround loading data from unaligned memory boundaries
		// also eliminate endianness issues
		public:
			explicit constexpr Loader(const uint8_t *ptr)
				: m_ptr(ptr)
			{
			}

			constexpr T operator[](const IndexType idx) const
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


	//
	template <int C, int D>
	constexpr SipHash<C, D>::SipHash(const Span<const Byte> key)
	{
		static_assert((CHAR_BIT == 8), "Sorry, we don't support exotic CPUs");
		assert(key.size() == 16);

		const Loader<uint64_t> keyView(key.data());
		m_key[0] = keyView[0];
		m_key[1] = keyView[1];

		reset();
	}

	template <int C, int D>
	constexpr void SipHash<C, D>::reset()
	{
		m_buffer.clear();
		m_sizeCounter = 0;

		m_state[0] = m_key[0] ^ 0x736f6d6570736575;
		m_state[1] = m_key[1] ^ 0x646f72616e646f6d;
		m_state[2] = m_key[0] ^ 0x6c7967656e657261;
		m_state[3] = m_key[1] ^ 0x7465646279746573;
	}

	template <int C, int D>
	CONSTEXPR_CPP17_CHOCOBO1_HASH SipHash<C, D>& SipHash<C, D>::finalize()
	{
		m_sizeCounter += static_cast<uint8_t>(m_buffer.size());

		// append paddings
		const auto len = static_cast<int>(BLOCK_SIZE - 1 - m_buffer.size());
		m_buffer.fill(0, len);

		// append size
		m_buffer.fill(m_sizeCounter, 1);

		addDataImpl({m_buffer.data(), m_buffer.size()});
		m_buffer.clear();

		m_state[2] ^= 0xff;
		for (int d = 0; d < D; ++d)
			sipRound();

		return (*this);
	}

	template <int C, int D>
	std::string SipHash<C, D>::toString() const
	{
		const auto digest = toArray();
		std::string ret;
		ret.resize(2 * digest.size());

		auto *retPtr = &ret.front();
		for (const auto c : digest)
		{
			const Byte upper = ror<Byte>(c, 4);
			*(retPtr++) = static_cast<char>((upper < 10) ? (upper + '0') : (upper - 10 + 'a'));

			const Byte lower = c & 0xf;
			*(retPtr++) = static_cast<char>((lower < 10) ? (lower + '0') : (lower - 10 + 'a'));
		}

		return ret;
	}

	template <int C, int D>
	std::vector<typename SipHash<C, D>::Byte> SipHash<C, D>::toVector() const
	{
		const auto digest = toArray();
		return {digest.begin(), digest.end()};
	}

	template <int C, int D>
	CONSTEXPR_CPP17_CHOCOBO1_HASH typename SipHash<C, D>::ResultArrayType SipHash<C, D>::toArray() const
	{
		const uint64_t result = m_state[0] ^ m_state[1] ^ m_state[2] ^ m_state[3];
		const int dataSize = sizeof(result);

		ResultArrayType ret {};
		auto *retPtr = ret.data();
		for (int j = (dataSize - 1); j >= 0; --j)
			*(retPtr++) = ror<Byte>(result, (j * 8));

		return ret;
	}

	template <int C, int D>
	template <typename T>
	CONSTEXPR_CPP17_CHOCOBO1_HASH SipHash<C, D>::operator T() const noexcept
	{
		static_assert(std::is_unsigned<T>::value, "");

		const auto digest = toArray();
		T ret = 0;
		for (int i = 0, iMax = static_cast<int>(std::min(sizeof(T), digest.size())); i < iMax; ++i)
		{
			ret <<= 8;
			ret |= digest[i];
		}
		return ret;
	}

	template <int C, int D>
	constexpr SipHash<C, D>& SipHash<C, D>::addData(const Span<const Byte> inData)
	{
		Span<const Byte> data = inData;

		if (!m_buffer.empty())
		{
			const size_t len = std::min<size_t>((BLOCK_SIZE - m_buffer.size()), data.size());  // try fill to BLOCK_SIZE bytes
			m_buffer.push_back(data.begin(), (data.begin() + len));

			if (m_buffer.size() < BLOCK_SIZE)  // still doesn't fill the buffer
				return (*this);

			addDataImpl({m_buffer.data(), m_buffer.size()});
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
			m_buffer = {(data.begin() + len), data.end()};

		return (*this);
	}

	template <int C, int D>
	constexpr SipHash<C, D>& SipHash<C, D>::addData(const void *ptr, const std::size_t length)
	{
		// Span::size_type = std::size_t
		return addData({static_cast<const Byte*>(ptr), length});
	}

	template <int C, int D>
	template <std::size_t N>
	constexpr SipHash<C, D>& SipHash<C, D>::addData(const Byte (&array)[N])
	{
		return addData({array, N});
	}

	template <int C, int D>
	template <typename T, std::size_t N>
	SipHash<C, D>& SipHash<C, D>::addData(const T (&array)[N])
	{
		return addData({reinterpret_cast<const Byte*>(array), (sizeof(T) * N)});
	}

	template <int C, int D>
	template <typename T>
	SipHash<C, D>& SipHash<C, D>::addData(const Span<T> inSpan)
	{
		return addData({reinterpret_cast<const Byte*>(inSpan.data()), inSpan.size_bytes()});
	}

	template <int C, int D>
	constexpr void SipHash<C, D>::addDataImpl(const Span<const Byte> data)
	{
		assert((data.size() % BLOCK_SIZE) == 0);

		m_sizeCounter += static_cast<uint8_t>(data.size());

		for (size_t i = 0, iend = static_cast<size_t>(data.size() / BLOCK_SIZE); i < iend; ++i)
		{
			const Loader<uint64_t> m(static_cast<const Byte *>(data.data() + (i * BLOCK_SIZE)));

			m_state[3] ^= m[0];
			for (int c = 0; c < C; ++c)
				sipRound();
			m_state[0] ^= m[0];
		}
	}

	template <int C, int D>
	constexpr void SipHash<C, D>::sipRound()
	{
		m_state[0] += m_state[1];
		m_state[2] += m_state[3];
		m_state[1] = rotl(m_state[1], 13);
		m_state[3] = rotl(m_state[3], 16);
		m_state[1] ^= m_state[0];
		m_state[3] ^= m_state[2];
		m_state[0] = rotl(m_state[0], 32);
		m_state[2] += m_state[1];
		m_state[0] += m_state[3];
		m_state[1] = rotl(m_state[1], 17);
		m_state[3] = rotl(m_state[3], 21);
		m_state[1] ^= m_state[2];
		m_state[3] ^= m_state[0];
		m_state[2] = rotl(m_state[2], 32);
	}
}
}

	using SipHash = Hash::SIPHASH_NS::SipHash<2, 4>;
}

namespace std
{
	template <int C, int D>
	struct hash<Chocobo1::Hash::SIPHASH_NS::SipHash<C, D>>
	{
		CONSTEXPR_CPP17_CHOCOBO1_HASH size_t operator()(const Chocobo1::Hash::SIPHASH_NS::SipHash<C, D> &hash) const noexcept
		{
			return hash;
		}
	};
}

#endif  // CHOCOBO1_SIPHASH_H
