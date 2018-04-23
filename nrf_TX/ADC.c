
#include <I2C.h>

/* ��ȡ��ǰ��ADCת��ֵ��chn-ADCͨ����0~3 */
unsigned char GetADCValue(unsigned char chn)
{
    unsigned char val;
    
    I2CStart();
    if (!I2CWrite(0x48<<1))  //ѰַPCF8591����δӦ����ֹͣ����������0
    {
        I2CStop();
        return 0;
    }
    I2CWrite(0x40|chn);        //д������ֽڣ�ѡ��ת��ͨ��
    I2CStart();
    I2CWrite((0x48<<1)|0x01);  //ѰַPCF8591��ָ������Ϊ������    
    I2CReadACK();              //�ȿն�һ���ֽڣ��ṩ����ת��ʱ��
    val = I2CReadNAK();        //��ȡ�ո�ת�����ֵ
    I2CStop();
    
    return val;
}

/* ADCת��ֵתΪʵ�ʵ�ѹֵ���ַ�����ʽ��str-�ַ���ָ�룬val-ADת��ֵ */
void ValueToString(unsigned char *str, unsigned char val)
{
    //��ѹֵ=ת�����*2.5V/255��ʽ�е�25������һλʮ����С��
                                   /*val = (val*50) / 255;  /*val = (val*25) / 255; */
	str[0] = (val/100) + '0';      /*��λ*//*str[0] = (val/10) + '0';  //����λ�ַ� */
    str[1] = (val/10)%10 + '0';    /*ʮλ*//*str[1] = '.'; //С����	*/
    str[2] = (val%10) + '0';       /*��λ*///С��λ�ַ�

    str[3] = '\0'; /*str[3] = 'V'; //��ѹ��λ*/
                  /* str[4] = '\0';   //������	*/
}
