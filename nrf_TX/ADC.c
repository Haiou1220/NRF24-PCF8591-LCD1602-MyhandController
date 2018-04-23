
#include <I2C.h>

/* 读取当前的ADC转换值，chn-ADC通道号0~3 */
unsigned char GetADCValue(unsigned char chn)
{
    unsigned char val;
    
    I2CStart();
    if (!I2CWrite(0x48<<1))  //寻址PCF8591，如未应答，则停止操作并返回0
    {
        I2CStop();
        return 0;
    }
    I2CWrite(0x40|chn);        //写入控制字节，选择转换通道
    I2CStart();
    I2CWrite((0x48<<1)|0x01);  //寻址PCF8591，指定后续为读操作    
    I2CReadACK();              //先空读一个字节，提供采样转换时间
    val = I2CReadNAK();        //读取刚刚转换完的值
    I2CStop();
    
    return val;
}

/* ADC转换值转为实际电压值的字符串形式，str-字符串指针，val-AD转换值 */
void ValueToString(unsigned char *str, unsigned char val)
{
    //电压值=转换结果*2.5V/255，式中的25隐含了一位十进制小数
                                   /*val = (val*50) / 255;  /*val = (val*25) / 255; */
	str[0] = (val/100) + '0';      /*百位*//*str[0] = (val/10) + '0';  //整数位字符 */
    str[1] = (val/10)%10 + '0';    /*十位*//*str[1] = '.'; //小数点	*/
    str[2] = (val%10) + '0';       /*个位*///小数位字符

    str[3] = '\0'; /*str[3] = 'V'; //电压单位*/
                  /* str[4] = '\0';   //结束符	*/
}
