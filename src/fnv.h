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

#ifndef CHOCOBO1_FNV_H
#define CHOCOBO1_FNV_H

#include <array>
#include <cassert>
#include <climits>
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
	// FNV32_0();
	// FNV32_1();
	// FNV32_1a();
	// FNV64_0();
	// FNV64_1();
	// FNV64_1a();
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


namespace FNVHASH_NS
{
	template <typename DigestType, int Variant>
	class FNVHash
	{
		// https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-17

		public:
			using Byte = uint8_t;
			using ResultArrayType = std::array<Byte, sizeof(DigestType)>;

#if (USE_STD_SPAN_CHOCOBO1_HASH == 1)
			template <typename T, std::size_t Extent = std::dynamic_extent>
			using Span = std::span<T, Extent>;
#else
			template <typename T, std::size_t Extent = gsl::dynamic_extent>
			using Span = gsl::span<T, Extent>;
#endif


			constexpr FNVHash();

			constexpr void reset();
			CONSTEXPR_CPP17_CHOCOBO1_HASH FNVHash& finalize();  // after this, only `toArray()`, `toString()`, `toVector()`, `reset()` are available

			std::string toString() const;
			std::vector<Byte> toVector() const;
			CONSTEXPR_CPP17_CHOCOBO1_HASH ResultArrayType toArray() const;

			constexpr FNVHash& addData(const Span<const Byte> inData);
			constexpr FNVHash& addData(const void *ptr, const std::size_t length);
			template <std::size_t N>
			constexpr FNVHash& addData(const Byte (&array)[N]);
			template <typename T, std::size_t N>
			FNVHash& addData(const T (&array)[N]);
			template <typename T>
			FNVHash& addData(const Span<T> inSpan);

		private:
			constexpr void addDataImpl(const Span<const Byte> data);

			DigestType m_hash = {0};
	};


	// helpers
	template <typename R, typename T>
	constexpr R ror(const T x, const unsigned int s)
	{
		static_assert(std::is_unsigned<R>::value, "");
		static_assert(std::is_unsigned<T>::value, "");
		return static_cast<R>(x >> s);
	}


	//
	template <typename DigestType, int Variant>
	constexpr FNVHash<DigestType, Variant>::FNVHash()
	{
		static_assert((CHAR_BIT == 8), "Sorry, we don't support exotic CPUs");

		reset();
	}

	template <typename DigestType, int Variant>
	constexpr void FNVHash<DigestType, Variant>::reset()
	{
		if (Variant >= 1)
		{
			switch (sizeof(DigestType))
			{
				case 4:
					m_hash = 0x811c9dc5;
					break;
				case 8:
					m_hash = static_cast<DigestType>(0xcbf29ce484222325);
					break;
				/*
				case 16:
					m_hash = static_cast<DigestType>(0x6c62272e07bb014262b821756295c58d);
					break;
				case 32:
					m_hash = static_cast<DigestType>(0xdd268dbcaac550362d98c384c4e576ccc8b1536847b6bbb31023b4c8caee0535);
					break;
				case 64:
					m_hash = static_cast<DigestType>(0xb86db0b1171f4416dca1e50f309990acac87d059c90000000000000000000d21e948f68a34c192f62ea79bc942dbe7ce182036415f56e34bac982aac4afe9fd9);
					break;
				case 128:
					m_hash = static_cast<DigestType>(0x0000000000000000005f7a76758ecc4d32e56d5a591028b74b29fc4223fdada16c3bf34eda3674da9a21d9000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004c6d7eb6e73802734510a555f256cc005ae556bde8cc9c6a93b21aff4b16c71ee90b3);
					break;
				*/
			}
		}
	}

	template <typename DigestType, int Variant>
	CONSTEXPR_CPP17_CHOCOBO1_HASH FNVHash<DigestType, Variant>& FNVHash<DigestType, Variant>::finalize()
	{
		return (*this);
	}

	template <typename DigestType, int Variant>
	std::string FNVHash<DigestType, Variant>::toString() const
	{
		const auto a = toArray();
		std::string ret;
		ret.resize(2 * a.size());

		auto retPtr = &ret.front();
		for (const auto c : a)
		{
			const Byte upper = ror<Byte>(c, 4);
			*(retPtr++) = static_cast<char>((upper < 10) ? (upper + '0') : (upper - 10 + 'a'));

			const Byte lower = c & 0xf;
			*(retPtr++) = static_cast<char>((lower < 10) ? (lower + '0') : (lower - 10 + 'a'));
		}

		return ret;
	}

