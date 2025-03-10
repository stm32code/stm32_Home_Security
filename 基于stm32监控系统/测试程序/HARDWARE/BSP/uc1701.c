#include"uc1701.h"
#include"delay.h"
#include"uc1701font.h"
#if(GDRAM_EN)
u8 GDRAM[8][128] = {0};
#endif
void UC1701GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void UC1701_WriteByte(u8 byte, u8 cmd)
{
    u8 i;
    UC1701_RS = cmd;
    UC1701_CS = 0;
    for(i = 0; i < 8; i++)
    {
        UC1701_SCLK = 0;
        if(byte & 0x80)UC1701_SDA = 1;
        else UC1701_SDA = 0;
        UC1701_SCLK = 1;
        byte <<= 1;
    }
    UC1701_CS = 1;
}
void UC1701_Init(void)
{

    UC1701GPIO_Init();
    UC1701_RST = 0; //复位
    delay_us(100);
    UC1701_RST = 1;
    delay_ms(20);
    UC1701_WriteByte(0XE2, Cmd); //软件复位
    delay_ms(5);
    UC1701_WriteByte(0X2c, Cmd); //升压步骤一
    delay_ms(5);
    UC1701_WriteByte(0X2e, Cmd); //升压步骤二
    delay_ms(5);
    UC1701_WriteByte(0X2f, Cmd); //升压步骤三
    delay_ms(5);
    UC1701_WriteByte(0X23, Cmd); //粗调对比度 0x20-0x27
    UC1701_WriteByte(0X81, Cmd); //微调对比度
    UC1701_WriteByte(0X35, Cmd); //对比度值 0x00-0x3f
    UC1701_WriteByte(0Xa2, Cmd); // 1/9 偏压比
    UC1701_WriteByte(0Xc8, Cmd); //行扫描顺序 上到下
    UC1701_WriteByte(0Xa0, Cmd); //列扫描顺序 左到右
    UC1701_WriteByte(0X40, Cmd); //起始行 第一行
    UC1701_WriteByte(0Xaf, Cmd); //开显示
    ClearScreen();
}
//设定页地址和列地址
void SetPosXY(u8 page, u8 column)
{
    // UC1701_CS=1;
    UC1701_WriteByte(0XB0 + page, Cmd); //设定页地址
    UC1701_WriteByte(0X10 + (column >> 4), Cmd); //设定列地址高四位
    UC1701_WriteByte(column & 0x0f, Cmd); //设定列地址 低四位
}
//清屏
void ClearScreen(void)
{
    u8 i, j;
    UC1701_CS = 0;
    for(i = 0; i < 8; i++)
    {
        SetPosXY(i, 0); //设定页地址，列地址自增
        for(j = 0; j < 128; j++)
        {
#if(GDRAM_EN)
            GDRAM[i][j] = 0x00;
#endif
            UC1701_WriteByte(0, Dat);
        }
    }
    UC1701_CS = 1;
}
//全屏填充
void FullFill(void)
{
    u8 i, j;
    UC1701_CS = 0;
    for(i = 0; i < 8; i++)
    {
        SetPosXY(i, 0);
        for(j = 0; j < 128; j++)
        {
#if(GDRAM_EN)
            GDRAM[i][j] = 0xff;
#endif
            UC1701_WriteByte(0xff, Dat);
        }
    }
    UC1701_CS = 1;
}
//指定位置写入汉字或者其他点阵
void Graphic16X16(u8 page, u8 column, u8 num, u8 white)
{
    u8 i, j, k;
    UC1701_CS = 0;
    k = num * 32;
    for(i = 0; i < 2; i++)
    {
        SetPosXY(page + i, column);
        for(j = 0; j < 16; j++)
        {
            if(white)
            {
                UC1701_WriteByte(~HanZi[k + j + i * 16], Dat);
				#if(GDRAM_EN)
                GDRAM[page + i][column + j] = ~HanZi[k + j + i * 16];
#endif
            }
            else
            {
				#if(GDRAM_EN)
                GDRAM[page + i][column + j] = HanZi[k + j + i * 16];
#endif
                UC1701_WriteByte(HanZi[k + j + i * 16], Dat);
            }
        }
    }
    UC1701_CS = 1;
}
//指定位置写入8x16字符
void Graphic8X16(u8 page, u8 column, u8 Ascii, u8 white)
{
    u8 i, j;
    UC1701_CS = 0;
    Ascii -= ' ';
    for(i = 0; i < 2; i++)
    {
        SetPosXY(page + i, column);
		
        for(j = 0; j < 8; j++)
        {
            if(white)
            {
				#if(GDRAM_EN)
                GDRAM[page + i][column + j] = ~font8x16[Ascii][i * 8 + j];
#endif
                UC1701_WriteByte(~font8x16[Ascii][i * 8 + j], Dat);
            }
            else
            {
				#if(GDRAM_EN)
                GDRAM[page + i][column + j] = ~font8x16[Ascii][i * 8 + j];
#endif
                UC1701_WriteByte(font8x16[Ascii][i * 8 + j], Dat);
            }
        }
    }
    UC1701_CS = 1;
}
//指定位置写入6x8字符
void Graphic6X8(u8 page, u8 column, u8 Ascii, u8 white)
{
    u8 i, j;
    UC1701_CS = 0;
    Ascii -= ' ';
    SetPosXY(page + i, column);
    for(j = 0; j < 6; j++)
    {
        if(white)
		{
			#if(GDRAM_EN)
            GDRAM[page + i][column + j] = ~font6x8[Ascii][j];
#endif
			UC1701_WriteByte(~font6x8[Ascii][j], Dat);
		}
        else 
		{
			#if(GDRAM_EN)
            GDRAM[page + i][column + j] = font6x8[Ascii][j];
#endif
			UC1701_WriteByte(font6x8[Ascii][j], Dat);
		}
    }
    UC1701_CS = 1;
}
//计数m^n的值
u32 mypow(u8 m, u8 n)
{
    u32 result = 1;
    while(n--)result *= m;
    return result;
}
void UC1701ShowNum(u8 page, u8 column, u32 num, u8 len, u8 white)
{
    u8 i;
    for(i = len; i > 0; i--)
    {
        Graphic8X16(page, column, num / mypow(10, i - 1) % 10 + '0', white);
        column += 8;
    }
}
/*
pixs:字符间距像素点
该函数可自动换行
*/

