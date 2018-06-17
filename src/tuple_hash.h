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

#ifndef CHOCOBO1_TUPLE_HASH_H
#define CHOCOBO1_TUPLE_HASH_H

#include "cshake.h"

#include "gsl/span"

#include <cstdint>
#include <string>
#include <vector>


namespace Chocobo1
{
	// Use these!!
	// TupleHash_128(const unsigned int digestLengthInBytes, const std::string &customize = {});
	// TupleHash_256(const unsigned int digestLengthInBytes, const std::string &customize = {});
}


namespace Chocobo1
{
// users should ignore things in this namespace
namespace Hash
{
namespace TupleHash_NS
{
	template <typename Alg>
	class TupleHash
	{
		// https://doi.org/10.6028/NIST.SP.800-185

		public:
			using Byte = uint8_t;

			template <typename T>
			using Span = gsl::span<T>;


			explicit constexpr TupleHash(const unsigned int digestLength, const std::string &customize = {});

			constexpr void reset();
			constexpr TupleHash& finalize();  // after this, only `toString()`, `toVector()`, `reset()` are available

			std::string toString() const;
			std::vector<Byte> toVector() const;

			TupleHash& nextData(const Span<const Byte> inData);  // pass in next element in tuple
			constexpr TupleHash& nextData(const void *ptr, const long int length);
			template <typename T, std::size_t N>
			TupleHash& nextData(const T (&array)[N]);
			template <typename T>
			TupleHash& nextData(const Span<T> inSpan);

		private:
			constexpr void addDataImpl(const Span<const Byte> data);

			Alg m_cshake;
			const unsigned int m_digestLength = 0;
	};


	// helpers
	inline std::vector<uint8_t> rightEncode(const uint64_t value)
	{
		std::vector<uint8_t> ret = Chocobo1::Hash::CShake_NS::leftEncode(value);
		const uint8_t first = ret.front();
		ret.erase(ret.begin());
		ret.emplace_back(first);
		return ret;
	}


	//
	template <typename Alg>
	constexpr TupleHash<Alg>::TupleHash(const unsigned int digestLength, const std::string &customize)
		: m_cshake(digestLength, "TupleHash", customize)
		, m_digestLength(digestLength)
	{
	}

	template <typename Alg>
	constexpr void TupleHash<Alg>::reset()
	{
		m_cshake.reset();
	}

	template <typename Alg>
	constexpr TupleHash<Alg>& TupleHash<Alg>::finalize()
	{
		addDataImpl(rightEncode(m_digestLength * 8));
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
	TupleHash<Alg>& TupleHash<Alg>::nextData(const Span<const Byte> inData)
	{
		const std::vector<uint8_t> encoded = Chocobo1::Hash::CShake_NS::leftEncode(inData.size() * 8);
		addDataImpl(encoded);
		addDataImpl(inData);

		return (*this);
	}

	template <typename Alg>
	constexpr TupleHash<Alg>& TupleHash<Alg>::nextData(const void *ptr, const long int length)
	{
		// gsl::span::index_type = long int
		return nextData({static_cast<const Byte*>(ptr), length});
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
	struct TupleHash_128 : Hash::TupleHash_NS::TupleHash<CSHAKE_128> { explicit TupleHash_128(const unsigned int l, const std::string &c = {}) : Hash::TupleHash_NS::TupleHash<CSHAKE_128>(l, c) {} };
	struct TupleHash_256 : Hash::TupleHash_NS::TupleHash<CSHAKE_256> { explicit TupleHash_256(const unsigned int l, const std::string &c = {}) : Hash::TupleHash_NS::TupleHash<CSHAKE_256>(l, c) {} };
}

#endif  // CHOCOBO1_TUPLE_HASH_H
