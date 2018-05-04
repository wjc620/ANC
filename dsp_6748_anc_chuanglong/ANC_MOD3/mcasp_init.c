/****************************************************************************/
/*                                                                          */
/*              广州创龙电子科技有限公司                                    */
/*                                                                          */
/*              Copyright 2015 Tronlong All rights reserved                 */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*              McASP 初始化										        */
/*                                                                          */
/*              2015年07月13日                  							*/
/*                                                                          */
/****************************************************************************/
#include "TL6748.h"                 // 创龙 DSP6748 开发板相关声明

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

/****************************************************************************/
/*                                                                          */
/*              宏定义                                                      */
/*                                                                          */
/****************************************************************************/
// McASP 接收通道
#define MCASP_XSER_RX                         (12u)

// McASP 发送通道
#define MCASP_XSER_TX                         (11u)

/****************************************************************************/
/*                                                                          */
/*              全局变量                                                    */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/*              McASP 输出数据                                              */
/*                                                                          */
/****************************************************************************/
void OutputSample(unsigned int outData)
{
	McASPTxBufWrite(SOC_MCASP_0_CTRL_REGS, 11, outData);
}

/****************************************************************************/
/*                                                                          */
/*              McASP 输入数据包                                            */
/*                                                                          */
/****************************************************************************/
unsigned int InputSample(void)
{
	return (McASPRxBufRead(SOC_MCASP_0_CTRL_REGS, 12));
}

/****************************************************************************/
/*                                                                          */
/*              初始化 McASP 接收通道    	                                */
/*                                                                          */
/****************************************************************************/
void McASPI2SRxConfigure(unsigned char wordSize,unsigned char slotSize,
		unsigned int slotNum, unsigned char modeDMA)
{
	// 复位
	McASPRxReset(SOC_MCASP_0_CTRL_REGS);

	switch(modeDMA)
	{
		case MCASP_MODE_DMA:
			// 使能 FIFO
			McASPReadFifoEnable(SOC_MCASP_0_FIFO_REGS, 1, 1);

			// 设置接收 word 和 slot 的大小
			McASPRxFmtI2SSet(SOC_MCASP_0_CTRL_REGS, wordSize, slotSize,
								MCASP_RX_MODE_DMA);
			break;
		case MCASP_MODE_NON_DMA:
			// 设置接收 word 和 slot 的大小
			McASPRxFmtI2SSet(SOC_MCASP_0_CTRL_REGS, wordSize, slotSize,
								MCASP_RX_MODE_NON_DMA);
			break;
	}

	// 初始化帧同步，TDM 格式使用 slot 个数，对齐帧同步信号的上升沿
	McASPRxFrameSyncCfg(SOC_MCASP_0_CTRL_REGS, slotNum, MCASP_RX_FS_WIDTH_WORD,
						MCASP_RX_FS_EXT_BEGIN_ON_RIS_EDGE);

	// 初始化接收时钟，使用外部时钟，时钟上升沿有效
	McASPRxClkCfg(SOC_MCASP_0_CTRL_REGS, MCASP_RX_CLK_EXTERNAL, 0, 0);
	McASPRxClkPolaritySet(SOC_MCASP_0_CTRL_REGS, MCASP_RX_CLK_POL_RIS_EDGE);
	McASPRxClkCheckConfig(SOC_MCASP_0_CTRL_REGS, MCASP_RX_CLKCHCK_DIV32,
						  0x00, 0xFF);

	// 使能发送接收同步
	McASPTxRxClkSyncEnable(SOC_MCASP_0_CTRL_REGS);
	McASPTxClkCfg(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLK_EXTERNAL, 0, 0);

	// 使能 接收 slot
	McASPRxTimeSlotSet(SOC_MCASP_0_CTRL_REGS, (1 << slotNum)-1);

	// 设置串行器，设置12通道接收
	McASPSerializerRxSet(SOC_MCASP_0_CTRL_REGS, MCASP_XSER_RX);

	// 初始化 McASP 引脚，和引脚输入输出方向
	McASPPinMcASPSet(SOC_MCASP_0_CTRL_REGS, 0xFFFFFFFF);
	McASPPinDirInputSet(SOC_MCASP_0_CTRL_REGS, MCASP_PIN_AFSX
											   | MCASP_PIN_ACLKX
											   | MCASP_PIN_AXR(MCASP_XSER_RX));
}

