#include <string.h>
#include "main.h"
#include "ws2812-spi.hpp"

uint8_t ws2812_buffer[WS2812_BUFFER_SIZE];

void ws2812_init(void) {
	memset(ws2812_buffer, 0x80, WS2812_BUFFER_SIZE - WS2812_RESET_PULSE);
	memset(ws2812_buffer, 0, WS2812_RESET_PULSE);
	HAL_SPI_Transmit_DMA(&WS2812_SPI_HANDLE, ws2812_buffer, WS2812_BUFFER_SIZE);
}

#define WS2812_FILL_BUFFER(COLOR) \
    for( uint8_t mask = 0x80; mask; mask >>= 1 ) { \
        if( COLOR & mask ) { \
            *ptr++ = 0xfc; \
        } else { \
            *ptr++ = 0x80; \
        } \
    }

void ws2812_pixel(uint16_t led_no, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t *ptr = &ws2812_buffer[24 * led_no];
	WS2812_FILL_BUFFER(g);
	WS2812_FILL_BUFFER(r);
	WS2812_FILL_BUFFER(b);
}

void ws2812_pixel_all(uint8_t r, uint8_t g, uint8_t b) {
	uint8_t *ptr = ws2812_buffer;
	for (uint16_t i = 0; i < WS2812_NUM_LEDS; ++i) {
		WS2812_FILL_BUFFER(g);
		WS2812_FILL_BUFFER(r);
		WS2812_FILL_BUFFER(b);
	}
}