void Graphic6x8Str(u8 page, u8 column, u8 str[], u8 pixs, u8 white)
{
    u8 i;
    while(*str != 0x00)
    {
        if(column > 122)
        {
            page++;
            column = 0;
        }
        if(page > 7)break;
        Graphic6X8(page, column, *(str++), white);
        for(i = 0; i < pixs; i++) //对间隔进行补点或空格
        {
            if(white)
			{
				#if(GDRAM_EN)
                GDRAM[page][column + 6 + i] = 0xff;
#endif
				UC1701_WriteByte(0xff, Dat);
			}
            else 
			{
				#if(GDRAM_EN)
                GDRAM[page][column + 6 + i] = 0x00;
#endif
				UC1701_WriteByte(0x00, Dat);
			}
        }
        column += (6 + pixs);
    }
}
/*
pixs:字符间距像素点
该函数可自动换行
*/
void Graphic16x8Str(u8 page, u8 column, u8 str[], u8 pixs, u8 white)
{
    u8 i;
    while(*str != 0x00)
    {
        if(column > 112)
        {
            page += 2;
            column = 0;
        }
        if(page > 6)break;
        Graphic8X16(page, column, *(str++), white);
        for(i = 0; i < pixs; i++) //对间隔进行补点或空格
        {
            SetPosXY(page, column + 8 + i);
            if(white)
			{
				#if(GDRAM_EN)
                GDRAM[page][column + 8 + i] = 0xff;
#endif
				UC1701_WriteByte(0xff, Dat);
			}
            else 
			{
				#if(GDRAM_EN)
                GDRAM[page][column + 8 + i] = 0x00;
#endif
				UC1701_WriteByte(0x00, Dat);
			}
            SetPosXY(page + 1, column + 8 + i);
            if(white)
			{
				#if(GDRAM_EN)
                GDRAM[page + 1][column + 8 + i] = 0xff;
#endif
				UC1701_WriteByte(0xff, Dat);
			}
            else 
			{
				#if(GDRAM_EN)
                GDRAM[page + 1][column + 8 + i] = 0x00;
#endif
				UC1701_WriteByte(0x00, Dat);
			}
        }
        column += (8 + pixs);
    }
}
void BmpShow(u8 *p)
{
    u8 i, j;
    UC1701_CS = 0;
    for(i = 0; i < 8; i++)
    {
        SetPosXY(i, 0); //设定页地址，列地址自增
        for(j = 0; j < 128; j++)
        {
			#if(GDRAM_EN)
            GDRAM[i][j] = *p;
#endif
            UC1701_WriteByte(*(p++), Dat);
        }
    }
    UC1701_CS = 1;
}

