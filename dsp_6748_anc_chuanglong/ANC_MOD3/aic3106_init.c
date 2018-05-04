/****************************************************************************/
/*                                                                          */
/*              广州创龙电子科技有限公司                                    */
/*                                                                          */
/*              Copyright 2015 Tronlong All rights reserved                 */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*              AIC3106初始化                                               */
/*                                                                          */
/*              2015年07月20日                                              */
/*                                                                          */
/****************************************************************************/
#include "codecif.h"
#include "aic3106_init.h"

/****************************************************************************/
/*                                                                          */
/*              宏定义                                                      */
/*                                                                          */
/****************************************************************************/
#define AIC31_RESET                 (0x80)

#define AIC31_SLOT_WIDTH_16         (0u << 4u)
#define AIC31_SLOT_WIDTH_20         (1u << 4u)
#define AIC31_SLOT_WIDTH_24         (2u << 4u)
#define AIC31_SLOT_WIDTH_32         (3u << 4u)

/****************************************************************************/
/*                                                                          */
/*              函数声明                                                    */
/*                                                                          */
/****************************************************************************/
/**
 *  功能	复位AIC3106
 *
 *  参数	baseAddr :连接 AIC3106 接口的基地址（I2C的基地址）
 *
 *  返回	无
 *
 **/
void AIC31Reset(unsigned int baseAddr)
{
	// 选择 Page 0
    I2CRegWrite(baseAddr, AIC31_P0_REG0, 0);

    // 复位
    I2CRegWrite(baseAddr, AIC31_P0_REG1, AIC31_RESET);
}

/**
 *  功能	初始化 AIC3106 数据模式和 slot 位宽
 *
 *  参数	baseAddr : 连接 AIC3106 接口的基地址（I2C的基地址）
 *  		dataType : 设置数据类型模式
 *  		slotWidth: 设置 Slot 的位宽
 *  		dataOff  : 从字时钟的上升沿到捕获有效数据的时钟个数
 *
 *  			dataType 的值可以设置为：
 *  				AIC31_DATATYPE_I2S - I2S 模式 \n
 *              	AIC31_DATATYPE_DSP - DSP 模式 \n
 *              	AIC31_DATATYPE_RIGHTJ - 右对齐模式 \n
 *              	AIC31_DATATYPE_LEFTJ -  左对齐模式 \n
 *
 *  返回	无
 *
 **/
void AIC31DataConfig(unsigned int baseAddr, unsigned char dataType, 
                     unsigned char slotWidth, unsigned char dataOff)
{
    unsigned char slot;

    switch(slotWidth)
    {
        case 16:
            slot = AIC31_SLOT_WIDTH_16;
        break;

        case 20:
            slot = AIC31_SLOT_WIDTH_20;
        break;

        case 24:
            slot = AIC31_SLOT_WIDTH_24;
        break;

        case 32:
            slot = AIC31_SLOT_WIDTH_32;
        break;

        default:
            slot = AIC31_SLOT_WIDTH_16;
        break;
    }

    // 设置数据模式和 slot 位宽
    I2CRegWrite(baseAddr, AIC31_P0_REG9, (dataType | slot));
  
    // 字时钟的上升沿到捕获有效数据的时钟个数
    I2CRegWrite(baseAddr, AIC31_P0_REG10, dataOff);
}

/**
 *  功能	设置 AIC3106 的采样率
 *
 *  参数	baseAddr  : 连接 AIC3106 接口的基地址（I2C的基地址）
 *  		mode      : 选择 AIC3106 内部的 DAC 或者 ADC
 *  		sampleRate: 采样率
 *
 *  			mode 的值可以设置为：
 *  				AIC31_MODE_ADC -  选择 ADC \n
 *                	AIC31_MODE_DAC -  选择 DAC \n
 *                	AIC31_MODE_BOTH -  选择 ADC 和 DAC \n
 *  			sampleRate 的值可以设置为：
 *  				FS_8000_HZ ,FS_11025_HZ ,FS_16000_HZ ,FS_22050_HZ ,FS_24000_HZ
 *  				FS_32000_HZ ,S_44100_HZ ,FS_48000_HZ ,FS_96000_HZ
 *  			采样率 fs 由以下等式推导：
 *  				fs = (PLL_IN * [pllJval.pllDval] * pllRval) /(2048 * pllPval).
 *  			其中 PLL_IN 由外部晶振输入 	PLL_IN = 24576 kHz
 *
 *  返回	无
 *
 **/
