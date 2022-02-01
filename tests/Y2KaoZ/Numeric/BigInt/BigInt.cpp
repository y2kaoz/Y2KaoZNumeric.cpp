#include "Y2KaoZ/Numeric/BigInt/BigInt.hpp"
#include "catch2/catch_all.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("deserialize BigInt") { // NOLINT

  using Y2KaoZ::Numeric::BigInt::fromBuffer;

  const std::uint16_t value{0xFEED};

  SECTION("using std::span<const std::uint8_t>") {
    const std::vector<std::uint8_t> bBuffer = {0xFE, 0xED};
    const std::vector<std::uint8_t> lBuffer = {0xED, 0xFE};

    CHECK(fromBuffer(bBuffer, true) == value);
    CHECK(fromBuffer(lBuffer, true) != value);
    CHECK(fromBuffer(lBuffer, false) == value);
    CHECK(fromBuffer(bBuffer, false) != value);
  }

  SECTION("using std::span<const std::byte>") {
    const std::vector<std::byte> bBuffer = {std::byte{0xFE}, std::byte{0xED}};
    const std::vector<std::byte> lBuffer = {std::byte{0xED}, std::byte{0xFE}};

    CHECK(fromBuffer(bBuffer, true) == value);
    CHECK(fromBuffer(lBuffer, true) != value);
    CHECK(fromBuffer(lBuffer, false) == value);
    CHECK(fromBuffer(bBuffer, false) != value);
  }

  SECTION("using std::span<const boost::endian::little_uint8_buf_t>") {
    using little = boost::endian::little_uint8_buf_t;
    std::vector<little> lBuffer{little{0xED}, little{0xFE}}; // NOLINT
    std::vector<little> bBuffer{little{0xFE}, little{0xED}}; // NOLINT
    CHECK(fromBuffer(lBuffer) == value);
    CHECK(fromBuffer(bBuffer) != value);
  }

  SECTION("using std::span<const boost::endian::little_uint8_buf_t>") {
    using big = boost::endian::big_uint8_buf_t;
    std::array<big, 2> lBuffer{{big{0xED}, big{0xFE}}}; // NOLINT
    std::array<big, 2> bBuffer{{big{0xFE}, big{0xED}}}; // NOLINT
    CHECK(fromBuffer(bBuffer) == value);
    CHECK(fromBuffer(lBuffer) != value);
  }
}