/****************************************************************************/
/*                                                                          */
/*              初始化 McASP 发送通道    	                                */
/*                                                                          */
/****************************************************************************/
void McASPI2STxConfigure(unsigned char wordSize,unsigned char slotSize,
		unsigned int slotNum, unsigned char modeDMA)
{
	// 复位
	McASPTxReset(SOC_MCASP_0_CTRL_REGS);

	switch(modeDMA)
	{
		case MCASP_MODE_DMA:
			// 使能 FIFO
			McASPWriteFifoEnable(SOC_MCASP_0_FIFO_REGS, 1, 1);

			// 设置发送 word 和 slot 的大小
			McASPTxFmtI2SSet(SOC_MCASP_0_CTRL_REGS, wordSize, slotSize,
								MCASP_TX_MODE_DMA);
			break;
		case MCASP_MODE_NON_DMA:
			// 设置发送 word 和 slot 的大小
			McASPTxFmtI2SSet(SOC_MCASP_0_CTRL_REGS, wordSize, slotSize,
								MCASP_TX_MODE_NON_DMA);
			break;
	}

	// 初始化帧同步，TDM 格式使用 slot 个数，对齐帧同步信号的上升沿
	McASPTxFrameSyncCfg(SOC_MCASP_0_CTRL_REGS, slotNum, MCASP_TX_FS_WIDTH_WORD,
						MCASP_TX_FS_EXT_BEGIN_ON_RIS_EDGE);

	// 初始化发送时钟，使用外部时钟，时钟上升沿有效
	McASPTxClkCfg(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLK_EXTERNAL, 0, 0);
	McASPTxClkPolaritySet(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLK_POL_FALL_EDGE);
	McASPTxClkCheckConfig(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLKCHCK_DIV32,
						  0x00, 0xFF);

	// 使能发送接收同步
	McASPTxRxClkSyncEnable(SOC_MCASP_0_CTRL_REGS);

	// 使能 发送 slot
	McASPTxTimeSlotSet(SOC_MCASP_0_CTRL_REGS, (1 << slotNum)-1);

	// 设置串行器，设置11通道发送
	McASPSerializerTxSet(SOC_MCASP_0_CTRL_REGS, MCASP_XSER_TX);

	// 初始化 McASP 引脚，和引脚输入输出方向
	McASPPinMcASPSet(SOC_MCASP_0_CTRL_REGS, 0xFFFFFFFF);
	McASPPinDirOutputSet(SOC_MCASP_0_CTRL_REGS,MCASP_PIN_AXR(MCASP_XSER_TX));
	McASPPinDirInputSet(SOC_MCASP_0_CTRL_REGS, MCASP_PIN_AFSX
											   | MCASP_PIN_ACLKX);
}

/****************************************************************************/
/*                                                                          */
/*              初始化 McASP 为 I2S 模式	                                */
/*                                                                          */
/****************************************************************************/
void McASPI2SConfigure(unsigned char transmitMode, unsigned char wordSize,
		unsigned char slotSize, unsigned int slotNum, unsigned char modeDMA)
{
	// 使能 McASP 模块 PSC
	PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_MCASP0, PSC_POWERDOMAIN_ALWAYS_ON,
			 PSC_MDCTL_NEXT_ENABLE);

	if(transmitMode & MCASP_TX_MODE)
	{
		McASPI2STxConfigure(wordSize, slotSize, slotNum,  modeDMA);
	}

	if(transmitMode & MCASP_RX_MODE)
	{
		McASPI2SRxConfigure(wordSize, slotSize, slotNum,  modeDMA);
	}
}

/****************************************************************************/
/*                                                                          */
/*              初始化 McASP 中断			                                */
/*                                                                          */
/****************************************************************************/
void McASPIntSetup(unsigned int cpuINT, void (*userISR)(void))
{
	// 初始化中断
	IntRegister(cpuINT, userISR);
	IntEventMap(cpuINT, SYS_INT_MCASP0_INT);
	IntEnable(cpuINT);
}

/****************************************************************************/
/*                                                                          */
/*              启动 McASP 发送和接收			                            */
/*                                                                          */
/****************************************************************************/
void I2SDataTxRxActivate(unsigned char transmitMode)
{
	if(transmitMode & MCASP_TX_MODE)
	{
		// 启动使用外部时钟
		McASPTxClkStart(SOC_MCASP_0_CTRL_REGS, MCASP_TX_CLK_EXTERNAL);

		// 启动串行器
		McASPTxSerActivate(SOC_MCASP_0_CTRL_REGS);

		// 使能状态机
		McASPTxEnable(SOC_MCASP_0_CTRL_REGS);

		// 发送数据0
		McASPTxBufWrite(SOC_MCASP_0_CTRL_REGS, MCASP_XSER_TX, 0);
	}

	if(transmitMode & MCASP_RX_MODE)
	{
		// 启动使用外部时钟
		McASPRxClkStart(SOC_MCASP_0_CTRL_REGS, MCASP_RX_CLK_EXTERNAL);

		// 启动串行器
		McASPRxSerActivate(SOC_MCASP_0_CTRL_REGS);

		// 使能状态机
		McASPRxEnable(SOC_MCASP_0_CTRL_REGS);
	}
}













