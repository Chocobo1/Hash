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

#ifndef CHOCOBO1_CSHAKE_H
#define CHOCOBO1_CSHAKE_H

#include "sha3.h"

#include "gsl/span"

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>


namespace Chocobo1
{
	// Use these!!
	// CSHAKE_128(const unsigned int digestLengthInBytes, const std::string &name = {}, const std::string &customize = {});
	// CSHAKE_256(const unsigned int digestLengthInBytes, const std::string &name = {}, const std::string &customize = {});
}


namespace Chocobo1
{
// users should ignore things in this namespace
namespace Hash
{
namespace CShake_NS
{
	template <typename S, typename K, unsigned int P>
	class CShake
	{
		// https://doi.org/10.6028/NIST.SP.800-185

		public:
			using Byte = uint8_t;

			template <typename T>
			using Span = gsl::span<T>;


			explicit CShake(const unsigned int digestLength, const std::string &name = {}, const std::string &customize = {});

			constexpr void reset();
			constexpr CShake& finalize();  // after this, only `toString()`, `toVector()`, `reset()` are available

			std::string toString() const;
			std::vector<Byte> toVector() const;

			constexpr CShake& addData(const Span<const Byte> inData);
			constexpr CShake& addData(const void *ptr, const long int length);

		private:
			constexpr void addDataImpl(const Span<const Byte> data);

			const bool m_customized = false;

			std::unique_ptr<S> m_shake;
			std::unique_ptr<K> m_keccak;
	};


	// helpers
	template <typename R, typename T, typename std::enable_if<(std::is_unsigned<T>::value), int>::type = 0>
	constexpr R ror(const T n, const unsigned int r)
	{
		const R mask = -1;
		return ((n >> r) & mask);
	}

	inline std::vector<uint8_t> leftEncode(const uint64_t value)
	{
		const uint8_t n = (value == 0) ? 1 : std::lround((std::log2(value) / 8) + 0.5);
		std::vector<uint8_t> ret = {n};
		ret.reserve(n + 1);

		for (int i = (n - 1); i >= 0; --i)
			ret.emplace_back(ror<uint8_t>(value, (8 * i)));

		return ret;
	}


	//
	template <typename S, typename K, unsigned int P>
	CShake<S, K, P>::CShake(const unsigned int digestLength, const std::string &name, const std::string &customize)
		: m_customized(!(name.empty() && customize.empty()))
	{
		if (!m_customized)
		{
			m_shake = std::unique_ptr<S>(new S(digestLength));
			return;
		}
		else
		{
			m_keccak = std::unique_ptr<K>(new K(digestLength));
		}

		const auto processString = [this](const std::string &str, size_t &length) -> void
		{
			const auto encodedStr = leftEncode(str.size() * 8);
			addData(encodedStr);
			length += encodedStr.size();
			addData(str.data(), str.size());
			length += str.size();
		};
		const auto bytepadCount = [](const size_t length, const size_t target) -> size_t
		{
			return ((target - (length % target)) % target);
		};

		size_t length = 0;

		const auto encodedW = leftEncode(P);
		addData(encodedW);
		length += encodedW.size();

		processString(name, length);
		processString(customize, length);

		const std::vector<Byte> zeros(bytepadCount(length, P), 0);
		addData(zeros);
	}

	template <typename S, typename K, unsigned int P>
	constexpr void CShake<S, K, P>::reset()
	{
		if (!m_customized)
			m_shake->reset();
		else
			m_keccak->reset();
	}

	template <typename S, typename K, unsigned int P>
	constexpr CShake<S, K, P>& CShake<S, K, P>::finalize()
	{
		if (!m_customized)
			m_shake->finalize();
		else
			m_keccak->finalize();
		return (*this);
	}

	template <typename S, typename K, unsigned int P>
	std::string CShake<S, K, P>::toString() const
	{
		if (!m_customized)
			return m_shake->toString();
		else
			return m_keccak->toString();
	}

	template <typename S, typename K, unsigned int P>
	std::vector<typename CShake<S, K, P>::Byte> CShake<S, K, P>::toVector() const
	{
		if (!m_customized)
			return m_shake->toVector();
		else
			return m_keccak->toVector();
	}

	template <typename S, typename K, unsigned int P>
	constexpr CShake<S, K, P>& CShake<S, K, P>::addData(const Span<const Byte> inData)
	{
		addDataImpl(inData);
		return (*this);
	}

	template <typename S, typename K, unsigned int P>
	constexpr CShake<S, K, P>& CShake<S, K, P>::addData(const void *ptr, const long int length)
	{
		// gsl::span::index_type = long int
		return addData({reinterpret_cast<const Byte*>(ptr), length});
	}

	template <typename S, typename K, unsigned int P>
	constexpr void CShake<S, K, P>::addDataImpl(const Span<const Byte> data)
	{
		if (!m_customized)
			m_shake->addData(data);
		else
			m_keccak->addData(data);
	}
}
}
	struct CSHAKE_128 : Hash::CShake_NS::CShake<SHAKE_128, Hash::SHA3_NS::Keccak<(1344 / 8), 0x04>, (1344 / 8)> { explicit CSHAKE_128(const unsigned int l, const std::string &n = {}, const std::string &c = {}) : Hash::CShake_NS::CShake<SHAKE_128, Hash::SHA3_NS::Keccak<(1344 / 8), 0x04>, (1344 / 8)>(l, n, c) {} };
	struct CSHAKE_256 : Hash::CShake_NS::CShake<SHAKE_256, Hash::SHA3_NS::Keccak<(1088 / 8), 0x04>, (1088 / 8)> { explicit CSHAKE_256(const unsigned int l, const std::string &n = {}, const std::string &c = {}) : Hash::CShake_NS::CShake<SHAKE_256, Hash::SHA3_NS::Keccak<(1088 / 8), 0x04>, (1088 / 8)>(l, n, c) {} };
}

#endif  // CHOCOBO1_CSHAKE_H