TEST_CASE("serialize BigInt") { // NOLINT

  using Y2KaoZ::Numeric::BigInt::BigInt;

  const BigInt value{0xFEED};

  SECTION("to std::vector<std::uint8_t>") {
    using Y2KaoZ::Numeric::BigInt::uintBuffer;
    const auto lbuffer = uintBuffer(value, false);
    const auto bbuffer = uintBuffer(value, true);

    CHECK(lbuffer == std::vector<std::uint8_t>{0xED, 0xFE});
    CHECK(bbuffer != std::vector<std::uint8_t>{0xED, 0xFE});
    CHECK(bbuffer == std::vector<std::uint8_t>{0xFE, 0xED});
    CHECK(lbuffer != std::vector<std::uint8_t>{0xFE, 0xED});
  }

  SECTION("to std::vector<std::byte>") {
    using Y2KaoZ::Numeric::BigInt::byteBuffer;
    const auto lbuffer = byteBuffer(value, false);
    const auto bbuffer = byteBuffer(value, true);

    CHECK(lbuffer == std::vector<std::byte>{std::byte{0xED}, std::byte{0xFE}});
    CHECK(bbuffer != std::vector<std::byte>{std::byte{0xED}, std::byte{0xFE}});
    CHECK(bbuffer == std::vector<std::byte>{std::byte{0xFE}, std::byte{0xED}});
    CHECK(lbuffer != std::vector<std::byte>{std::byte{0xFE}, std::byte{0xED}});
  }

  SECTION("to std::vector<boost::endian::little_uint8_buf_t>") {
    using Y2KaoZ::Numeric::BigInt::littleEndianBuffer;
    const auto lbuffer = littleEndianBuffer(value);
    CHECK(lbuffer[0].value() == 0xED);
    CHECK(lbuffer[0].value() != 0xFE);
    CHECK(lbuffer[1].value() == 0xFE);
    CHECK(lbuffer[1].value() != 0xED);
  }

  SECTION("to std::vector<boost::endian::big_uint8_buf_t>") {
    using Y2KaoZ::Numeric::BigInt::bigEndianBuffer;
    const auto bbuffer = bigEndianBuffer(value);
    CHECK(bbuffer[0].value() != 0xED);
    CHECK(bbuffer[0].value() == 0xFE);
    CHECK(bbuffer[1].value() != 0xFE);
    CHECK(bbuffer[1].value() == 0xED);
  }

  SECTION("to a small std::array<std::uint8_t, SIZE>") {
    using Y2KaoZ::Numeric::BigInt::uintBuffer;
    CHECK_THROWS(uintBuffer<1>(value, false));
    CHECK_THROWS(uintBuffer<1>(value, true));
  }

  SECTION("to an exact std::array<std::uint8_t, SIZE>") {
    using Y2KaoZ::Numeric::BigInt::uintBuffer;
    CHECK_NOTHROW(uintBuffer<2>(value, false));
    CHECK_NOTHROW(uintBuffer<2>(value, true));

    const auto lbuffer = uintBuffer<2>(value, false);
    const auto bbuffer = uintBuffer<2>(value, true);

    CHECK(lbuffer == std::array<std::uint8_t, 2>{{0xED, 0xFE}});
    CHECK(bbuffer != std::array<std::uint8_t, 2>{{0xED, 0xFE}});
    CHECK(bbuffer == std::array<std::uint8_t, 2>{{0xFE, 0xED}});
    CHECK(lbuffer != std::array<std::uint8_t, 2>{{0xFE, 0xED}});
  }

  SECTION("to a bigger std::array<std::uint8_t, SIZE>") {
    using Y2KaoZ::Numeric::BigInt::fromBuffer;
    using Y2KaoZ::Numeric::BigInt::uintBuffer;
    CHECK_NOTHROW(uintBuffer<4>(value, false));
    CHECK_NOTHROW(uintBuffer<4>(value, true));

    const auto lbuffer = uintBuffer<4>(value, false);
    const auto bbuffer = uintBuffer<4>(value, true);

    CHECK(lbuffer == std::array<std::uint8_t, 4>{{0xED, 0xFE, 0x00, 0x00}});
    CHECK(bbuffer != std::array<std::uint8_t, 4>{{0xED, 0xFE, 0x00, 0x00}});
    CHECK(bbuffer == std::array<std::uint8_t, 4>{{0x00, 0x00, 0xFE, 0xED}});
    CHECK(lbuffer != std::array<std::uint8_t, 4>{{0x00, 0x00, 0xFE, 0xED}});

    CHECK(fromBuffer(lbuffer, false) == value);
    CHECK(fromBuffer(bbuffer, false) != value);
    CHECK(fromBuffer(lbuffer, true) != value);
    CHECK(fromBuffer(bbuffer, true) == value);
  }

  SECTION("to a bigger std::array<std::byte, SIZE>") {
    using Y2KaoZ::Numeric::BigInt::byteBuffer;
    using Y2KaoZ::Numeric::BigInt::fromBuffer;
    CHECK_NOTHROW(byteBuffer<4>(value, false));
    CHECK_NOTHROW(byteBuffer<4>(value, true));

    const auto lbuffer = byteBuffer<4>(value, false);
    const auto bbuffer = byteBuffer<4>(value, true);

    CHECK(fromBuffer(lbuffer, false) == value);
    CHECK(fromBuffer(bbuffer, false) != value);
    CHECK(fromBuffer(lbuffer, true) != value);
    CHECK(fromBuffer(bbuffer, true) == value);
  }

  SECTION("to a bigger std::array<boost::endian::little_uint8_buf_t, SIZE>") {
    using Y2KaoZ::Numeric::BigInt::fromBuffer;
    using Y2KaoZ::Numeric::BigInt::littleEndianBuffer;
    CHECK_NOTHROW(littleEndianBuffer<4>(value));
    CHECK(fromBuffer(littleEndianBuffer<4>(value)) == value);
  }

  SECTION("to a bigger std::array<boost::endian::big_uint8_buf_t, SIZE>") {
    using Y2KaoZ::Numeric::BigInt::bigEndianBuffer;
    using Y2KaoZ::Numeric::BigInt::fromBuffer;
    CHECK_NOTHROW(bigEndianBuffer<4>(value));
    CHECK(fromBuffer(bigEndianBuffer<4>(value)) == value);
  }
}

