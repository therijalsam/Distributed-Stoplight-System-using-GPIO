#include <stdint.h>
#include "sleep.h"

#define LED_Data      *((uint32_t *)0x41210000)
#define Switch_Data   *((uint32_t *)0x41220000)
#define Button_Data   *((uint32_t *)0x41200000)

// define the addresses for the GPIO inputs directly
#define Bank0_Input   *((uint32_t *)0xE000A060)
#define Bank2_Input   *((uint32_t *)0xE000A068)

// the GPIO BANK output data
#define Bank0_Output  *((uint32_t *)0xE000A040)
#define Bank2_Output  *((uint32_t *)0xE000A048)

// the GPIO BANK Direction data
#define Bank0_Dir     *((uint32_t *)0xE000A204)
#define Bank2_Dir     *((uint32_t *)0xE000A284)

#define Bank0_Enable  *((uint32_t *)0xE000A208)
#define Bank2_Enable  *((uint32_t *)0xE000A288)

// PMODC pin values from class notes
#define PMODC_PIN1    0x08000
#define PMODC_PIN2    0x10000
#define PMODC_PIN3    0x20000
#define PMODC_PIN4    0x40000
#define PMODC_PIN7    0x080000
#define PMODC_PIN8    0x100000
#define PMODC_PIN9    0x200000
#define PMODC_PIN10   0x400000

// LED12 bits on bank0
#define LD12_B        0x10000
#define LD12_R        0x20000
#define LD12_G        0x40000

// traffic light values
#define RED           1
#define YELLOW        2
#define GREEN         3

void set_ld12_red(void) {
    Bank0_Output = LD12_R;
}

void set_ld12_yellow(void) {
    Bank0_Output = LD12_R | LD12_G;
}

void set_ld12_green(void) {
    Bank0_Output = LD12_G;
}

void send_color(uint8_t color) {
    // clear PMODC output pins 1,2,3 first
    Bank2_Output &= ~(PMODC_PIN1 | PMODC_PIN2 | PMODC_PIN3);

    if (color == RED) {
        Bank2_Output |= PMODC_PIN1;
    }
    else if (color == YELLOW) {
        Bank2_Output |= PMODC_PIN2;
    }
    else if (color == GREEN) {
        Bank2_Output |= PMODC_PIN3;
    }
}

uint8_t read_color(void) {
    uint32_t input_val;
    input_val = Bank2_Input;

    if (input_val & PMODC_PIN7) {
        return RED;
    }
    else if (input_val & PMODC_PIN8) {
        return YELLOW;
    }
    else if (input_val & PMODC_PIN9) {
        return GREEN;
    }

    return RED;
}

void show_color(uint8_t color) {
    if (color == RED) {
        set_ld12_red();
    }
    else if (color == YELLOW) {
        set_ld12_yellow();
    }
    else if (color == GREEN) {
        set_ld12_green();
    }
}

int main(void) {
    uint8_t NS;
    uint8_t color_in;

    // setup the pins 1-4 as outputs on PMODC
    Bank2_Dir = 0x78000;
    Bank2_Output = 0x00000;
    Bank2_Enable = 0x78000;

    // setup for LED12
    Bank0_Dir = 0x70000;
    Bank0_Output = LD12_R;     // start with red
    Bank0_Enable = 0x70000;

    while (1) {
        // read SW0
        NS = Switch_Data & 0x1;

        // if SW0 = 1, this board is N/S
        if (NS == 1) {
            // time 0: both red
            set_ld12_red();
            send_color(RED);
            sleep(2);

            // green, red
            set_ld12_green();
            send_color(RED);
            sleep(2);

            // yellow, red
            set_ld12_yellow();
            send_color(RED);
            sleep(2);

            // red, red
            set_ld12_red();
            send_color(RED);
            sleep(1);

            // red, green
            set_ld12_red();
            send_color(GREEN);
            sleep(2);

            // red, yellow
            set_ld12_red();
            send_color(YELLOW);
            sleep(2);

            // red, red
            set_ld12_red();
            send_color(RED);
            sleep(1);
        }
        else {
            color_in = read_color();
            show_color(color_in);
        }
    }

    return 0;
}
