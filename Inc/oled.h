#ifndef __OLED_H
#define __OLED_H			  	 

#include "main.h"
#include "gpio.h"

//OLED模式设置
//0:4线串行模式
#define u8 unsigned char	
#define u32 unsigned long 
		    						  
//-----------------OLED端口定义----------------  
#define OLED_CS_Clr()     HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)
#define OLED_CS_Set()     HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)

#define OLED_DC_Clr()     HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define OLED_DC_Set()     HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)

#define OLED_RST_Clr()    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define OLED_RST_Set()    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)
		
//OLED模式设置
//并行8080模式
enum WEEK {SUN=0,MON,TUE,WED,THU,FRI,SAT};
enum SIZE {SMALL_SIZE =8 , MEDIAN_SIZE = 16 };
enum NUM_SIZE {MEDIAN_NUM = 24 ,MEDIAN_COLON = 25, BIG_NUM = 32 ,BIG_COLON = 33};
enum DIRECTION {LEFT = 0x27 , RIGHT = 0x26};
enum INVERSE {POSITIVE = 0xA6 , NEGATIVE = 0xA7 };

//#define SIZE	16
#define Max_Column	128

//OLED控制用函数
void OLED_WR_Cmd(u8 cmd); 				// 写命令
void OLED_WR_Byte(u8 dat);	     	// 写数据
void OLED_Display_On(void);
void OLED_Display_Off(void);	
void OLED_Scroll_Display(u8 start_page,u8 end_page,enum DIRECTION dirct);
void OLED_Inverse_Display(u8 page,enum INVERSE inverse);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,enum SIZE size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,enum SIZE size);
void OLED_ShowString(u8 x,u8 y,u8 *chr,enum SIZE size);	 
void OLED_Set_Pos(u8 x, u8 y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ShowCHineseWeek(u8 x,u8 y,u8 no);
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);
void OLED_ShowBigNum(u8 x,u8 y,u8 no,enum NUM_SIZE size);
#endif  