TEST_CASE("cpp_int power modulo's fix") { // NOLINT
  using Y2KaoZ::Numeric::BigInt::BigInt;
  BigInt base{
    "-60069516222067934465050534050924735913259123336604003695277708341824332895413979944668873920813978546728706494838542031539374089411576323557683032079456300479576399686088457834323998789559447280306593096633556722918700757880790576568543160534527724248702686289473589204691773279466673778290317667494679021095759596587143108565755917479750768759519224411507201300590256025341023717379667034891896621559345748879219694808586783746072343055239350628677886152249850829448959449884027507137715617906830580188376921330347491783964930384160916882168844833991645809121747349916283613126899731249980617076083650399040055004037242145087844202129219022561806749382444723981250548071098932686072865117465590944246091868534389708960372902345012028512996301150892770425867448873457573540209290963648133766450038070701366521974251173331552053367466421294567346255207594293174647572109245024389844295595423148975431938500850550834122854589221010198699007954971109000609845414067610786404346462923082635417069420150663"};
  BigInt exp{
    "146462318780050704398491824679577877249637357233997958820955798606849034089055435560888738746472653803474347785215052176587544928236521681250726410243117146638424272590451021577470339456543664816801048455555325077949491666342480646917362501726371391429930610071051177605862191601117139467049042192421315075925"};
  BigInt mod{
    "5809605995369958062791915965639201402176612226902900533702900882779736177890990861472094774477339581147373410185646378328043729800750470098210924487866935059164371588168047540943981644516632755067501626434556398193186628990071248660819361205119793693985433297036118232914410171876807536457391277857011849897410207519105333355801121109356897459426271845471397952675959440793493071628394122780510124618488232602464649876850458861245784240929258426287699705312584509625419513463605155428017165714465363094021609290561084025893662561222573202082865797821865270991145082200656978177192827024538990239969175546190770645685893438011714430426409338676314743571154537142031573004276428701433036381801705308659830751190352946025482059931306571004727362479688415574702596946457770284148435989129632853918392117997472632693078113129886487399347796982772784615865232621289656944284216824611318709764535152507354116344703769998514148343807"};
  BigInt expected{
    "1230568615184131985239346694446825655892733339973705775794155717290665406519709910407387531042376274634783342677355212818101752559959454003565957987129773506359982415765799964505534198533317147147330259998210157489219128947077927164476066764792878755295287228542190650053716328463274779372118557925988198416352338648543137326490077853579150463484865391556678099929526143587731521332343634512569891817217753866541107231323752824752313183429346607264168653584884607177140562811376873489718089411252979981686946488318076743666882294457424784508392043142317072588165582750606934742360106685332345292776496954791945874043489368333919203276279792510478642311867043688320756550623809890002329790942184110903137973601678472100031271022409063644536472615014574455126844337240082531858482475302880382827055687831445880729409121007713054522708302456617762637870271318479897319132492452505220079651903923918555883332563223829904474269679"};
  CHECK(boost::multiprecision::powm(base, exp, mod) != expected);
  CHECK(Y2KaoZ::Numeric::BigInt::powm(base, exp, mod) == expected);
}