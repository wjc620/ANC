/*
 * AncProc.h
 *
 *  Created on: 2018Äê3ÔÂ6ÈÕ
 *      Author: 11090
 */

#ifndef ANCPROC_H_
#define ANCPROC_H_

//mod
#define PREFILT_LEN 139
#define LS_LEN 400
//ctrl
#define LC_LEN 200

#define muMod ((float)1e-4)
#define muCtrl ((float)1e-3)

extern float gafWs[];

extern void AncInit(void);
extern void NoiceReduce(float errorSignal_ee, float refSignal_xx, float *Ws, unsigned int wsLen, float *ctlSignal_yn);
extern void BuildMode(float desSignal_dd, float refSignal_xx, float *Ws, unsigned int wsLen);

#endif /* ANCPROC_H_ */
