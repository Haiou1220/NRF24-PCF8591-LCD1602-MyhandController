/**********************51��Ƭ��ѧϰ����************************
*  ƽ̨��Keil U4 + STC89C52
*  ���ƣ�����NRF24L01ģ�飬������룬���͵�����Ϊ
*					5��0x23
*  ��д��MCU��
*  ���ڣ�2015-8-17
*   QQ : 2246080671
*  ����:11.0592MHZ
*	 ���ļ�¼��δ����
******************************************************************/
#include <reg52.h>
#include "24l01.h"	 //function
#include "reg24l01.h" //commands and registers
#include "uart.h" //UART��������
#include "Lcd1602.h"
#include "ADC.h"
/******************************************/
void ConfigTimer0(unsigned int ms); //��ʱ��t0
void delay_ms(unsigned int x);    //��ʱ����
/******************************************/
bit flag300ms = 1;       //300ms��ʱ��־
unsigned char T0RH = 0;  //T0����ֵ�ĸ��ֽ�
unsigned char T0RL = 0;  //T0����ֵ�ĵ��ֽ�
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
	init_24l01_io();		//���ų�ʼ��
	delay_ms(1000);
	/*****/
	TX_Mode();
	UART_INIT();	
	/*****/
    EA = 1;            //�����ж�
    ConfigTimer0(10);  //����T0��ʱ10ms
    InitLcd1602();     //��ʼ��Һ��    
    LcdShowStr(0, 0, "AIN0  AIN1  AIN3");  //��ʾͨ��ָʾ
	/*****/
	while(1)
	{
		CE = 0;
  	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); //���������뷢�仺����
		CE = 1;
		while(IRQ);
		sta=SPI_Read(STATUS);
		SPI_RW_Reg(FLUSH_TX, 0xff);					//������仺����
		SPI_RW_Reg(WRITE_REG+STATUS,sta);			//��������жϱ�־
		delay_ms(1000);

	//	uart_send_byte(sta);
	//	uart_send_byte( sta1	  ); //SPI_Read(CONFIG)
	//	uart_send_byte( sta2	  ); //SPI_Read(RF_CH )
		/***/
		 if (1)
        {
            flag300ms = 0;
            //��ʾͨ��0�ĵ�ѹ /*���ǵ�ѹ��0-255*/
           tx_buf[0]=sta1 = val= GetADCValue(0);     //��ȡADCͨ��0��ת��ֵ
            ValueToString(str, val);  //תΪ�ַ�����ʽ�ĵ�ѹֵ
            LcdShowStr(0, 1, str);    //��ʾ��Һ����

            //��ʾͨ��1�ĵ�ѹ	/*���ǵ�ѹ��0-255*/
           tx_buf[1] = sta2 = val = GetADCValue(1);
            ValueToString(str, val);
            LcdShowStr(6, 1, str);
            //��ʾͨ��3�ĵ�ѹ
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

/* ���ò�����T0��ms-T0��ʱʱ�� */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //��ʱ����
    
    tmp = 11059200 / 12;      //��ʱ������Ƶ��
    tmp = (tmp * ms) / 1000;  //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 12;           //�����ж���Ӧ��ʱ��ɵ����
    T0RH = (unsigned char)(tmp>>8);  //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}

/* T0�жϷ�������ִ��300ms��ʱ */
void InterruptTimer0() interrupt 1
{
    static unsigned char tmr300ms = 0;
    
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
    tmr300ms++;
    if (tmr300ms >= 30)  //��ʱ300ms
    {
        tmr300ms = 0;
        flag300ms = 1;
    }
}




