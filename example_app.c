/*
 * MIT License

Copyright (c) 2017 DeeplyEmbedded

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 * example_app.c
 *
 *  Created on  : Sep 6, 2017
 *  Author      : Vinay Divakar
 *  Website     : www.deeplyembedded.org
 */

/* Lib Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include  <signal.h>
#include "SSD1306_OLED.h"

/* MACRO's */
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

/* Extern volatile */
extern volatile unsigned char flag;

/* Function Declarations */
void display_texts();

/* Bit Map - Taken from Adafruit SSD1306 OLED Library  */
static const unsigned char logo16_glcd_bmp[] =
{
 0b00000000, 0b11000000,
 0b00000001, 0b11000000,
 0b00000001, 0b11000000,
 0b00000011, 0b11100000,
 0b11110011, 0b11100000,
 0b11111110, 0b11111000,
 0b01111110, 0b11111111,
 0b00110011, 0b10011111,
 0b00011111, 0b11111100,
 0b00001101, 0b01110000,
 0b00011011, 0b10100000,
 0b00111111, 0b11100000,
 0b00111111, 0b11110000,
 0b01111100, 0b11110000,
 0b01110000, 0b01110000,
 0b00000000, 0b00110000
};

/* Display Texts
void display_texts(char temp[256], char umi[256], char solo[256])
{
    setTextSize(2);
    setTextColor(WHITE);
    setCursor(10,0);
    print_str(reinterpret_cast<const unsigned char *>("Temperatura: "));
    print_str(reinterpret_cast<const unsigned char *>(temp));
    println();
    println();
    print_str(reinterpret_cast<const unsigned char *>("Humildade: "));
    print_str(reinterpret_cast<const unsigned char *>(umi));
    println();
    println();
    print_str(reinterpret_cast<const unsigned char *>("Solo: "));
    print_str(reinterpret_cast<const unsigned char *>(solo));
}
 */