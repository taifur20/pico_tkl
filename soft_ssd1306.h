// Copyright 2022 Yoichiro Tanaka (@yoichiro)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

// OLED init
void oled_init_soft(void);

// Basic drawing
void ssd1306_clear(void);
void ssd1306_fill(uint8_t value);
void ssd1306_print(const char *str, uint8_t page, uint8_t col);
void ssd1306_print_double(const char *str, uint8_t page, uint8_t col);
void oled_print_line(uint8_t line, const char *str);

