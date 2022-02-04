#pragma once

#include "Y2KaoZ/Numeric/Visibility.hpp"
#include <boost/endian/buffers.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <span>

namespace Y2KaoZ::Numeric::BigInt {

using BigInt = boost::multiprecision::cpp_int;
using LittleUint8 = boost::endian::little_uint8_buf_t;
using BigUint8 = boost::endian::big_uint8_buf_t;

static_assert(sizeof(std::uint8_t) == sizeof(std::byte));

static_assert(CHAR_BIT == 8); // NOLINT

[[nodiscard]] Y2KAOZNUMERIC_EXPORT auto fromBuffer(std::span<const std::uint8_t> buffer, bool bigEndian = false)
  -> BigInt;
[[nodiscard]] Y2KAOZNUMERIC_EXPORT auto fromBuffer(std::span<const std::byte> buffer, bool bigEndian = false) -> BigInt;
[[nodiscard]] Y2KAOZNUMERIC_EXPORT auto fromBuffer(std::span<const LittleUint8> buffer) -> BigInt;
[[nodiscard]] Y2KAOZNUMERIC_EXPORT auto fromBuffer(std::span<const BigUint8> buffer) -> BigInt;

[[nodiscard]] Y2KAOZNUMERIC_EXPORT auto uintBuffer(const BigInt& bigInt, bool bigEndian = false)
  -> std::vector<std::uint8_t>;
[[nodiscard]] Y2KAOZNUMERIC_EXPORT auto byteBuffer(const BigInt& bigInt, bool bigEndian = false)
  -> std::vector<std::byte>;
[[nodiscard]] Y2KAOZNUMERIC_EXPORT auto littleEndianBuffer(const BigInt& bigInt) -> std::vector<LittleUint8>;
[[nodiscard]] Y2KAOZNUMERIC_EXPORT auto bigEndianBuffer(const BigInt& bigInt) -> std::vector<BigUint8>;

template <std::size_t SIZE>
inline auto uintBuffer(const BigInt& bigInt, bool bigEndian = false) -> std::array<std::uint8_t, SIZE> {
  std::vector<std::uint8_t> vector = uintBuffer(bigInt, bigEndian);
  if (SIZE < vector.size()) {
    throw std::overflow_error("The number is too big for the array.");
  }
  std::array<std::uint8_t, SIZE> array{};
  std::size_t index = bigEndian ? SIZE - vector.size() : 0;
  std::memcpy(&array[index], vector.data(), vector.size());
  return array;
}

template <std::size_t SIZE>
inline auto byteBuffer(const BigInt& bigInt, bool bigEndian = false) -> std::array<std::byte, SIZE> {
  std::vector<std::byte> vector = byteBuffer(bigInt, bigEndian);
  if (SIZE < vector.size()) {
    throw std::overflow_error("The number is too big for the array.");
  }
  std::array<std::byte, SIZE> array{};
  std::size_t index = bigEndian ? SIZE - vector.size() : 0;
  std::memcpy(&array[index], vector.data(), vector.size());
  return array;
}

template <std::size_t SIZE>
auto littleEndianBuffer(const BigInt& bigInt) -> std::array<boost::endian::little_uint8_buf_t, SIZE> {
  return std::bit_cast<std::array<boost::endian::little_uint8_buf_t, SIZE>>(uintBuffer<SIZE>(bigInt, false));
}

template <std::size_t SIZE>
auto bigEndianBuffer(const BigInt& bigInt) -> std::array<boost::endian::big_uint8_buf_t, SIZE> {
  return std::bit_cast<std::array<boost::endian::big_uint8_buf_t, SIZE>>(uintBuffer<SIZE>(bigInt, true));
}

// https://github.com/boostorg/multiprecision/issues/39
// https://stackoverflow.com/questions/48233368/boostmultiprecisionpowm-differs-from-bigintegermodpow-in-java
// There is a documented difference of how signed modulo is implemented with cpp_int.
// As a simple fix, just add the divisor if the return value is negative:
template <class T>
auto powm(const T& a, const T& p, const T& c) -> T {
  if constexpr (std::is_same_v<BigInt, boost::multiprecision::cpp_int>) {
    using std::abs; // summon the god of ADL
    T r = boost::multiprecision::powm(a, p, c);
    return r < 0 ? r + abs(c) : r;
  }
  return boost::multiprecision::powm(a, p, c);
}

} // namespace Y2KaoZ::Numeric::BigInt