	template <typename DigestType, int Variant>
	std::vector<typename FNVHash<DigestType, Variant>::Byte> FNVHash<DigestType, Variant>::toVector() const
	{
		const auto a = toArray();
		return {a.begin(), a.end()};
	}

	template <typename DigestType, int Variant>
	CONSTEXPR_CPP17_CHOCOBO1_HASH typename FNVHash<DigestType, Variant>::ResultArrayType FNVHash<DigestType, Variant>::toArray() const
	{
		ResultArrayType ret {};
		auto retPtr = ret.data();
		for (int j = (ret.size() - 1); j >= 0; --j)
			*(retPtr++) = ror<Byte>(m_hash, (j * 8));

		return ret;
	}

	template <typename DigestType, int Variant>
	constexpr FNVHash<DigestType, Variant>& FNVHash<DigestType, Variant>::addData(const Span<const Byte> inData)
	{
		addDataImpl(inData);
		return (*this);
	}

	template <typename DigestType, int Variant>
	constexpr FNVHash<DigestType, Variant>& FNVHash<DigestType, Variant>::addData(const void *ptr, const std::size_t length)
	{
		// Span::size_type = std::size_t
		return addData({static_cast<const Byte*>(ptr), length});
	}

	template <typename DigestType, int Variant>
	template <std::size_t N>
	constexpr FNVHash<DigestType, Variant>& FNVHash<DigestType, Variant>::addData(const Byte (&array)[N])
	{
		return addData({array, N});
	}

	template <typename DigestType, int Variant>
	template <typename T, std::size_t N>
	FNVHash<DigestType, Variant>& FNVHash<DigestType, Variant>::addData(const T (&array)[N])
	{
		return addData({reinterpret_cast<const Byte*>(array), (sizeof(T) * N)});
	}

	template <typename DigestType, int Variant>
	template <typename T>
	FNVHash<DigestType, Variant>& FNVHash<DigestType, Variant>::addData(const Span<T> inSpan)
	{
		return addData({reinterpret_cast<const Byte*>(inSpan.data()), inSpan.size_bytes()});
	}

	template <typename DigestType, int Variant>
	constexpr void FNVHash<DigestType, Variant>::addDataImpl(const Span<const Byte> data)
	{
		const auto multiplyPrime = [this]()
		{
			switch (sizeof(DigestType))
			{
				case 4:
					m_hash *= 0x01000193;
					break;
				case 8:
					m_hash *= static_cast<DigestType>(0x00000100000001b3);
					break;
				/*
				case 16:
					m_hash *= static_cast<DigestType>(0x0000000001000000000000000000013b);
					break;
				case 32:
					m_hash *= static_cast<DigestType>(0x0000000000000000000001000000000000000000000000000000000000000163);
					break;
				case 64:
					m_hash *= static_cast<DigestType>(0x00000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000157);
					break;
				case 128:
					m_hash *= static_cast<DigestType>(0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000018d);
					break;
				*/
			}
		};

		for (const Byte dataPart : data)
		{
			if (Variant <= 1)
			{
				multiplyPrime();
				m_hash ^= dataPart;
			}
			if (Variant == 2)
			{
				m_hash ^= dataPart;
				multiplyPrime();
			}
		}
	}
}
}
	using FNV32_0 = Hash::FNVHASH_NS::FNVHash<uint32_t, 0>;
	using FNV32_1 = Hash::FNVHASH_NS::FNVHash<uint32_t, 1>;
	using FNV32_1a = Hash::FNVHASH_NS::FNVHash<uint32_t, 2>;
	using FNV64_0 = Hash::FNVHASH_NS::FNVHash<uint64_t, 0>;
	using FNV64_1 = Hash::FNVHASH_NS::FNVHash<uint64_t, 1>;
	using FNV64_1a = Hash::FNVHASH_NS::FNVHash<uint64_t, 2>;
}

#endif  // CHOCOBO1_FNV_H
