/****************************************************************************/
/*                                                                          */
/*              广州创龙电子科技有限公司                                    */
/*                                                                          */
/*              Copyright 2015 Tronlong All rights reserved                 */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*              I2C接口API头文件                                            */
/*                                                                          */
/*              2015年04月07日                                              */
/*                                                                          */
/****************************************************************************/

#ifndef _CODECIF_H_
#define _CODECIF_H_

/****************************************************************************/
/*                                                                          */
/*              宏定义					                                    */
/*                                                                          */
/****************************************************************************/
extern void I2CSetup(unsigned int baseAddr, unsigned int slaveAddr);
extern void I2CIntRegister(unsigned int cpuINT, unsigned int sysIntNum);

/****************************************************************************/
/*                                                                          */
/*              			 IIC接口函数                                    */
/*                                                                          */
/****************************************************************************/
extern void I2CRegWrite(unsigned int baseAddr, unsigned char regAddr,
                          unsigned char regData);
extern void I2CRegWrite3(unsigned int baseAddr, unsigned char regAddr,
                          unsigned char regData, unsigned char regData2);

extern void I2CRegBitSet(unsigned int baseAddr, unsigned char regAddr,
                           unsigned char bitMask);
extern void I2CRegBitClr(unsigned int baseAddr, unsigned char regAddr,
                           unsigned char bitMask);
extern unsigned char I2CRegRead(unsigned int baseAddr,
                                  unsigned char regAddr);

#endif
