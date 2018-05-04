#include "stdio.h"
#include "string.h"
#include "AncProc.h"

static float gafX1_NR[PREFILT_LEN*2];
static float gafX_NR[LS_LEN*2];
static float gafD1_NR[PREFILT_LEN*2];
static float gafXs_NR[LS_LEN*2];
static float gafF11_NR[LC_LEN*2];
static float gafWc1_NR[LC_LEN];

static float gafX1_BM[PREFILT_LEN*2];  // 参考信号
static float gafX_BM[LS_LEN*2];
static float gafD1_BM[PREFILT_LEN*2];

float gafWs[LS_LEN];

#ifdef DEBUG
    float save_en[76526];
#endif

extern const float gafPreFilter[];

// py=py-x*data
void SubVecdMultiply(float x, float *pdata, float *py, unsigned short num)
{
	unsigned short loopcnt, idx;
	float tmp1, tmp2, *pdata1;
	loopcnt = num & 0xfffe;
	pdata1 = pdata+1;

	for(idx=0; idx < loopcnt; idx+=2)
	{
		tmp1 = x*pdata[idx];
		tmp2 = x*pdata1[idx];
		py[idx] = py[idx]-tmp1;
		py[idx+1] = py[idx+1]-tmp2;
	}

	if (num & 0x0001)
	{
		py[idx] = py[idx]-x*pdata[idx];
	}
}

float DotProduct(float * x, float * y, unsigned int nx)
{
    unsigned int i;
    float sum = 0;

    for(i = 0; i < nx; i++)
    {
        sum += x[i] * y[i];
    }

    return (sum);
}

//square mean power
float SqrMeanPowCalc(float *px, unsigned short num)
{
    float res0=0, res1=0, *px1=px+1;
    unsigned short idx=0, loopcnt;
    loopcnt = num & 0xfffe;

    for(; idx < loopcnt; idx+=2)
    {
        res0+=px[idx]*px[idx];
        res1+=px1[idx]*px1[idx];
    }

    if (num & 0x0001)
    {
        res0+=px[idx]*px[idx];
    }
    //res0 = res0+res1;

    //no mean and no square root calculation 160811
#if 0
    res0 = (res0+res1)/num;
    res0 = sqrt(res0);
#else
    res0 += res1;
#endif

    return res0;
}

// data = data * x
void VecdMultiply(float x, float *pdata, unsigned short num)
{
    unsigned short loopcnt, idx;
    float *pdata1;
    loopcnt = num & 0xfffe;
    pdata1 = pdata+1;

    for(idx=0; idx < loopcnt; idx+=2)
    {
        pdata[idx] = x*pdata[idx];
        pdata1[idx] = x*pdata1[idx];
    }

    if (num & 0x0001)
    {
        pdata[idx] = x*pdata[idx];
    }

}


void AncInit(void)
{

}

/**
 *  功能  降噪算法
 *
 *  输入参数  errorSignal_ee：误差信号
 *            refSignal_xx：参考信号
 *            Ws          ：传递函数
 *            wsLen       : 传递函数长度
 *
 *  返回  ctlSignal_yn：扬声器激励信号
 *
 **/
