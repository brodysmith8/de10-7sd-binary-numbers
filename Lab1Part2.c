#define LED_BASE        0xFF200000
#define HEX3_HEX0_BASE  0xFF200020
#define HEX5_HEX4_BASE  0xFF200030
#define SW_BASE         0xFF200040

typedef unsigned int hex_digit_t;

hex_digit_t SEVEN_SEG_DISPLAY_PATTERN_LOOKUP[12] = {
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
    0b00000000 << 24, /*   */
    0b01000000 << 24  /* - */
};

// volatile because hardware
volatile unsigned int* const hex_register_one = (unsigned int*)(HEX3_HEX0_BASE);
volatile unsigned int* const hex_register_two = (unsigned int*)(HEX5_HEX4_BASE);

// Input is a 32 bit value in decimal
void display_hex(int value)
{
    // The max value displayable is 999,999
    if (value > 999999) return;
    
    int value_cpy = value;

    // take the abs value 
    unsigned int is_negative = 0;
    if (value < 0) {
        value_cpy = 0 - value; 
    } 
    
    unsigned char idx = 0;
    unsigned int digits[6];
    while (value_cpy > 0) {
        digits[idx] = SEVEN_SEG_DISPLAY_PATTERN_LOOKUP[value_cpy % 10];
        
        value_cpy /= 10; // floor of the number div 10 to advance to next place
        idx += 1;
    }

    // make leading places nothing, or add the negative sign just before
    // the first digit
    for (int i = 5; i > idx - 1; i--) {
        // silly 
        digits[i] = SEVEN_SEG_DISPLAY_PATTERN_LOOKUP[10 + (i == idx)];
    }

    *hex_register_one = 0;
    *hex_register_two = 0;

    // register length - 1. Don't bitshift after last entry
    for (idx = 0; idx < 3; idx++) {
        *hex_register_one |= digits[idx];
        *hex_register_one >>= 8;
    }
    *hex_register_one |= digits[3];

    return;
}

int read_switches(void) {
}
	
int main(void) {
    display_hex(-9876);
}
