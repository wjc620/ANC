/****************************************************************************/
/*                                                                          */
/*              广州创龙电子科技有限公司                                    */
/*                                                                          */
/*              Copyright 2015 Tronlong All rights reserved                 */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*              音频测试：LINE_IN读取音频数据，LINE_OUT输出数据（中断方式） */
/*                                                                          */
/*              2015年07月13日                  							*/
/*                                                                          */
/****************************************************************************/
#include <c6x.h>

#include "TL6748.h"                 // 创龙 DSP6748 开发板相关声明
#include "hw_types.h"
#include "edma_event.h" 
#include "interrupt.h"
#include "soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"

#include "codecif.h"
#include "mcasp.h"
#include "edma.h"
#include "psc.h"
#include "uartStdio.h"
#include "dspcache.h"

#include "aic3106_init.h"
#include "mcasp_init.h"

#include "AncProc.h"

/****************************************************************************/
/*                                                                          */
/*              宏定义                                                      */
/*                                                                          */
/****************************************************************************/
// AIC3106 I2C从地址
#define I2C_SLAVE_CODEC_AIC31                 (0x18u)

// I2S 使用2个 slot
#define I2S_SLOTS                             (2u)

// 发送/接收 slot 大小
#define SLOT_SIZE                             (16u)

// 发送/接收数据 word 大小. Word size <= Slot size
#define WORD_SIZE                             (16u)

/****************************************************************************/
/*                                                                          */
/*              全局变量                                                    */
/*                                                                          */
/****************************************************************************/
extern const signed short gadWgnTbl[];

#ifdef DEBUG
    float save_yn[76526];

    extern float siso_xx_data[];
    extern float siso_ee_data[];
    extern float siso_Ws_data[];

    extern float spmod_xx_data[];
    extern float spmod_dd_data[];

    // 计数器（用于性能测试）
    long long t_start, t_stop, t_overhead;
    int t_test[76526];
#endif

/****************************************************************************/
/*                                                                          */
/*              函数声明                                                    */
/*                                                                          */
/****************************************************************************/
static void InitAIC31I2S(void);
static void InitMcaspIntr(void);
// DSP 中断初始化
static void InterruptInit(void);

/****************************************************************************/
/*                                                                          */
/*              McASP 中断函数                                              */
/*                                                                          */
/****************************************************************************/
static void McASPIsr(void)
{
	unsigned int status;
	static unsigned int spkOut = 0, nearSpkIn = 0, nearChairIn = 0;
	static unsigned int buildModeSamCnt = 0, wgnPlaySamCnt = 0;
	float tmp1, tmp2, tmp3;

	IntEventClear(SYS_INT_MCASP0_INT);

	status = McASPTxStatusGet(SOC_MCASP_0_CTRL_REGS);

	if(status & MCASP_TX_STAT_CURRSLOT_EVEN)
	{
        // 输出右声道数据 激励扬声器
        OutputSample(spkOut);
        // 读取右通道数据 声源附近传声器采集
        nearSpkIn = InputSample();

        if(buildModeSamCnt < 80000)
        {
            tmp1 = (float)nearSpkIn;
            tmp2 = (float)gadWgnTbl[wgnPlaySamCnt++];
            if(wgnPlaySamCnt == 16000)
            {
                wgnPlaySamCnt = 0;
            }
            BuildMode(tmp1, tmp2, gafWs, LS_LEN);
        }
        else
        {
            tmp1 = (float)nearSpkIn;
            tmp2 = (float)nearChairIn;
            NoiceReduce(tmp2, tmp1, gafWs, LS_LEN, &tmp3);
            if(tmp3 > 65535)
            {
                tmp3 = 65535;
            }
            else if(tmp3 < 0)
            {
                tmp3 = 0;
            }
            spkOut = (unsigned int)tmp3;
        }

#ifdef DEBUG
        static int j = 0;
//        // 计数开始值
//        t_start = _itoll(TSCH, TSCL);
//        NoiceReduce(siso_ee_data[j], siso_xx_data[j], siso_Ws_data, LS_LEN, &save_yn[j]);
//        // 计数结束值
//        t_stop = _itoll(TSCH, TSCL);
//        t_test[j] = (t_stop - t_start) - t_overhead;

        // 计数开始值
        t_start = _itoll(TSCH, TSCL);
        BuildMode(spmod_dd_data[j], spmod_xx_data[j], gafWs, LS_LEN);
        // 计数结束值
        t_stop = _itoll(TSCH, TSCL);
        t_test[j] = (t_stop - t_start) - t_overhead;
        j++;
        if(j >= 76526)
        {
            asm(" SWBP 0 ");
            j = 0;
        }
#endif
	}
	else
	{
		// 读取左通道数据
	    nearChairIn = InputSample();
		// 输出左声道数据
		OutputSample(0);
	}
}