void AIC31SampleRateConfig(unsigned int baseAddr, unsigned int mode, 
                           unsigned int sampleRate)
{
    unsigned char fs;
    unsigned char ref = 0x0Au;
    unsigned char temp;
    unsigned char pllPval = 4u;
    unsigned char pllRval = 1u;
    unsigned char pllJval = 16u; 
    unsigned short pllDval = 0u;

    // 采样率参数选择
    switch(sampleRate)
    {
        case 8000:
            fs = 0xAAu;
        break;

        case 11025:
            fs = 0x66u;
            ref = 0x8Au;
            pllJval = 14u;
            pllDval = 7000u;
        break;

        case 16000:
            fs = 0x44u;
        break;

        case 22050:
            fs = 0x22u;
            ref = 0x8Au;
            pllJval = 14u;
            pllDval = 7000u;
        break;

        case 24000:
            fs = 0x22u;
        break;
    
        case 32000:
            fs = 0x11u;
        break;

        case 44100:
            ref = 0x8Au;
            fs = 0x00u;
            pllJval = 14u;
            pllDval = 7000u;
        break;

        case 48000:
            fs = 0x00u;
        break;

        case 96000:
            ref = 0x6Au;
            fs = 0x00u;
        break;

        default:
            fs = 0x00u;
        break;
    }
    
    temp = (mode & fs);
   
    // 设置采样率
    I2CRegWrite(baseAddr, AIC31_P0_REG2, temp);
  
    I2CRegWrite(baseAddr, AIC31_P0_REG3, 0x80 | pllPval);

    // 使用 PLLCLK_IN 作为 MCLK
    I2CRegWrite(baseAddr, AIC31_P0_REG102, 0x08);

    // 使用 PLLDIV_OUT 作为 CODEC_CLKIN
    I2CRegBitClr(baseAddr, AIC31_P0_REG101, 0x01);

    // GPIO1 选择输出分频后的PLL IN
    I2CRegWrite(baseAddr, AIC31_P0_REG98, 0x20);

    temp = (pllJval << 2);
    I2CRegWrite(baseAddr, AIC31_P0_REG4, temp);

    // 初始化 PLL 分频寄存器
    I2CRegWrite(baseAddr, AIC31_P0_REG5, (pllDval >> 6) & 0xFF);
    I2CRegWrite(baseAddr, AIC31_P0_REG6, (pllDval & 0x3F) << 2);

    temp = pllRval;
    I2CRegWrite(baseAddr, AIC31_P0_REG11, temp);

    // 使能编解码器作为主机用于输出fs 和 bclk
    I2CRegWrite(baseAddr, AIC31_P0_REG8, 0xD0);

    I2CRegWrite(baseAddr, AIC31_P0_REG7, ref);
}

/**
 *  功能	初始化 AIC3106 的 ADC 及其输出增益
 *
 *  参数	baseAddr  : 连接 AIC3106 接口的基地址（I2C的基地址）
 *  		adcGain   : ADC 的输出增益
 *  		inSource  : 模拟信号输入源
 *
 *  			adcGain 的值可以设置为： 0~59.5，其值为0.5的倍数
 *  			inSource的值可以设置为：
 *  				AIC31_LINE_IN -  选择 LINE IN \n
 *  				AIC31_MIC_IN -  选择 MIC IN \n
 *
 *  返回	无
 *
 **/
void AIC31ADCInit(unsigned int baseAddr, float adcGain, unsigned char inSource)
{
    unsigned char adc_gain = adcGain/0.5;

	// 设置左右声道的 ADC 增益
    I2CRegWrite(baseAddr, AIC31_P0_REG15, adc_gain);
    I2CRegWrite(baseAddr, AIC31_P0_REG16, adc_gain);

    // 采样率参数选择
	switch(inSource)
	{
		case AIC31_LINE_IN:
			// MIC3L/R 不连接到左 ADC PGA
			I2CRegWrite(baseAddr, AIC31_P0_REG17, 0xFF);

			// MIC3L/R 不连接到右 ADC PGA
			I2CRegWrite(baseAddr, AIC31_P0_REG18, 0xFF);

			// Line L1R 上电
			I2CRegWrite(baseAddr, AIC31_P0_REG19, 0x04);

			// Line L1L 上电
			I2CRegWrite(baseAddr, AIC31_P0_REG22, 0x04);
		break;

		case AIC31_MIC_IN:
			// 失能 LINE IN 连接到 ADC
			I2CRegWrite(baseAddr, AIC31_P0_REG19, 0x7C);
			I2CRegWrite(baseAddr, AIC31_P0_REG22, 0x7C);

			// 连接 MIC IN 连接到 ADC
			I2CRegWrite(baseAddr, AIC31_P0_REG17, 0x0F);
			I2CRegWrite(baseAddr, AIC31_P0_REG18, 0xF0);

			// MIC IN 上电
			I2CRegWrite(baseAddr, AIC31_P0_REG25, 0xc0);
		break;
	}


}

/**
 *  功能	初始化 AIC3106 的 DAC 及其输入衰减
 *
 *  参数	baseAddr  : 连接 AIC3106 接口的基地址（I2C的基地址）
 *  		dacAtten  : DAC 的输入衰减
 *
 *  			dacAtten 的值可以设置为： 0~63.5，其值为0.5的倍数
 *
 *  返回	无
 *
 **/
void AIC31DACInit(unsigned int baseAddr, float dacAtten)
{
    unsigned char dac_atten = dacAtten/0.5;

	// 左右声道 DACs 上电
    I2CRegWrite(baseAddr, AIC31_P0_REG37, 0xE0);

    // 选择 DAC L1 R1 路径
    I2CRegWrite(baseAddr, AIC31_P0_REG41, 0x02);
    I2CRegWrite(baseAddr, AIC31_P0_REG42, 0x6C);


    // DAC L 连接到 HPLOUT
    I2CRegWrite(baseAddr, AIC31_P0_REG47, 0x80);
    I2CRegWrite(baseAddr, AIC31_P0_REG51, 0x09);

    // DAC R 连接到 HPROUT
    I2CRegWrite(baseAddr, AIC31_P0_REG64, 0x80);
    I2CRegWrite(baseAddr, AIC31_P0_REG65, 0x09);

    // DACL1 连接到 LINE1 LOUT
    I2CRegWrite(baseAddr, AIC31_P0_REG82, 0x80);
    I2CRegWrite(baseAddr, AIC31_P0_REG86, 0x09);

    // DACR1 连接到 LINE1 ROUT
    I2CRegWrite(baseAddr, AIC31_P0_REG92, 0x80);
    I2CRegWrite(baseAddr, AIC31_P0_REG93, 0x09);

    // 设置 DAC 衰减
    I2CRegWrite(baseAddr, AIC31_P0_REG43, dac_atten);
    I2CRegWrite(baseAddr, AIC31_P0_REG44, dac_atten);
}

/***************************** End Of File ***********************************/
