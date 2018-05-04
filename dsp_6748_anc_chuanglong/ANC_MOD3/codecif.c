/****************************************************************************/
/*                                                                          */
/*              广州创龙电子科技有限公司                                    */
/*                                                                          */
/*              Copyright 2015 Tronlong All rights reserved                 */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*              I2C接口API                                                  */
/*                                                                          */
/*              2015年03月27日                                              */
/*                                                                          */
/****************************************************************************/
#include <stdio.h>
#include <stdint.h>

#include "TL6748.h"
#include "hw_types.h"
#include "i2c.h"
#include "codecif.h"
#include "interrupt.h"
#include "soc_C6748.h"
#include "hw_syscfg0_C6748.h"

#include "delay.h"

/****************************************************************************/
/*                                                                          */
/*              函数声明                                                    */
/*                                                                          */
/****************************************************************************/
void I2CISR(void);
static void I2CSendBlocking(unsigned int baseAddr, unsigned int dataCnt);
static void I2CRcvBlocking(unsigned int baseAddr, unsigned int dataCnt);
void I2CIntRegister(unsigned int cpuINT, unsigned int sysIntNum);

/****************************************************************************/
/*                                                                          */
/*              全局变量                                                    */
/*                                                                          */
/****************************************************************************/
volatile unsigned int dataIdx = 0;
volatile unsigned int txCompFlag = 1;
volatile unsigned int slaveData[3];
unsigned int savedBase;

/****************************************************************************/
/*                                                                          */
/*              动态创建I2C硬件中断                                         */
/*                                                                          */
/****************************************************************************/
void I2CIntRegister(unsigned int cpuINT, unsigned int sysIntNum)
{
	IntRegister(cpuINT, I2CISR);
	IntEventMap(cpuINT, sysIntNum);
	IntEnable(cpuINT);
}

/****************************************************************************/
/*                                                                          */
/*              初始化I2C接口                                               */
/*                                                                          */
/****************************************************************************/
// baseAddr: I2C模块基地址
// slaveAddr: 器件从地址
void I2CSetup(unsigned int baseAddr, unsigned int slaveAddr)
{
    // 初始化I2C0引脚PINMUX
	I2CPinMuxSetup(0);

    // IIC 复位 / 禁用
    I2CMasterDisable(baseAddr);

    // 配置总线速度为 100KHz
    I2CMasterInitExpClk(baseAddr, 24000000, 8000000, 100000);

    // 设置从设备地址
    I2CMasterSlaveAddrSet(baseAddr, slaveAddr);

    // IIC 使能
    I2CMasterEnable(baseAddr);
}

/****************************************************************************/
/*                                                                          */
/*              I2C发送数据                                                 */
/*                                                                          */
/****************************************************************************/
// iic IIC 模块基址
// dataCnt 数据大小
// 阻塞函数
static void I2CSendBlocking(unsigned int baseAddr, unsigned int dataCnt)
{
    txCompFlag = 1;
    dataIdx = 0;    
    savedBase = baseAddr;
 
    I2CSetDataCount(baseAddr, dataCnt);

    I2CMasterControl(baseAddr, I2C_CFG_MST_TX | I2C_CFG_STOP);

    I2CMasterIntEnableEx(baseAddr, I2C_INT_TRANSMIT_READY | I2C_INT_STOP_CONDITION);

    I2CMasterStart(baseAddr);
   
    // 等待数据发送完成
    while(txCompFlag);
}

//static void I2CCodecSendBlockingNstop(unsigned int baseAddr, unsigned int dataCnt)
//{
//	txCompFlag = 1;
//    dataIdx = 0;
//    savedBase = baseAddr;
//
//    I2CSetDataCount(baseAddr, dataCnt);
//
//    I2CMasterControl(baseAddr, I2C_CFG_MST_TX );//| I2C_CFG_STOP
//
//    I2CMasterIntEnableEx(baseAddr, I2C_INT_TRANSMIT_READY | I2C_INT_STOP_CONDITION);
//
//    I2CMasterStart(baseAddr);
//
//    // 等待数据发送完成
//    while(dataIdx == 0);
////    Task_sleep(19);
//    delay_us(190);
//}

/****************************************************************************/
/*                                                                          */
/*              IIC 接收数据                                                */
/*                                                                          */
/****************************************************************************/
// iic IIC 模块基址
// dataCnt 数据大小
// 阻塞函数
static void I2CRcvBlocking(unsigned int baseAddr, unsigned int dataCnt)
{
    txCompFlag = 1;
    dataIdx = 0;
    savedBase = baseAddr;
    
    I2CSetDataCount(baseAddr, dataCnt);

    I2CMasterControl(baseAddr, I2C_CFG_MST_RX | I2C_CFG_STOP);

    I2CMasterIntEnableEx(baseAddr, I2C_INT_DATA_READY | I2C_INT_STOP_CONDITION);

    I2CMasterStart(baseAddr);

    // 等待数据接收完成
    while(txCompFlag);
}

