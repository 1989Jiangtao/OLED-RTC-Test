#include "oled.h"
#include "oledfont.h"  
#include "spi.h"

extern SPI_HandleTypeDef hspi2;

// 向SSD1306写入一个字节：命令
void OLED_WR_Cmd(u8 cmd)
{
	OLED_DC_Clr(); // 片选拉低，开始写命令
	HAL_SPI_Transmit(&hspi2,&cmd,sizeof(cmd),1000);
	OLED_DC_Set();
}

// 向SSD1306写入一个字节：数据
void OLED_WR_Byte(u8 dat)
{	
	OLED_DC_Set(); // 片选拉高，开始写数据
	HAL_SPI_Transmit(&hspi2,&dat,sizeof(dat),1000);
	OLED_DC_Set();
//	u8 i;			  
//	cmd ? OLED_DC_Set(): OLED_DC_Clr();	 // 根据传入的CMD，判断是写数据还是写命令
//	OLED_CS_Clr(); // 片选置低
//	for(i=0;i<8;i++)
//	{			  
//		OLED_SCLK_Clr(); // 时钟置低
//		dat&0x80 ? OLED_SDIN_Set():OLED_SDIN_Clr();
//	  OLED_SCLK_Set(); // 时钟拉高
//		dat<<=1;   
//	}				 		  
//	OLED_CS_Set(); // 片选拉高
//	OLED_DC_Set();   	  
} 

// 设置位置
void OLED_Set_Pos(u8 x, u8 y) 
{ 
	OLED_WR_Cmd(0xb0+y);
	OLED_WR_Cmd(((x&0xf0)>>4)|0x10);
	OLED_WR_Cmd((x&0x0f)|0x01); 
}   	  

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Cmd(0X8D);  //SET DCDC命令
	OLED_WR_Cmd(0X14);  //DCDC ON
	OLED_WR_Cmd(0XAF);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Cmd(0X8D);  //SET DCDC命令
	OLED_WR_Cmd(0X10);  //DCDC OFF
	OLED_WR_Cmd(0XAE);  //DISPLAY OFF
}		   

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Cmd (0xb0+i);    //设置页地址（0~7）
		OLED_WR_Cmd (0x00);      //设置显示位置—列低地址
		OLED_WR_Cmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0); 
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,enum SIZE size)
{      	
	u8 c=0,i=0;	
	c=chr-' ';//得到偏移后的值
	
	switch(size){
		case SMALL_SIZE:  // 小字体 6x8
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i]);	
			break;
		case MEDIAN_SIZE: // 中尺寸 16x16
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8]);
			break;
	}

}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	
			  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,enum SIZE size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size); 
	}
} 

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,enum SIZE size)
{
	u8 j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],size);
		x+=8;
		if(x>Max_Column)
		{
			x=0;
			y = (size==16? y+2 : y+1); // 根据字体换行
		}
		j++;
	}
}

//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t;
	OLED_Set_Pos(x,y);	
	for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t]);
	}	
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t]);
	}					
}

// 显示星期
void OLED_ShowCHineseWeek(u8 x,u8 y,u8 no)
{      			    
	u8 t;
	OLED_Set_Pos(x,y);	
	for(t=0;t<16;t++)
	{
		OLED_WR_Byte(WeekZk[2*no][t]);
	}	
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(WeekZk[2*no+1][t]);
	}					
}

// start: 开始页 0 - 7
// end  : 结束页 0 - 7
// dirct：方向枚举，left向左，right向右
void OLED_Scroll_Display(u8 start,u8 end,enum DIRECTION dirct)
{
	if(start > 7 || end > 7) return;  // 页码超出
	OLED_WR_Cmd(0x2E); // 关闭滚动
	OLED_WR_Cmd(dirct); // 向右滚动
	OLED_WR_Cmd(0x00); // 空制令
	OLED_WR_Cmd(start);    // 起始页
	OLED_WR_Cmd(0x0F); // 滚动间隔，0=不滚动，值越大，滚动越快
	OLED_WR_Cmd(end);    // 结束页
	OLED_WR_Cmd(0x00); // 空指令
	OLED_WR_Cmd(0xFF); // 空指令，加两条空指令，不然不会生效
	OLED_WR_Cmd(0x2F); // 开启滚动
}
//显示汉字
void OLED_ShowBigNum(u8 x,u8 y,u8 no,enum NUM_SIZE size)
{      			    
	u8 t,i;
	u8 index = (u8)size>>3; // 除以8
	u8 len = (u8)size>>1; // 除以2为数组长度
	for(i=0;i<index;i++)
	{
		OLED_Set_Pos(x,y+i);	
		switch(size)
		{
			case MEDIAN_NUM:
				for(t=0;t<len;t++)
					OLED_WR_Byte(BigNum24[index*no+i][t]);
				break;
			case MEDIAN_COLON:
				for(t=0;t<len;t++)
					OLED_WR_Byte(BigColon24[index*no+i][t]);
				break;
			case BIG_NUM:
				for(t=0;t<len;t++)
					OLED_WR_Byte(BigNum32[index*no+i][t]);
				break;
			case BIG_COLON:
				for(t=0;t<len;t++)
					OLED_WR_Byte(BigColon32[index*no+i][t]);
				break;
		}	
	}		
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[])
{ 	
 unsigned int j=0;
 u8 x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++]);	    	
	    }
	}
}    
//初始化SSD1306					    
void OLED_Init(void)
{ 	
	
	OLED_RST_Clr();
	HAL_Delay(300);
	OLED_RST_Set(); 

	OLED_WR_Cmd(0xAE);//--turn off oled panel
	OLED_WR_Cmd(0x02);//---set low column address
	OLED_WR_Cmd(0x10);//---set high column address
	OLED_WR_Cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Cmd(0x81);//--set contrast control register
	OLED_WR_Cmd(0xff); // Set SEG Output Current Brightness
	OLED_WR_Cmd(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Cmd(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Cmd(0xA6);//--set normal display
	OLED_WR_Cmd(0xA8);//--set multiplex ratio(1 to 64)
	OLED_WR_Cmd(0x3f);//--1/64 duty
	OLED_WR_Cmd(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Cmd(0x00);//-not offset
	OLED_WR_Cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Cmd(0xD9);//--set pre-charge period
	OLED_WR_Cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Cmd(0xDA);//--set com pins hardware configuration
	OLED_WR_Cmd(0x12);
	OLED_WR_Cmd(0xDB);//--set vcomh
	OLED_WR_Cmd(0x40);//Set VCOM Deselect Level
	OLED_WR_Cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Cmd(0x02);//
	OLED_WR_Cmd(0x8D);//--set Charge Pump enable/disable
	OLED_WR_Cmd(0x14);//--set(0x10) disable
	OLED_WR_Cmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Cmd(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Cmd(0xAF);//--turn on oled panel
	
	OLED_WR_Cmd(0xAF); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0);
}  

