#ifndef INC_WS2812_SPI_HPP_
#define INC_WS2812_SPI_HPP_

// https://www.newinnovations.nl/post/controlling-ws2812-and-ws2812b-using-only-stm32-spi/
#define WS2812_NUM_LEDS 16
#define WS2812_SPI_HANDLE hspi2

#define WS2812_RESET_PULSE 60
#define WS2812_BUFFER_SIZE WS2812_NUM_LEDS * 24 + WS2812_RESET_PULSE

extern SPI_HandleTypeDef WS2812_SPI_HANDLE;
extern uint8_t ws2812_buffer[];

void ws2812_init(void);
void ws2812_pixel(uint16_t led_no, uint8_t r, uint8_t g, uint8_t b);
void ws2812_pixel_all(uint8_t r, uint8_t g, uint8_t b);

#endif /* INC_WS2812_SPI_HPP_ */
