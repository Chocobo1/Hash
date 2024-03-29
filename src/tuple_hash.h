/*
 *  Chocobo1/Hash
 *
 *   Copyright 2017-2020 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

#ifndef CHOCOBO1_TUPLE_HASH_H
#define CHOCOBO1_TUPLE_HASH_H

#include "cshake.h"

#include <climits>
#include <cmath>
#include <cstdint>
#include <string>
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
	// TupleHash_128(const int digestLengthInBytes, const std::string &customize = {});
	// TupleHash_256(const int digestLengthInBytes, const std::string &customize = {});
}


namespace Chocobo1
{
// users should ignore things in this namespace
namespace Hash
{
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


namespace TupleHash_NS
{
	template <typename Alg>
	class TupleHash
	{
		// https://doi.org/10.6028/NIST.SP.800-185

		public:
			using Byte = uint8_t;

#if (USE_STD_SPAN_CHOCOBO1_HASH == 1)
			template <typename T, std::size_t Extent = std::dynamic_extent>
			using Span = std::span<T, Extent>;
#else
			template <typename T, std::size_t Extent = gsl::dynamic_extent>
			using Span = gsl::span<T, Extent>;
#endif


			explicit constexpr TupleHash(int digestLength, const std::string &customize = {});

			constexpr void reset();
			constexpr TupleHash& finalize();  // after this, only `operator T()`, `reset()`, `toString()`, `toVector()` are available

			std::string toString() const;
			std::vector<Byte> toVector() const;
			template <typename T>
			operator T() const noexcept;

			constexpr TupleHash& nextData(Span<const Byte> inData);  // pass in next element in tuple
			constexpr TupleHash& nextData(const void *ptr, std::size_t length);
			template <std::size_t N>
			constexpr TupleHash& nextData(const Byte (&array)[N]);
			template <typename T, std::size_t N>
			TupleHash& nextData(const T (&array)[N]);
			template <typename T>
			TupleHash& nextData(Span<T> inSpan);

			friend constexpr bool operator==(const TupleHash &left, const TupleHash &right)
			{
				return (left.m_cshake == right.m_cshake);
			}
			friend constexpr bool operator!=(const TupleHash &left, const TupleHash &right)
			{
				return !(left == right);
			}

		private:
			constexpr void addDataImpl(Span<const Byte> data);

			Alg m_cshake;
			int m_digestLength = 0;
	};


	// helpers
	const auto rightEncode = [](const uint64_t value) -> Buffer<uint8_t, sizeof(value)>
	{
		const uint8_t n = (value == 0) ? 1 : static_cast<uint8_t>(std::lround((std::log2(value) / 8) + 0.5));

		Buffer<uint8_t, sizeof(value)> ret;
		for (int i = (n - 1); i >= 0; --i)
			ret.fill(ror<uint8_t>(value, (8 * i)));
		ret.fill(n);

		return ret;
	};


	//
	template <typename Alg>
	constexpr TupleHash<Alg>::TupleHash(const int digestLength, const std::string &customize)
		: m_cshake(digestLength, "TupleHash", customize)
		, m_digestLength(digestLength)
	{
		static_assert((CHAR_BIT == 8), "Sorry, we don't support exotic CPUs");
	}

	template <typename Alg>
	constexpr void TupleHash<Alg>::reset()
	{
		m_cshake.reset();
	}

	template <typename Alg>
	constexpr TupleHash<Alg>& TupleHash<Alg>::finalize()
	{
		const auto encoded = rightEncode(m_digestLength * 8);
		addDataImpl({encoded.data(), encoded.size()});
		m_cshake.finalize();
		return (*this);
	}

	template <typename Alg>
	std::string TupleHash<Alg>::toString() const
	{
		return m_cshake.toString();
	}

	template <typename Alg>
	std::vector<typename TupleHash<Alg>::Byte> TupleHash<Alg>::toVector() const
	{
		return m_cshake.toVector();
	}

	template <typename Alg>
	template <typename T>
	TupleHash<Alg>::operator T() const noexcept
	{
		static_assert(std::is_unsigned<T>::value, "");

		const auto digest = toVector();
		T ret = 0;
		for (int i = 0, iMax = static_cast<int>(std::min(sizeof(T), digest.size())); i < iMax; ++i)
		{
			ret <<= 8;
			ret |= digest[i];
		}
		return ret;
	}

	template <typename Alg>
	constexpr TupleHash<Alg>& TupleHash<Alg>::nextData(const Span<const Byte> inData)
	{
		const auto encoded = Chocobo1::Hash::CShake_NS::leftEncode(inData.size() * 8);
		addDataImpl({encoded.data(), encoded.size()});
		addDataImpl(inData);

		return (*this);
	}

	template <typename Alg>
	constexpr TupleHash<Alg>& TupleHash<Alg>::nextData(const void *ptr, const std::size_t length)
	{
		// Span::size_type = std::size_t
		return nextData({static_cast<const Byte*>(ptr), length});
	}

	template <typename Alg>
	template <std::size_t N>
	constexpr TupleHash<Alg>& TupleHash<Alg>::nextData(const Byte (&array)[N])
	{
		return nextData({array, N});
	}

	template <typename Alg>
	template <typename T, std::size_t N>
	TupleHash<Alg>& TupleHash<Alg>::nextData(const T (&array)[N])
	{
		return nextData({reinterpret_cast<const Byte*>(array), (sizeof(T) * N)});
	}

	template <typename Alg>
	template <typename T>
	TupleHash<Alg>& TupleHash<Alg>::nextData(const Span<T> inSpan)
	{
		return nextData({reinterpret_cast<const Byte*>(inSpan.data()), inSpan.size_bytes()});
	}

	template <typename Alg>
	constexpr void TupleHash<Alg>::addDataImpl(const Span<const Byte> data)
	{
		m_cshake.addData(data);
	}
}
}

	template <typename Base>
	struct TupleHashAlias : Base
	{
		using BaseType = Base;
		explicit constexpr TupleHashAlias(const int l, const std::string &c = {}) : Base(l, c) {}
		TupleHashAlias(const Base &other) : Base(other) {}
		TupleHashAlias(Base &&other) noexcept : Base(std::move(other)) {}
		TupleHashAlias& operator=(const Base &other) { if (this != &other) { Base::operator=(other); } return *this; }
		TupleHashAlias& operator=(Base &&other) noexcept { if (this != &other) { Base::operator=(std::move(other)); } return *this; }
	};
	using TupleHash_128 = TupleHashAlias<Hash::TupleHash_NS::TupleHash<CSHAKE_128>>;
	using TupleHash_256 = TupleHashAlias<Hash::TupleHash_NS::TupleHash<CSHAKE_256>>;
}

namespace std
{
	template <typename Alg>
	struct hash<Chocobo1::Hash::TupleHash_NS::TupleHash<Alg>>
	{
		size_t operator()(const Chocobo1::Hash::TupleHash_NS::TupleHash<Alg> &hash) const noexcept
		{
			return hash;
		}
	};

	template <>
	struct hash<Chocobo1::TupleHash_128>
	{
		size_t operator()(const Chocobo1::TupleHash_128 &hash) const noexcept
		{
			return hash;
		}
	};

	template <>
	struct hash<Chocobo1::TupleHash_256>
	{
		size_t operator()(const Chocobo1::TupleHash_256 &hash) const noexcept
		{
			return hash;
		}
	};
}

#endif  // CHOCOBO1_TUPLE_HASH_H
