/* Your first program*/

#define LED_BASE        0xFF200000
#define HEX3_HEX0_BASE  0xFF200020
#define HEX5_HEX4_BASE  0xFF200030
#define SW_BASE         0xFF200040

/* 7 SEGMENT DISPLAY INTERFACE
    -> Reference 2.10.2 in DE10 Datasheet

    Six 7SD's on DE10. Each SD is controlled from
    two registers. First 4 7SD's (called HEX0 - HEX3)
    are controlled via a register starting at address
    0xFF200020. It is a 32-bit, write-only data register.
    HEX3 is controlled by (MSB - 1) - 6 bits (i.e., the top 7 bits),
    HEX0 is controlled by LSB + 6 bits (i.e., the bottom 7 bits).
    The top-most bit in a specific hex control group is 
    unused (since it's only got 7 segments).
    
    Order is this:
      0
    5   1
      6    
    4   2
      3
*/

typedef unsigned int hex_digit_t;

hex_digit_t SEVEN_SEG_DISPLAY_PATTERN_LOOKUP[11] = {
    0b00111111 << 24, /* 0 */
    0b00000110 << 24, /* 1 */
    0b01011011 << 24, /* 2 */
    0b01001111 << 24, /* 3 */
    0b01100110 << 24, /* 4 */
    0b01101101 << 24, /* 5 */
    0b01111101 << 24, /* 6 */
    0b00000111 << 24, /* 7 */
    0b01111111 << 24, /* 8 */
    0b01101111 << 24, /* 9 */
    0b01000000 << 24  /* - */
};

// there are two hex registers. One is a 32 bit register (0xff200020) where each byte
// maps to one hex, so four 7SDs total (see preamble/docs). The other two
// 7SDs are controlled with the bottom 16 bits of the next register (0xff200030).
volatile unsigned int* const hex_register_one = (unsigned int*)(HEX3_HEX0_BASE);
volatile unsigned int* const hex_register_two = (unsigned int*)(HEX5_HEX4_BASE);

// Input is a 32 bit value in decimal
void display_hex(int value)
{
    // The max value displayable is 999,999
    if (value > 999999) return;
    
    int value_cpy = value;
    unsigned int is_negative = 0;
    if (value < 0) {
        is_negative = 1;
        value_cpy = 0 - value; // take the abs value 
    } 
    
    // I don't know if this is the fastest way to do this
    unsigned char idx = 0;
    unsigned int digits[6];
    hex_digit_t* this_digit_hex_pattern;
    unsigned int this_digit;
    while (value_cpy > 0) {
        this_digit = value_cpy % 10;
        this_digit_hex_pattern = &SEVEN_SEG_DISPLAY_PATTERN_LOOKUP[this_digit];
        
        digits[idx] = *this_digit_hex_pattern;
        
        value_cpy /= 10; // floor of the number div 10
        idx += 1;
    }

    // check to make sure negative sign won't go out of bounds
    if (idx - 7 > 0) {
        return;
    }

    // idx -1 is last nonzero digit
    for (int i = 5; i > idx - 1; i--) {
        digits[i] = SEVEN_SEG_DISPLAY_PATTERN_LOOKUP[0];
    }

    // display the digits now
    // 
    // these all have to be 32 bit integers because you can't do
    // bitwise operations with varying-sized types 
    // 
    // the number we are bitshifting is the actual data value in the hex
    // register. Addressing is of no concern here because we're bitshifting
    // it by 8 bits (1 byte) every time anyways. No need to change the pointer;
    // you're just changing the value itself
    // 
    // 0b01011011 >> 8 = 0b( 01011011 00000000 )
    //                |= 0b( 00000000 01100101 )
    //                 =     01011011 01100101

    *hex_register_two = 0;
    *hex_register_one = 0; // initialize to 0b000[...]

    // register length - 1. Don't bitshift after last entry
    for (idx = 0; idx < 3; idx++) {
        *hex_register_one |= digits[idx];
        *hex_register_one >>= 8;
    }
    *hex_register_one |= digits[3];

    return;
}

int read_switches(void)
{
}
	
int main(void) {
    display_hex(9876);
}
