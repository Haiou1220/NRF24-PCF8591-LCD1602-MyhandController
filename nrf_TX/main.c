/**********************51单片机学习例程************************
*  平台：Keil U4 + STC89C52
*  名称：测试NRF24L01模块，发射代码，发送的数据为
*					5个0x23
*  编写：MCU起航
*  日期：2015-8-17
*   QQ : 2246080671
*  晶振:11.0592MHZ
*	 更改记录：未更改
******************************************************************/
#include <reg52.h>
#include "24l01.h"	 //function
#include "reg24l01.h" //commands and registers
#include "uart.h" //UART函数声明
#include "Lcd1602.h"
#include "ADC.h"
/******************************************/
void ConfigTimer0(unsigned int ms); //定时器t0
void delay_ms(unsigned int x);    //延时函数
/******************************************/
bit flag300ms = 1;       //300ms定时标志
unsigned char T0RH = 0;  //T0重载值的高字节
unsigned char T0RL = 0;  //T0重载值的低字节
/********************************************/
uchar  tx_buf[TX_PLOAD_WIDTH] = {0,0,0,0,0};

/************************************************************************************/

void main(void)
{
    unsigned char val;
    unsigned char str[5];
	/*****/
	unsigned char sta,sta1,sta2;
	delay_ms(1000);
	init_24l01_io();		//引脚初始化
	delay_ms(1000);
	/*****/
	TX_Mode();
	UART_INIT();	
	/*****/
    EA = 1;            //开总中断
    ConfigTimer0(10);  //配置T0定时10ms
    InitLcd1602();     //初始化液晶    
    LcdShowStr(0, 0, "AIN0  AIN1  AIN3");  //显示通道指示
	/*****/
	while(1)
	{
		CE = 0;
  	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); //将数据送入发射缓冲区
		CE = 1;
		while(IRQ);
		sta=SPI_Read(STATUS);
		SPI_RW_Reg(FLUSH_TX, 0xff);					//清除发射缓冲区
		SPI_RW_Reg(WRITE_REG+STATUS,sta);			//清除各种中断标志
		delay_ms(1000);

	//	uart_send_byte(sta);
	//	uart_send_byte( sta1	  ); //SPI_Read(CONFIG)
	//	uart_send_byte( sta2	  ); //SPI_Read(RF_CH )
		/***/
		 if (1)
        {
            flag300ms = 0;
            //显示通道0的电压 /*不是电压是0-255*/
           tx_buf[0]=sta1 = val= GetADCValue(0);     //获取ADC通道0的转换值
            ValueToString(str, val);  //转为字符串格式的电压值
            LcdShowStr(0, 1, str);    //显示到液晶上

            //显示通道1的电压	/*不是电压是0-255*/
           tx_buf[1] = sta2 = val = GetADCValue(1);
            ValueToString(str, val);
            LcdShowStr(6, 1, str);
            //显示通道3的电压
           tx_buf[2]= val = GetADCValue(3);
            ValueToString(str, val);
            LcdShowStr(12, 1, str);
        }
		/***/

	}
}

 /************************************************************************************/
 void delay_ms(unsigned int x)
{
    unsigned int i,j;
    i=0;
    for(i=0;i<x;i++)
    {
       j=108;
           ;
       while(j--);
    }
}

/* 配置并启动T0，ms-T0定时时间 */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //临时变量
    
    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 12;           //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}

/* T0中断服务函数，执行300ms定时 */
void InterruptTimer0() interrupt 1
{
    static unsigned char tmr300ms = 0;
    
    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    tmr300ms++;
    if (tmr300ms >= 30)  //定时300ms
    {
        tmr300ms = 0;
        flag300ms = 1;
    }
}




