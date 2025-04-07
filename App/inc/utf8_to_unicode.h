//
// Created by Taseny on 25-4-6.
//

#ifndef UTF8_TO_UNICODE_H
#define UTF8_TO_UNICODE_H

#include "fatfs.h"

#define USE_CHINESE_FONT 1

#define ASCII_WIDTH 6
#define ASCII_HEIGHT 8

#define UNICODE_WIDTH 12
#define UNICODE_HEIGHT 16
#define FONT_CHAR_BYTES 24  // 每个字符24字节

// 字库编码范围映射表
typedef struct {
    uint32_t start;
    uint32_t end;
    uint32_t base;
} FONT_RANGE;

uint8_t get_utf8_char_length(char ch);
void utf8_to_unicode(const char *ptrStr, uint32_t* ptr_unicode, uint8_t char_bytes);
uint16_t get_font_index(uint32_t unicode);
uint8_t get_font_model(uint32_t unicode, const uint8_t** font, const uint8_t char_bytes);

#endif //UTF8_TO_UNICODE_H
