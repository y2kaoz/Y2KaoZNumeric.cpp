#include "Y2KaoZ/Numeric/BigInt/BigInt.hpp"

namespace Y2KaoZ::Numeric::BigInt {

auto fromBuffer(std::span<const std::uint8_t> buffer, bool bigEndian) -> BigInt {
  BigInt bigInt;
  boost::multiprecision::import_bits(bigInt, std::begin(buffer), std::end(buffer), CHAR_BIT, bigEndian);
  return bigInt;
}

auto fromBuffer(std::span<const std::byte> buffer, bool bigEndian) -> BigInt {
  std::vector<std::uint8_t> uchars(buffer.size());
  std::memcpy(uchars.data(), buffer.data(), uchars.size());
  return fromBuffer(uchars, bigEndian);
}

auto fromBuffer(std::span<const boost::endian::little_uint8_buf_t> buffer) -> BigInt {
  std::vector<std::uint8_t> uchars(buffer.size());
  std::memcpy(uchars.data(), buffer.data(), uchars.size());
  return fromBuffer(uchars, false);
}

auto fromBuffer(std::span<const boost::endian::big_uint8_buf_t> buffer) -> BigInt {
  std::vector<std::uint8_t> uchars(buffer.size());
  std::memcpy(uchars.data(), buffer.data(), uchars.size());
  return fromBuffer(uchars, true);
}

auto uintBuffer(const BigInt& bigInt, bool bigEndian) -> std::vector<std::uint8_t> {
  std::vector<std::uint8_t> buffer;
  boost::multiprecision::export_bits(bigInt, std::back_inserter(buffer), CHAR_BIT, bigEndian);
  return buffer;
}

auto byteBuffer(const BigInt& bigInt, bool bigEndian) -> std::vector<std::byte> {
  std::vector<std::uint8_t> buffer;
  boost::multiprecision::export_bits(bigInt, std::back_inserter(buffer), CHAR_BIT, bigEndian);
  const auto bytes = std::as_bytes(std::span(buffer));
  return {bytes.begin(), bytes.end()};
}

auto littleEndianBuffer(const BigInt& bigInt) -> std::vector<boost::endian::little_uint8_buf_t> {
  std::vector<std::uint8_t> buffer;
  boost::multiprecision::export_bits(bigInt, std::back_inserter(buffer), CHAR_BIT, false);
  return {buffer.begin(), buffer.end()};
}

auto bigEndianBuffer(const BigInt& bigInt) -> std::vector<boost::endian::big_uint8_buf_t> {
  std::vector<std::uint8_t> buffer;
  boost::multiprecision::export_bits(bigInt, std::back_inserter(buffer), CHAR_BIT, true);
  return {buffer.begin(), buffer.end()};
}

} // namespace Y2KaoZ::Numeric::BigInt