void NoiceReduce(float errorSignal_ee, float refSignal_xx, float *Ws, unsigned int wsLen, float *ctlSignal_yn)
{
    static float *gafX1_p = &gafX1_NR[PREFILT_LEN-1];  // 参考信号
    static float *gafX_p = &gafX_NR[LS_LEN-1];
    static float *gafD1_p = &gafD1_NR[PREFILT_LEN-1];
    static float *gafXs_p = &gafXs_NR[LS_LEN-1];
    static float *gafF11_p = &gafF11_NR[LC_LEN-1];
    float x1n, yn, en, Xfactor, tmp, f11n;
    float meanPower;

    /*******************************************/
    *gafX1_p = refSignal_xx;                                                       // X1= [x(n);X1(1:Lf-1)];
    x1n = DotProduct(gafX1_p, (float *)&gafPreFilter[0], PREFILT_LEN);            //x1(n)=X1'*Prefilter';
                                                                                  //
    //gafPreFilter;
    if (gafX1_p == &gafX1_NR[0])
    {
        memcpy(&gafX1_NR[PREFILT_LEN], gafX1_p, PREFILT_LEN * sizeof(float));
        gafX1_p = &gafX1_NR[PREFILT_LEN-1];
    }
    else
    {
        gafX1_p--;
    }
    /********************************************/
   *gafX_p = x1n;                                                                  //X = [x1(n);X(1:Lc-1)];
    yn = DotProduct(gafX_p, gafWc1_NR, LC_LEN);                                    //y(n) = X'*Wc1;

    //gafPreFilter;
    if (gafX_p == &gafX_NR[0])
    {
        memcpy(&gafX_NR[LC_LEN], gafX_p, LC_LEN * sizeof(float));
        gafX_p = &gafX_NR[LC_LEN-1];
    }
    else
    {
        gafX_p--;
    }
    /********************************************/
   *gafXs_p = x1n;                                                                 //Xs = [x1(n);Xs(1:Ls-1)];
    f11n = DotProduct(gafXs_p, Ws, wsLen);                                         //f11(n) = Xs'*Ws;

    //gafPreFilter;
    if (gafXs_p == &gafXs_NR[0])
    {
        memcpy(&gafXs_NR[wsLen], gafXs_p, wsLen * sizeof(float));
        gafXs_p = &gafXs_NR[wsLen-1];
    }
    else
    {
        gafXs_p--;
    }
    /********************************************/
   *gafF11_p = f11n;                                                                //F11 = [f11(n);F11(1:Lc-1)];
    //gafPreFilter;
    if (gafF11_p == &gafF11_NR[0])
    {
        memcpy(&gafF11_NR[LC_LEN], gafF11_p, LC_LEN * sizeof(float));
        gafF11_p = &gafF11_NR[LC_LEN-1];
    }
    else
    {
        gafF11_p--;
    }
    meanPower = SqrMeanPowCalc(gafF11_p, LC_LEN);
    tmp = 1/meanPower;
    VecdMultiply(tmp, gafF11_p, LC_LEN);                                           //F11=F11/sum(F11.^2);
    /********************************************/
    *gafD1_p = errorSignal_ee;                                                     //E= [e(n);E(1:Lf-1)];
    en = DotProduct(gafD1_p,  (float *)&gafPreFilter[0], PREFILT_LEN);            //e1(n)=E'*Prefilter';

    //gafPreFilter;
    if (gafD1_p == &gafD1_NR[0])
    {
        memcpy(&gafD1_NR[PREFILT_LEN], gafD1_p, PREFILT_LEN * sizeof(float));
        gafD1_p = &gafD1_NR[PREFILT_LEN-1];
    }
    else
    {
        gafD1_p--;
    }
    /********************************************/

    Xfactor = en*muCtrl;
    SubVecdMultiply(Xfactor, gafF11_p, gafWc1_NR, LC_LEN); //update Wc1            Wc1 = Wc1 - mu*e1(n)*F11;

    *ctlSignal_yn = yn;
}

/**
 *  功能  建模算法
 *
 *  输入参数  desSignal_dd：期望信号
 *            refSignal_xx：参考信号
 *            wsLen       : 传递函数长度
 *
 *  返回  Ws：返回传递函数
 *
 **/
void BuildMode(float desSignal_dd, float refSignal_xx, float *Ws, unsigned int wsLen)
{
	static float *gafX1_p = &gafX1_BM[PREFILT_LEN-1];  // 参考信号
	static float *gafX_p = &gafX_BM[LS_LEN-1];
	static float *gafD1_p = &gafD1_BM[PREFILT_LEN-1];
	float x1n, yn, d1n, en, Xfactor;

	/*******************************************/
	*gafX1_p = refSignal_xx;                                                   // X1=[x(n);X1(1:Lf-1)];
	x1n = DotProduct(gafX1_p, (float *)&gafPreFilter[0], PREFILT_LEN);         // x1(n)=X1'*Prefilter';

	//gafPreFilter;
	if (gafX1_p == &gafX1_BM[0])
	{
		memcpy(&gafX1_BM[PREFILT_LEN], gafX1_p, PREFILT_LEN * sizeof(float));
		gafX1_p = &gafX1_BM[PREFILT_LEN-1];
	}
	else
	{
		gafX1_p--;
	}
	/********************************************/
	*gafX_p = x1n;                                                            //  X = [x1(n);X(1:Ls-1)];
	yn = DotProduct(gafX_p, Ws, wsLen);                                   //  y(n) = X'*Ws;

	//gafPreFilter;
	if (gafX_p == &gafX_BM[0])
	{
		memcpy(&gafX_BM[wsLen], gafX_p, wsLen * sizeof(float));
		gafX_p = &gafX_BM[wsLen-1];
	}
	else
	{
		gafX_p--;
	}
	/********************************************/
	//input depends on whether exp is routed to left or right channel
	*gafD1_p = desSignal_dd;                                                 //  D1=[d(n);D1(1:Lf-1)];
	d1n = DotProduct(gafD1_p, (float *)&gafPreFilter[0], PREFILT_LEN);      //  d1(n)=D1'*Prefilter';

	//gafPreFilter;
	if (gafD1_p == &gafD1_BM[0])
	{
		memcpy(&gafD1_BM[PREFILT_LEN], gafD1_p, PREFILT_LEN * sizeof(float));
		gafD1_p = &gafD1_BM[PREFILT_LEN-1];
	}
	else
	{
		gafD1_p--;
	}
	/********************************************/
	en = yn + d1n;                                                         //  e(n) = d1(n)+y(n);

	Xfactor = en*muMod;
	SubVecdMultiply(Xfactor, (gafX_p+1), Ws, wsLen);                   //  Ws = Ws - mu*e(n)*X;

#ifdef DEBUG
	static int i = 0;
	save_en[i] = en;
	i++;
	if(i == 76526)
	    i = 0;
#endif
}
