#ifndef STM32_LCD_H
#define STM32_LCD_H

// 此LCD驱动为ILI9341驱动，使用 FSMC 通信

#include "stm32f4xx_hal.h"

// LCD width and height
#define LCD_Total_Page    320
#define LCD_Total_Col     240

extern volatile uint16_t *FSMC_LCD_Reg;
extern volatile uint16_t *FSMC_LCD_Data;

#define LCD_WR_REG(cmd)    (*FSMC_LCD_Reg   = (uint16_t) cmd)
#define LCD_WR_DATA(data)  (*FSMC_LCD_Data  = (uint16_t) data)

// RGB565 color definition
#define WHITE   0xffff
#define BLACK   0x0000
#define BLUE    0x001f
#define RED     0xf800
#define GREEN   0x07e0
#define YELLOW  0xff40
#define GREY    0x7bef
#define PINK    0xf91c

// 方向设置
#define LCD_DIR_UL      0
#define LCD_DIR_UR      1
#define LCD_DIR_DL      2
#define LCD_DIR_DR      3

void LCD_Init();
void LCD_SetDirection(uint16_t _dir);
void LCD_DisplayOn();
void LCD_DisplayOff();
void LCD_DrawPoint(uint16_t _x, uint16_t _y, uint16_t _color);
void LCD_SetDispWindow(uint16_t _sx, uint16_t _sy, uint16_t _ex, uint16_t _ey);
void LCD_Fill(uint16_t _sx, uint16_t _sy, uint16_t _ex, uint16_t _ey, uint16_t _color);
void LCD_DrawBlock(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _color);
void LCD_DrawLine(uint16_t _sx, uint16_t _sy, uint16_t _ex, uint16_t _ey, uint16_t _color);
void LCD_DrawRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint16_t _color);

#endif //STM32_LCD_H