#if(GDRAM_EN)
void DrawPoint(u8 x, u8 y, u8 pix)
{
    u8 y_mod, y_yu;
    y_mod = y / 8;
    y_yu = y % 8;
   // SetPosXY(y_mod, x);
	UC1701_WriteByte(0XB0 + y_mod, Cmd); //设定页地址
        UC1701_WriteByte(0X10 + (x >> 4), Cmd); //设定列地址高四位
       UC1701_WriteByte(x & 0x0f, Cmd); 
    if(pix)
    {
        GDRAM[y_mod][x] |= 1 << y_yu;
		UC1701_WriteByte(GDRAM[y_mod][x], Dat);
        
    }
    else
    {
        GDRAM[y_mod][x] &= ~(1 << y_yu);
        UC1701_WriteByte(GDRAM[y_mod][x], Dat);
    }
	
}

//画线函数，使用Bresenham 画线算法
void Gui_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 Color)
{
    short dx,             // difference in x's
          dy,             // difference in y's
          dx2,            // dx,dy * 2
          dy2,
          x_inc,          // amount in pixel space to move during drawing
          y_inc,          // amount in pixel space to move during drawing
          error,          // the discriminant i.e. error i.e. decision variable
          index;          // used for looping
    dx = x1 - x0; //计算x距离
    dy = y1 - y0; //计算y距离
    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx    = -dx;
    }
    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy    = -dy;
    }
    dx2 = dx << 1;
    dy2 = dy << 1;
    if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
    {
        //且线的点数等于x距离，以x轴递增画点
        // initialize error term
        error = dy2 - dx;
        // draw the line
        for (index = 0; index <= dx; index++) //要画的点数不会超过x距离
        {
            //画点
            DrawPoint(x0, y0, Color);
            // test if error has overflowed
            if (error >= 0) //是否需要增加y坐标值
            {
                error -= dx2;
                // move to next line
                y0 += y_inc; //增加y坐标值
            } // end if error overflowed
            // adjust the error term
            error += dy2;
            // move to the next pixel
            x0 += x_inc; //x坐标值每次画点后都递增1
        } // end for
    } // end if |slope| <= 1
    else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
    {
        //以y轴为递增画点
        // initialize error term
        error = dx2 - dy;
        // draw the line
        for (index = 0; index <= dy; index++)
        {
            // set the pixel
            DrawPoint(x0, y0, Color);
            // test if error overflowed
            if (error >= 0)
            {
                error -= dy2;
                // move to next line
                x0 += x_inc;
            } // end if error overflowed
            // adjust the error term
            error += dx2;
            // move to the next pixel
            y0 += y_inc;
        } // end for
    } // end else |slope| > 1
}
#endif