/****************************************************************************/
/*                                                                          */
/*              主函数                                                      */
/*                                                                          */
/****************************************************************************/
int main(void)
{
    // 使能缓存
	CacheEnableMAR((unsigned int)0xC0000000, (unsigned int)0x10000000);
	CacheEnable(L1PCFG_L1PMODE_32K | L1DCFG_L1DMODE_32K | L2CFG_L2MODE_256K);

#ifdef DEBUG
    // 计数器（用于性能测试）
    TSCL = 0;
    TSCH = 0;
    t_start = _itoll(TSCH, TSCL);
    t_stop = _itoll(TSCH, TSCL);
    t_overhead = t_stop - t_start;
#endif

	// 串口终端初始化 使用串口2
    UARTStdioInit();

    UARTPuts("\r\n ============Test Start===========.\r\n", -1);
	UARTPuts("Welcome to StarterWare Audio_Line_In_Intr Demo application.\r\n\r\n", -1);
	UARTPuts("This application loops back the input at LINE_IN of the EVM to the LINE_OUT of the EVM\r\n\r\n", -1);

    // I2C 模块引脚配置
    I2CPinMuxSetup(0);

    // McASP 引脚配置
    McASPPinMuxSetup();

    // DSP 中断初始化
    InterruptInit();

    // 初始化 I2C 接口地址为 AIC31 的地址
    I2CSetup(SOC_I2C_0_REGS, I2C_SLAVE_CODEC_AIC31);
    I2CIntRegister(C674X_MASK_INT5, SYS_INT_I2C0_INT);

	// 初始化 AIC31 音频芯片
    InitAIC31I2S();

	// 初始化 McASP 为中断方式
    InitMcaspIntr();

    while(1);
}

/****************************************************************************/
/*                                                                          */
/*              初始化 AIC31 音频芯片                                       */
/*                                                                          */
/****************************************************************************/
static void InitAIC31I2S(void)
{
    volatile unsigned int delay = 0xFFF;

    // 复位
    AIC31Reset(SOC_I2C_0_REGS);
    while(delay--);

    // 初始化 AIC31 为 I2S 模式，slot 的大小
    AIC31DataConfig(SOC_I2C_0_REGS, AIC31_DATATYPE_I2S, SLOT_SIZE, 0);

    // 初始化采样率为 8000Hz
    AIC31SampleRateConfig(SOC_I2C_0_REGS, AIC31_MODE_BOTH, FS_8000_HZ);

    // 初始化 ADC 0分贝增益,连接 LINE IN
    AIC31ADCInit(SOC_I2C_0_REGS, ADC_GAIN_0DB, AIC31_LINE_IN);

    // 初始化 DAC 0分贝衰减
    AIC31DACInit(SOC_I2C_0_REGS, DAC_ATTEN_0DB);
}

/****************************************************************************/
/*                                                                          */
/*              初始化 McASP 为中断方式                                     */
/*                                                                          */
/****************************************************************************/
static void InitMcaspIntr(void)
{
	// 初始化 McASP 为 I2S 模式
	McASPI2SConfigure(MCASP_BOTH_MODE, WORD_SIZE, SLOT_SIZE, I2S_SLOTS, MCASP_MODE_NON_DMA);

	// 使能发送中断
	McASPTxIntEnable(SOC_MCASP_0_CTRL_REGS, MCASP_TX_DATAREADY);

	// 设置 McASP 中断
	McASPIntSetup(C674X_MASK_INT6, McASPIsr);

	// 启动 McASP 发送和接收
	I2SDataTxRxActivate(MCASP_BOTH_MODE);
}

/****************************************************************************/
/*                                                                          */
/*              DSP 中断初始化                                              */
/*                                                                          */
/****************************************************************************/
static void InterruptInit(void)
{
	// 初始化 DSP 中断控制器
	IntDSPINTCInit();

	// 使能 DSP 全局中断
	IntGlobalEnable();
}




