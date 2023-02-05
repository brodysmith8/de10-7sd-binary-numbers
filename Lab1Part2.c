/* Your first program*/

#define LED_BASE        0xFF200000
#define HEX3_HEX0_BASE  0xFF200020
#define HEX5_HEX4_BASE  0xFF200030
#define SW_BASE         0xFF200040

#define min(a, b) \
    ({  __typeof__ (a) _a = a; \
        __typeof__ (b) _b = b; \
        _a < _b ? _a : _b; })

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

unsigned char SEVEN_SEG_DISPLAY_PATTERN_LOOKUP[10] = {
    0b00111111, /* 0 */
    0b00000110, /* 1 */
    0b01011011, /* 2 */
    0b01001111, /* 3 */
    0b01100110, /* 4 */
    0b01101101, /* 5 */
    0b01111101, /* 6 */
    0b00000111, /* 7 */
    0b01111111, /* 8 */
    0b01101111, /* 9 */
    0b01000000  /* - */
};

typedef struct _seven_segment_display {
    unsigned char top_bar; // 8 bit integer
    unsigned char top_right_bar; // 8 bit integer
    unsigned char bottom_right_bar; // 8 bit integer
    unsigned char bottom_bar; // 8 bit integer
    unsigned char bottom_left_bar; // 8 bit integer
    unsigned char top_left_bar; // 8 bit integer
    unsigned char centre_bar; // 8 bit integer
    unsigned char unused; // 8 bit integer
} seven_segment_display;

// could make these a bank struct
volatile seven_segment_display* const hex0_ptr = (seven_segment_display*)(HEX3_HEX0_BASE);
volatile seven_segment_display* const hex1_ptr = (seven_segment_display*)(HEX3_HEX0_BASE + 0x20);
volatile seven_segment_display* const hex2_ptr = (seven_segment_display*)(HEX3_HEX0_BASE + 0x40);
volatile seven_segment_display* const hex3_ptr = (seven_segment_display*)(HEX3_HEX0_BASE + 0x60);
volatile seven_segment_display* const hex4_ptr = (seven_segment_display*)(HEX5_HEX4_BASE);
volatile seven_segment_display* const hex5_ptr = (seven_segment_display*)(HEX5_HEX4_BASE + 0x20);

// Input is a 32 bit value in decimal
void display_hex(int value)
{
    // The max value displayable is 999,999
    if (value > 999999) return;
    
    int value_cpy = value; // if value is negative, c implicitly 
    unsigned char is_negative = 0;
    if (value < 0) {
        is_negative = 1;
        value_cpy = 0 - value; // take the abs value 
    } 
    
    // I don't know if this is the fastest way to do this
    unsigned char idx = 0;
    unsigned char digits[6];
    while (value_cpy > 0) {
        digits[idx] = value_cpy % 10;
        value_cpy /= 10; // floor of the number div 10
        idx += 1;
    }

    // use minterms for 0-9 in a lookup table :^)
    for (idx = 5; idx >= 0; idx++) {
        // write to 6 - idx th hex display idx-th digit in the lookup table 
    }
}

int read_switches(void)
{
}
	
int main(void) {
}
