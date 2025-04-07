#ifndef IPS_160_H__
#define IPS_160_H__

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "gpio.h"
#include "zf_common_function.h"
#include "font.h"
#include "fatfs.h"
#include "tim.h"
#include "utf8_to_unicode.h"

#if USE_CHINESE_FONT == 1
#define CHINESE_FONT_LIB_PATH   "FLASH:/font_unicode_ch_12x16.bin"
#endif

#define USE_IPS_ASSERT 1
#define DISPLAY_DELAY 1000

#define IPS160_SPI_PORT (&hspi2)

#define IPS160_DC_PIN GPIO_PIN_14 //PB14
#define IPS160_DC_PORT GPIOB
#define IPS160_RES_PIN GPIO_PIN_5 //PB5
#define IPS160_RES_PORT GPIOB

#define IPS160_RES(x)                   	((x) ? (HAL_GPIO_WritePin(IPS160_RES_PORT, IPS160_RES_PIN, GPIO_PIN_SET))	:	(HAL_GPIO_WritePin(IPS160_RES_PORT, IPS160_RES_PIN, GPIO_PIN_RESET)))
#define IPS160_DC(x)                    	((x) ? (HAL_GPIO_WritePin(IPS160_DC_PORT, IPS160_DC_PIN, GPIO_PIN_SET))	:	(HAL_GPIO_WritePin(IPS160_DC_PORT, IPS160_DC_PIN, GPIO_PIN_RESET)))

#define X_OFFSET 26
#define Y_OFFSET 0

typedef enum
{
    IPS160_PORTAIT                      = 0,
    IPS160_PORTAIT_180                  = 1,
    IPS160_CROSSWISE                    = 2,
    IPS160_CROSSWISE_180                = 3,
}ips160_dir_enum;

#define IPS160_DEFAULT_DISPLAY_DIR      (IPS160_CROSSWISE_180)
#define IPS160_DEFAULT_PENCOLOR         (RGB565_RED    )
#define IPS160_DEFAULT_BGCOLOR          (RGB565_WHITE  )

void    ips160_clear                    (void);
void    ips160_full                     (uint16_t color);
void    ips160_set_brightness           (uint8_t brightness);
void    ips160_set_dir                  (ips160_dir_enum dir);
void    ips160_set_color                (uint16_t pen, uint16_t bgcolor);
void    ips160_draw_point               (uint16_t x, uint16_t y, uint16_t color);
void    ips160_draw_line                (uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);

void    ips160_show_char                (uint8_t x, uint8_t y, uint8_t width, const uint8_t *font, uint16_t bgcolor, uint16_t pencolor, uint8_t show_with_zh);
void    ips160_show_string              (uint16_t x, uint16_t y, char *ptrStr, uint16_t bgcolor, uint16_t pencolor, uint8_t show_with_zh);
void    ips160_show_int                 (uint16_t x, uint16_t y, int32_t dat, uint8_t num);
void    ips160_show_uint                (uint16_t x, uint16_t y, uint32_t dat, uint8_t num);
void    ips160_show_float               (uint16_t x, uint16_t y, double dat, uint8_t num, uint8_t pointnum);

void    ips160_show_binary_image        (uint16_t x, uint16_t y, const uint8_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void    ips160_show_gray_image          (uint16_t x, uint16_t y, const uint8_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold);
void    ips160_show_rgb565_image        (uint16_t x, uint16_t y, const uint16_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t color_mode);

void    ips160_show_wave                (uint16_t x, uint16_t y, const uint16_t *wave, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max);

void    ips160_init											(void);

extern char display_string_buffer[128];

#endif

