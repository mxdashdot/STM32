#include "lcd.h"

volatile uint16_t *FSMC_LCD_Reg = (uint16_t *) 0x6C000000; // NE4
volatile uint16_t *FSMC_LCD_Data = (uint16_t *) 0x6C000080; // A6

// init函数中很多指令和ILI9341文档冲突，这个没办法
// 我能保证我的Init函数可以正常工作，但我不能保证官方文档的正确性
void LCD_Init() {
    LCD_WR_REG(0xCF);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC1);
    LCD_WR_DATA(0X30);

    LCD_WR_REG(0xED);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0X12);
    LCD_WR_DATA(0X81);

    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x85);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x7A);

    LCD_WR_REG(0xCB);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x02);

    LCD_WR_REG(0xF7);
    LCD_WR_DATA(0x20);

    LCD_WR_REG(0xEA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC0);    //Power control
    LCD_WR_DATA(0x1B);   //VRH[5:0]

    LCD_WR_REG(0xC1);    //Power control
    LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0]

    LCD_WR_REG(0xC5);    //VCM control
    LCD_WR_DATA(0x30);     //3F
    LCD_WR_DATA(0x30);     //3C

    LCD_WR_REG(0xC7);    //VCM control2
    LCD_WR_DATA(0XB7);

    LCD_WR_REG(0x36);    // Memory Access Control
    LCD_WR_DATA(0x08);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);

    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1A);

    LCD_WR_REG(0xB6);    // Display Function Control
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0xA2);

    LCD_WR_REG(0xF2);    // 3Gamma Function Disable
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x26);    //Gamma curve selected
    LCD_WR_DATA(0x01);

    LCD_WR_REG(0xE0);    //Set Gamma
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x2A);
    LCD_WR_DATA(0x28);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x54);
    LCD_WR_DATA(0XA9);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0XE1);    //Set Gamma
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x56);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x0F);

    LCD_WR_REG(0x11); //Exit Sleep
    HAL_Delay(120);

    LCD_DisplayOn();

    LCD_Fill(0, 0, 239, 319, WHITE);
}

// 设置方向
void LCD_SetDirection(uint16_t _dir) {
    uint16_t data = 0x08;
    data += _dir << 6;

    LCD_WR_REG(0x36);
    LCD_WR_DATA(data);
}

void LCD_DisplayOn() {
    LCD_WR_REG(0x29);
}

void LCD_DisplayOff() {
    LCD_WR_REG(0x28);
}

// 设置显示区域
// 从(_sx, _sy) 到 (_ex, _ey) 之间的矩形区域
void LCD_SetDispWindow(uint16_t _sx, uint16_t _sy, uint16_t _ex, uint16_t _ey) {
    LCD_WR_REG(0x2A);
    LCD_WR_DATA(_sx >> 8);
    LCD_WR_DATA(_sx & 0xFF);
    LCD_WR_DATA(_ex >> 8);
    LCD_WR_DATA(_ex & 0xFF);

    LCD_WR_REG(0x2B);
    LCD_WR_DATA(_sy >> 8);
    LCD_WR_DATA(_sy & 0xFF);
    LCD_WR_DATA(_ey >> 8);
    LCD_WR_DATA(_ey & 0xFF);
}

// 绘制单个点
void LCD_DrawPoint(uint16_t _x, uint16_t _y, uint16_t _color) {
    LCD_SetDispWindow(_x, _x, _y, _y);

    LCD_WR_REG(0x2C);
    LCD_WR_DATA(_color); // color in RGB565 value(uint_16)
}

// 使用颜色填充一块矩形区域
void LCD_Fill(uint16_t _sx, uint16_t _sy, uint16_t _ex, uint16_t _ey, uint16_t _color) {
    LCD_SetDispWindow(_sx, _sy, _ex, _ey);

    LCD_WR_REG(0x2C);
    for (int i = 0; i < (_ex - _sx + 1) * (_ey - _sy + 1); ++i) {
        LCD_WR_DATA(_color);
    }
}

// 绘制一个方形方块
void LCD_DrawBlock(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _color) {
    LCD_SetDispWindow(_x, _y, _x + _w - 1, _y + _w - 1);
    LCD_WR_REG(0x2C);
    for (int i = 0; i < _w * _w; ++i) {
        LCD_WR_DATA(_color);
    }
}

// 绘制一条线
void LCD_DrawLine(uint16_t _sx, uint16_t _sy, uint16_t _ex, uint16_t _ey, uint16_t _color) {

}

void LCD_DrawRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint16_t _color) {

}