// #include "address_map_arm.h"

#define LED_BASE	0xFF200000

/* Our first program!  Watch the blinking light! */
volatile int DELAY_LENGTH; // volatile so it survives compilation

int main(void) {
    /* Declare volatile pointers to I/O registers (volatile means that IO load
     * and store instructions will be used to access these pointer locations,
     * instead of regular memory loads and stores)
    */
    volatile int * LED_ptr       = (int *)LED_BASE; // LED address
    volatile int delay_count; // volatile so the C compiler doesn't remove the loop
	volatile int status;
	
	DELAY_LENGTH = 700000;
	status = 0;
	
    while (1) 
	{
		if (status == 0)
		{
			status = 1;
			*(LED_ptr) |= 0x1;
		}
		else
		{
			status = 0;
			*(LED_ptr) &= ~0x1;
		}

        for (delay_count = DELAY_LENGTH; delay_count != 0; --delay_count)
            ; // delay loop
    }
}
