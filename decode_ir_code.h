#include <stdint.h>

extern "C" {

struct A;
struct B;

typedef void* handle_a;
typedef void* handle_b;

// C++: A* result = reinterpret_cast<A>(handle);
// C: (A*)handle;

const uint16_t NEC_HEADER_MARK = 342;
const uint16_t NEC_HEADER_SPACE = 171;
const uint16_t NEC_MARK = 21;
const uint16_t NEC_HIGH_SPACE = 60;
const uint16_t NEC_LOW_SPACE = 21;

uint16_t generate_nec(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t NEC_SHORT_HEADER_MARK = 171;
const uint16_t NEC_SHORT_HEADER_SPACE = 171;
const uint16_t NEC_SHORT_MARK = 21;
const uint16_t NEC_SHORT_HIGH_SPACE = 60;
const uint16_t NEC_SHORT_LOW_SPACE = 21;

uint16_t generate_nec_short(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t RC5_SLICE = 32;
uint16_t generate_rc5(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t RC5_38_SLICE = 32; // should actually be 34 but this works better on Dish TV
uint16_t generate_rc5_38(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t RC5_57_SLICE = 52;
uint16_t generate_rc5_57(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t RC6_HEADER_MARK = 96;
const uint16_t RC6_HEADER_SPACE = 32;
const uint16_t RC6_SLICE = 16;

uint16_t generate_rc6(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t RCMM_HEADER_MARK = 15;
const uint16_t RCMM_HEADER_SPACE = 10;
const uint16_t RCMM_MARK = 6;
const uint16_t RCMM_SPACE0 = 10;
const uint16_t RCMM_SPACE1 = 16;
const uint16_t RCMM_SPACE2 = 22;
const uint16_t RCMM_SPACE3 = 28;

uint16_t generate_rcmm(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t XMP1_MARK = 7;
const uint16_t XMP1_PULSE_FIXED = 29;
const float XMP1_DATA_MULTI = 5.2f;
const uint16_t XMP1_GAP = 500;
const uint16_t XMP1_REPEAT_GAP = 3100;

uint16_t generate_xmp1(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t SONY_HEADER_MARK = 96;
const uint16_t SONY_HEADER_SPACE = 24;
const uint16_t SONY_HIGH_MARK = 48;
const uint16_t SONY_LOW_MARK = 24;
const uint16_t SONY_SPACE = 24;

uint16_t generate_sony(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t PANA_HEADER_MARK = 128;
const uint16_t PANA_HEADER_SPACE = 64;
const uint16_t PANA_MARK = 16;
const uint16_t PANA_HIGH_SPACE = 48;
const uint16_t PANA_LOW_SPACE = 16;

uint16_t generate_pana(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t JVC_HEADER_MARK = 320;
const uint16_t JVC_HEADER_SPACE = 160;
const uint16_t JVC_MARK = 20;
const uint16_t JVC_HIGH_SPACE = 60;
const uint16_t JVC_LOW_SPACE = 20;

uint16_t generate_jvc(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t SHARP_MARK = 12;
const uint16_t SHARP_HIGH_SPACE = 63;
const uint16_t SHARP_LOW_SPACE = 25;
const uint16_t SHARP_GAP_SPACE = 1520;

uint16_t generate_sharp(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t RCA38_HEADER_MARK = 152;
const uint16_t RCA38_HEADER_SPACE = 152;
const uint16_t RCA38_MARK = 19;
const uint16_t RCA38_HIGH_SPACE = 76;
const uint16_t RCA38_LOW_SPACE = 38;

uint16_t generate_rca_38(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t RCA57_HEADER_MARK = 228;
const uint16_t RCA57_HEADER_SPACE = 228;
const uint16_t RCA57_MARK = 29;
const uint16_t RCA57_HIGH_SPACE = 114;
const uint16_t RCA57_LOW_SPACE = 57;

uint16_t generate_rca_57(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t MITSUBISHI_HEADER_MARK = 304;
const uint16_t MITSUBISHI_HEADER_SPACE = 152;
const uint16_t MITSUBISHI_MARK = 20;
const uint16_t MITSUBISHI_HIGH_SPACE = 56;
const uint16_t MITSUBISHI_LOW_SPACE = 18;

uint16_t generate_mitsubishi(uint64_t code, uint32_t size, uint16_t *buffer);

const uint16_t KONKA_HEADER_MARK = 114;
const uint16_t KONKA_TRAILER_SPACE = 152;
const uint16_t KONKA_MARK = 19;
const uint16_t KONKA_HIGH_SPACE = 95;
const uint16_t KONKA_LOW_SPACE = 57;

uint16_t generate_konka(uint64_t code, uint32_t size, uint16_t *buffer);

uint16_t generate_test_seq(uint64_t code, uint32_t size, uint16_t *buffer);

uint16_t generate_test_pattern(uint64_t code, uint32_t size, uint16_t *buffer);

}