/****************************************************************************/
/*                                                                          */
/*              IIC 中断服务函数                                            */
/*                                                                          */
/****************************************************************************/
void I2CISR(void)
{
	volatile unsigned int intCode = 0;

	// 取得中断代码
	intCode = I2CInterruptVectorGet(savedBase);

	if (intCode == I2C_INTCODE_TX_READY)
	{
	  I2CMasterDataPut(savedBase, slaveData[dataIdx]);
	  dataIdx++;
	}

	if(intCode == I2C_INTCODE_RX_READY)
	{
	  slaveData[dataIdx] = I2CMasterDataGet(savedBase);
	  dataIdx++;
	}

	if (intCode == I2C_INTCODE_STOP)
	{
	  // 失能中断
	  I2CMasterIntDisableEx(savedBase, I2C_INT_TRANSMIT_READY
									   | I2C_INT_DATA_READY);
	  txCompFlag = 0;
	}

	if (intCode == I2C_INTCODE_NACK)
	{
	 I2CMasterIntDisableEx(SOC_I2C_0_REGS, I2C_INT_TRANSMIT_READY |
										   I2C_INT_DATA_READY |
										   I2C_INT_NO_ACK |
										   I2C_INT_STOP_CONDITION);
	 // 产生停止位
	 I2CMasterStop(SOC_I2C_0_REGS);

	 I2CStatusClear(SOC_I2C_0_REGS, I2C_CLEAR_STOP_CONDITION);

	 // 清除中断
	 IntEventClear(SYS_INT_I2C0_INT);
	 txCompFlag = 0;
	}

	intCode = I2CInterruptVectorGet(savedBase);
}

/****************************************************************************/
/*                                                                          */
/*              写数据到寄存器                                              */
/*                                                                          */
/****************************************************************************/
void I2CRegWrite(unsigned int baseAddr, unsigned char regAddr,
                   unsigned char regData)
{
    // 发送寄存器地址和数据
    slaveData[0] = regAddr;
    slaveData[1] = regData;

    I2CSendBlocking(baseAddr, 2);
}

/****************************************************************************/
/*                                                                          */
/*              IIC 写两个数据到寄存器                                            */
/*                                                                          */
/****************************************************************************/
void I2CRegWrite3(unsigned int baseAddr, unsigned char regAddr,
                    unsigned char regData, unsigned char regData2)
{
	// 发送寄存器地址和数据
    slaveData[0] = regAddr;
    slaveData[1] = regData;
    slaveData[2] = regData2;

    I2CSendBlocking(baseAddr, 3);
}

/****************************************************************************/
/*                                                                          */
/*              读寄存器                                                    */
/*                                                                          */
/****************************************************************************/
unsigned char I2CRegRead(unsigned int baseAddr, unsigned char regAddr)
{
	// 发送寄存器地址
    slaveData[0] = regAddr;
    I2CSendBlocking(baseAddr, 1);
//    I2CCodecSendBlockingNstop(baseAddr, 1);

    // 接收寄存器返回数据
    I2CRcvBlocking(baseAddr, 1);

    return (slaveData[0]);
}

/****************************************************************************/
/*                                                                          */
/*              IIC写寄存器的某一bit                                        */
/*                                                                          */
/****************************************************************************/
void I2CRegBitSet(unsigned int baseAddr, unsigned char regAddr,
                    unsigned char bitMask)
{
	// 发送寄存器地址
    slaveData[0] = regAddr;
    I2CSendBlocking(baseAddr, 1);
//    I2CCodecSendBlockingNstop(baseAddr, 1);
  
    // 接收寄存器返回数据
    I2CRcvBlocking(baseAddr, 1);

    slaveData[1] =  slaveData[0] | bitMask;
    slaveData[0] = regAddr;

    I2CSendBlocking(baseAddr, 2);
}

/****************************************************************************/
/*                                                                          */
/*              IIC 清除寄存器的某一bit                                     */
/*                                                                          */
/****************************************************************************/
void I2CRegBitClr(unsigned int baseAddr, unsigned char regAddr,
                    unsigned char bitMask)
{
	// 发送寄存器地址
    slaveData[0] = regAddr;
    I2CSendBlocking(baseAddr, 1);
//    I2CCodecSendBlockingNstop(baseAddr, 1);

    // 接收寄存器返回数据
    I2CRcvBlocking(baseAddr, 1);

    slaveData[1] =  slaveData[0] & ~bitMask;
    slaveData[0] = regAddr;
   
    I2CSendBlocking(baseAddr, 2);
}

/***************************** End Of File ***********************************/
