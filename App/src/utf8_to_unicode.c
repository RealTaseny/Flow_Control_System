//
// Created by Taseny on 25-4-6.
//

#include "utf8_to_unicode.h"

uint8_t font_data[FONT_CHAR_BYTES] = {0};

static const FONT_RANGE font_ranges[] = {
    {0x4E00, 0x9FA5, 0},          // 基本汉字
    {0x9FA6, 0x9FD5, 20902},      // 基本汉字补充
    {0x3400, 0x4DB5, 20950},      // 扩展A
    {0xF900, 0xFAD9, 27532},      // 兼容汉字
    {0x3000, 0x303F, 28006},      // CJK标点符号
    {0xFF00, 0xFFEE, 28070},      // 半角及全角字符
    {0xFE10, 0xFE19, 28309},      // 纵向标点1
    {0xFE30, 0xFE4F, 28319},      // 纵向标点2
    {0x3200, 0x33FF, 28351},      // CJK特殊符号
    {0x31C0, 0x31E3, 28863},      // 汉字笔画
    {0x2F00, 0x2FD5, 28899},      // 康熙部首
};

uint8_t get_utf8_char_length(const char ch)
{
    if (ch < 0x80) return 1;
    else if ((ch & 0xE0) == 0xC0) return 2;
    else if ((ch & 0xF0) == 0xE0) return 3;
    else if ((ch & 0xF8) == 0xF0) return 4;
    return 0; // 非法编码
}

// 将一个UTF-8字符转换为Unicode码点，返回该字符占用的字节数，错误时返回 0
void utf8_to_unicode(const char *ptrStr, uint32_t* ptr_unicode, uint8_t char_bytes)
{
    char ch[4];

    for (uint8_t i = 0; i < char_bytes; i++)
    {
        ch[i] = *ptrStr;
        ptrStr++;
    }

    switch (char_bytes)
    {
    case 1:
        *ptr_unicode = ch[0];
        break;
    case 2:
        *ptr_unicode = ((ch[0] & 0x1F) << 6) | (ch[1] & 0x3F);
        break;
    case 3:
        *ptr_unicode = ((ch[0] & 0x0F) << 12) |
            ((ch[1] & 0x3F) << 6) |
            (ch[2] & 0x3F);
        break;
    case 4:
        *ptr_unicode = ((ch[0] & 0x07) << 18) |
            ((ch[1] & 0x3F) << 12) |
            ((ch[2] & 0x3F) << 6) |
            (ch[3] & 0x3F);
        break;
    default:
        *ptr_unicode = 0x00000000;
        break;
    }
}

// 返回字库中的索引，若不在范围内则返回 0
uint16_t get_font_index(uint32_t unicode)
{
    if (unicode >= font_ranges[0].start && unicode <= font_ranges[0].end)
        return unicode - font_ranges[0].start + font_ranges[0].base; // 基本汉字: 0 ~ 20901

    else if (unicode >= font_ranges[1].start && unicode <= font_ranges[1].end)
        return unicode - font_ranges[1].start + font_ranges[1].base; // 基本汉字补充

    else if (unicode >= font_ranges[2].start && unicode <= font_ranges[2].end)
        return unicode - font_ranges[2].start + font_ranges[2].base; // 扩展A

    else if (unicode >= font_ranges[3].start && unicode <= font_ranges[3].end)
        return unicode - font_ranges[3].start + font_ranges[3].base; // 兼容汉字

    else if (unicode >= font_ranges[4].start && unicode <= font_ranges[4].end)
        return unicode - font_ranges[4].start + font_ranges[4].base; // CJK标点符号

    else if (unicode >= font_ranges[5].start && unicode <= font_ranges[5].end)
        return unicode - font_ranges[5].start + font_ranges[5].base; // 半角及全角字符

    else if (unicode >= font_ranges[6].start && unicode <= font_ranges[6].end)
        return unicode - font_ranges[6].start + font_ranges[6].base; // CJK纵向标点符号（一段）

    else if (unicode >= font_ranges[7].start && unicode <= font_ranges[7].end)
        return unicode - font_ranges[7].start + font_ranges[7].base; // CJK纵向标点符号（二段）

    else if (unicode >= font_ranges[8].start && unicode <= font_ranges[8].end)
        return unicode - font_ranges[8].start + font_ranges[8].base; // CJK特殊符号

    else if (unicode >= font_ranges[9].start && unicode <= font_ranges[9].end)
        return unicode - font_ranges[9].start + font_ranges[9].base; // 汉字笔画

    else if (unicode >= font_ranges[10].start && unicode <= font_ranges[10].end)
        return unicode - font_ranges[10].start + font_ranges[10].base; // 康熙部首

    else
        return 0; // 不在字库范围内
}

uint8_t get_font_model(uint32_t unicode, const uint8_t** font, uint8_t char_bytes)
{
    uint32_t offset;
    UINT br;
    uint16_t font_model_index = 0;
    switch(char_bytes)
    {
    case 1:
        if (unicode >= 0x20) *font = &ascii_font_6x8[(unicode - 0x20)][0];
        return ASCII_WIDTH;

    case 2:
        font_model_index = get_font_index(unicode);
        offset = font_model_index * FONT_CHAR_BYTES;

        retUSER = f_lseek(&fontFile, offset);
        if (retUSER != FR_OK) {
            f_close(&fontFile);
            return 0xFF;
        }

        retUSER = f_read(&fontFile, font_data, FONT_CHAR_BYTES, &br);
        if (retUSER != FR_OK || br != FONT_CHAR_BYTES) return 0xFF;

        *font = font_data;
        return UNICODE_WIDTH;

    case 3:
        font_model_index = get_font_index(unicode);
        offset = font_model_index * FONT_CHAR_BYTES;

        retUSER = f_lseek(&fontFile, offset);
        if (retUSER != FR_OK) {
            printf("Error: %d\r\n", retUSER);
            f_close(&fontFile);
            return 0xFF;
        }

        retUSER = f_read(&fontFile, font_data, FONT_CHAR_BYTES, &br);
        if (retUSER != FR_OK || br != FONT_CHAR_BYTES)
        {
            printf("Read Error: %d\r\n", retUSER);
            return 0xFF;
        }

        *font = font_data;
        return UNICODE_WIDTH;

    case 4:
        font_model_index = get_font_index(unicode);
        offset = font_model_index * FONT_CHAR_BYTES;

        retUSER = f_lseek(&fontFile, offset);
        if (retUSER != FR_OK) {
            f_close(&fontFile);
            return 0xFF;
        }

        retUSER = f_read(&fontFile, font_data, FONT_CHAR_BYTES, &br);
        if (retUSER != FR_OK || br != FONT_CHAR_BYTES) return 0xFF;

        *font = font_data;
        return UNICODE_WIDTH;

    default:
        *font = &ascii_font_6x8[0][0]; //无法匹配，显示空白
        return 0;
    }
}



