#ifndef _UC1701_H
#define _UC1701_H
#include"sys.h"

#define Cmd 0
#define Dat 1
#define UC1701_SCLK PBout(9)

#define UC1701_SDA PBout(8)
#define UC1701_RS PBout(6)
#define UC1701_RST PBout(7)
#define UC1701_CS PBout(5)

#define GDRAM_EN 1//软件显示缓存使能位

void UC1701_Init(void);
void ClearScreen(void);
void FullFill(void);
void Graphic16x8Str(u8 page,u8 column,u8 str[],u8 pixs,u8 white);
void Graphic16X16(u8 page,u8 column,u8 num,u8 white);
void Graphic8X16(u8 page,u8 column,u8 Ascii,u8 white);
void UC1701ShowNum(u8 page,u8 column,u32 num,u8 len,u8 white);

#if(GDRAM_EN)
void DrawPoint(u8 x, u8 y, u8 pix);
void Gui_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 Color);
#endif

#endif

