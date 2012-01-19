/*
 ** Copyright 2003-2010, VisualOn, Inc.
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 **     http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 */
/*******************************************************************************
	File:		psy_configuration.c

	Content:	Psychoaccoustic configuration functions

*******************************************************************************/

#include "basic_op.h"
#include "oper_32b.h"
#include "psy_configuration.h"
#include "adj_thr.h"
#include "aac_rom.h"



#define BARC_SCALE	100 /* integer barc values are scaled with 100 */
#define LOG2_1000	301 /* log2*1000 */
#define PI2_1000	1571 /* pi/2*1000*/
#define ATAN_COEF1	3560 /* 1000/0.280872f*/
#define ATAN_COEF2	281 /* 1000*0.280872f*/


typedef struct{
  Word32 sampleRate;
  const UWord8 *paramLong;
  const UWord8 *paramShort;
}SFB_INFO_TAB;

static const Word16 ABS_LEV = 20;
static const Word16 BARC_THR_QUIET[] = {15, 10,  7,  2,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                         3,  5, 10, 20, 30};



static const Word16 max_bark = 24; /* maximum bark-value */
static const Word16 maskLow  = 30; /* in 1dB/bark */
static const Word16 maskHigh = 15; /* in 1*dB/bark */
static const Word16 c_ratio  = 0x0029; /* pow(10.0f, -(29.0f/10.0f)) */

static const Word16 maskLowSprEnLong = 30;       /* in 1dB/bark */
static const Word16 maskHighSprEnLong = 20;      /* in 1dB/bark */
static const Word16 maskHighSprEnLongLowBr = 15; /* in 1dB/bark */
static const Word16 maskLowSprEnShort = 20;      /* in 1dB/bark */
static const Word16 maskHighSprEnShort = 15;     /* in 1dB/bark */
static const Word16 c_minRemainingThresholdFactor = 0x0148;    /* 0.01 *(1 << 15)*/
static const Word32 c_maxsnr = 0x66666666;		 /* upper limit is -1 dB */
static const Word32 c_minsnr = 0x00624dd3;		 /* lower limit is -25 dB */

static const Word32 c_maxClipEnergyLong = 0x77359400;  /* 2.0e9f*/
static const Word32 c_maxClipEnergyShort = 0x01dcd650; /* 2.0e9f/(AACENC_TRANS_FAC*AACENC_TRANS_FAC)*/


Word32 GetSRIndex(Word32 sampleRate)
{
    if (92017 <= sampleRate) return 0;
    if (75132 <= sampleRate) return 1;
    if (55426 <= sampleRate) return 2;
    if (46009 <= sampleRate) return 3;
    if (37566 <= sampleRate) return 4;
    if (27713 <= sampleRate) return 5;
    if (23004 <= sampleRate) return 6;
    if (18783 <= sampleRate) return 7;
    if (13856 <= sampleRate) return 8;
    if (11502 <= sampleRate) return 9;
    if (9391 <= sampleRate) return 10;

    return 11;
}


/*********************************************************************************
*
* function name: atan_1000
* description:  calculates 1000*atan(x/1000)
<<<<<<< HEAD
*               based on atan approx for x > 0				
=======
*               based on atan approx for x > 0
>>>>>>> upstream/master
*				atan(x) = x/((float)1.0f+(float)0.280872f*x*x)  if x < 1
*						= pi/2 - x/((float)0.280872f +x*x)	    if x >= 1
* return:       1000*atan(x/1000)
*
**********************************************************************************/
<<<<<<< HEAD
static Word16 atan_1000(Word32 val) 
{
  Word32 y;

   
=======
static Word16 atan_1000(Word32 val)
{
  Word32 y;


>>>>>>> upstream/master
  if(L_sub(val, 1000) < 0) {
    y = extract_l(((1000 * val) / (1000 + ((val * val) / ATAN_COEF1))));
  }
  else {
    y = PI2_1000 - ((1000 * val) / (ATAN_COEF2 + ((val * val) / 1000)));
  }

  return extract_l(y);
}


/*****************************************************************************
*
* function name: BarcLineValue
* description:  Calculates barc value for one frequency line
* returns:      barc value of line * BARC_SCALE
* input:        number of lines in transform, index of line to check, Fs
* output:
*
*****************************************************************************/
static Word16 BarcLineValue(Word16 noOfLines, Word16 fftLine, Word32 samplingFreq)
{
  Word32 center_freq, temp, bvalFFTLine;

  /* center frequency of fft line */
  center_freq = (fftLine * samplingFreq) / (noOfLines << 1);
  temp =  atan_1000((center_freq << 2) / (3*10));
<<<<<<< HEAD
  bvalFFTLine = 
    (26600 * atan_1000((center_freq*76) / 100) + 7*temp*temp) / (2*1000*1000 / BARC_SCALE);
  
=======
  bvalFFTLine =
    (26600 * atan_1000((center_freq*76) / 100) + 7*temp*temp) / (2*1000*1000 / BARC_SCALE);

>>>>>>> upstream/master
  return saturate(bvalFFTLine);
}

/*****************************************************************************
*
* function name: initThrQuiet
* description:  init thredhold in quiet
*
*****************************************************************************/
static void initThrQuiet(Word16  numPb,
                         Word16 *pbOffset,
                         Word16 *pbBarcVal,
                         Word32 *pbThresholdQuiet) {
  Word16 i;
  Word16 barcThrQuiet;

  for(i=0; i<numPb; i++) {
    Word16 bv1, bv2;

<<<<<<< HEAD
     
=======

>>>>>>> upstream/master
    if (i>0)
      bv1 = (pbBarcVal[i] + pbBarcVal[i-1]) >> 1;
    else
      bv1 = pbBarcVal[i] >> 1;

<<<<<<< HEAD
     
    if (i < (numPb - 1))
      bv2 = (pbBarcVal[i] + pbBarcVal[i+1]) >> 1;
    else {
      bv2 = pbBarcVal[i];                                        
=======

    if (i < (numPb - 1))
      bv2 = (pbBarcVal[i] + pbBarcVal[i+1]) >> 1;
    else {
      bv2 = pbBarcVal[i];
>>>>>>> upstream/master
    }

    bv1 = min((bv1 / BARC_SCALE), max_bark);
    bv2 = min((bv2 / BARC_SCALE), max_bark);

    barcThrQuiet = min(BARC_THR_QUIET[bv1], BARC_THR_QUIET[bv2]);

<<<<<<< HEAD
    
    /*
      we calculate 
=======

    /*
      we calculate
>>>>>>> upstream/master
      pow(10.0f,(float)(barcThrQuiet - ABS_LEV)*0.1)*(float)ABS_LOW*(pbOffset[i+1] - pbOffset[i]);
    */

    pbThresholdQuiet[i] = pow2_xy((((barcThrQuiet - ABS_LEV) * 100) +
                          LOG2_1000*(14+2*LOG_NORM_PCM)), LOG2_1000) * (pbOffset[i+1] - pbOffset[i]);
  }
}


/*****************************************************************************
*
* function name: initSpreading
* description:  init energy spreading parameter
*
*****************************************************************************/
static void initSpreading(Word16  numPb,
                          Word16 *pbBarcValue,
                          Word16 *pbMaskLoFactor,
                          Word16 *pbMaskHiFactor,
                          Word16 *pbMaskLoFactorSprEn,
                          Word16 *pbMaskHiFactorSprEn,
                          const Word32 bitrate,
                          const Word16 blockType)
{
  Word16 i;
  Word16 maskLowSprEn, maskHighSprEn;

<<<<<<< HEAD
   
  if (sub(blockType, SHORT_WINDOW) != 0) {
    maskLowSprEn = maskLowSprEnLong;                                     
       
=======

  if (sub(blockType, SHORT_WINDOW) != 0) {
    maskLowSprEn = maskLowSprEnLong;

>>>>>>> upstream/master
    if (bitrate > 22000)
      maskHighSprEn = maskHighSprEnLong;
    else
      maskHighSprEn = maskHighSprEnLongLowBr;
  }
  else {
<<<<<<< HEAD
    maskLowSprEn = maskLowSprEnShort;            
    maskHighSprEn = maskHighSprEnShort;          
  }

  for(i=0; i<numPb; i++) {
     
=======
    maskLowSprEn = maskLowSprEnShort;
    maskHighSprEn = maskHighSprEnShort;
  }

  for(i=0; i<numPb; i++) {

>>>>>>> upstream/master
    if (i > 0) {
      Word32 dbVal;
      Word16 dbark = pbBarcValue[i] - pbBarcValue[i-1];

      /*
<<<<<<< HEAD
        we calulate pow(10.0f, -0.1*dbVal/BARC_SCALE) 
      */
      dbVal = (maskHigh * dbark);
      pbMaskHiFactor[i] = round16(pow2_xy(L_negate(dbVal), (Word32)LOG2_1000));             /* 0.301 log10(2) */
       
      dbVal = (maskLow * dbark);
      pbMaskLoFactor[i-1] = round16(pow2_xy(L_negate(dbVal),(Word32)LOG2_1000)); 
       
      
      dbVal = (maskHighSprEn * dbark);
      pbMaskHiFactorSprEn[i] =  round16(pow2_xy(L_negate(dbVal),(Word32)LOG2_1000)); 
=======
        we calulate pow(10.0f, -0.1*dbVal/BARC_SCALE)
      */
      dbVal = (maskHigh * dbark);
      pbMaskHiFactor[i] = round16(pow2_xy(L_negate(dbVal), (Word32)LOG2_1000));             /* 0.301 log10(2) */

      dbVal = (maskLow * dbark);
      pbMaskLoFactor[i-1] = round16(pow2_xy(L_negate(dbVal),(Word32)LOG2_1000));


      dbVal = (maskHighSprEn * dbark);
      pbMaskHiFactorSprEn[i] =  round16(pow2_xy(L_negate(dbVal),(Word32)LOG2_1000));
>>>>>>> upstream/master
      dbVal = (maskLowSprEn * dbark);
      pbMaskLoFactorSprEn[i-1] = round16(pow2_xy(L_negate(dbVal),(Word32)LOG2_1000));
    }
    else {
<<<<<<< HEAD
      pbMaskHiFactor[i] = 0;                     
      pbMaskLoFactor[numPb-1] = 0;               

      pbMaskHiFactorSprEn[i] = 0;                
      pbMaskLoFactorSprEn[numPb-1] = 0;          
=======
      pbMaskHiFactor[i] = 0;
      pbMaskLoFactor[numPb-1] = 0;

      pbMaskHiFactorSprEn[i] = 0;
      pbMaskLoFactorSprEn[numPb-1] = 0;
>>>>>>> upstream/master
    }
  }

}


/*****************************************************************************
*
* function name: initBarcValues
* description:  init bark value
*
*****************************************************************************/
static void initBarcValues(Word16  numPb,
                           Word16 *pbOffset,
                           Word16  numLines,
                           Word32  samplingFrequency,
                           Word16 *pbBval)
{
  Word16 i;
  Word16 pbBval0, pbBval1;

<<<<<<< HEAD
  pbBval0 = 0;                                       
=======
  pbBval0 = 0;
>>>>>>> upstream/master

  for(i=0; i<numPb; i++){
    pbBval1 = BarcLineValue(numLines, pbOffset[i+1], samplingFrequency);
    pbBval[i] = (pbBval0 + pbBval1) >> 1;
<<<<<<< HEAD
    pbBval0 = pbBval1;                              
=======
    pbBval0 = pbBval1;
>>>>>>> upstream/master
  }
}


/*****************************************************************************
*
* function name: initMinSnr
* description:  calculate min snr parameter
*				minSnr(n) = 1/(2^sfbPemin(n)/w(n) - 1.5)
*
*****************************************************************************/
static void initMinSnr(const Word32  bitrate,
                       const Word32  samplerate,
                       const Word16  numLines,
                       const Word16 *sfbOffset,
                       const Word16 *pbBarcVal,
                       const Word16  sfbActive,
                       Word16       *sfbMinSnr)
{
  Word16 sfb;
  Word16 barcWidth;
  Word16 pePerWindow;
  Word32 pePart;
  Word32 snr;
  Word16 pbVal0, pbVal1, shift;

  /* relative number of active barks */


  pePerWindow = bits2pe(extract_l((bitrate * numLines) / samplerate));

<<<<<<< HEAD
  pbVal0 = 0;                                                    
=======
  pbVal0 = 0;
>>>>>>> upstream/master

  for (sfb=0; sfb<sfbActive; sfb++) {

    pbVal1 = (pbBarcVal[sfb] << 1) - pbVal0;
    barcWidth = pbVal1 - pbVal0;
<<<<<<< HEAD
    pbVal0 = pbVal1;                                             
=======
    pbVal0 = pbVal1;
>>>>>>> upstream/master

    /* allow at least 2.4% of pe for each active barc */
	pePart = ((pePerWindow * 24) * (max_bark * barcWidth)) /
        (pbBarcVal[sfbActive-1] * (sfbOffset[sfb+1] - sfbOffset[sfb]));
<<<<<<< HEAD
   
      
    pePart = min(pePart, 8400); 
=======


    pePart = min(pePart, 8400);
>>>>>>> upstream/master
    pePart = max(pePart, 1400);

    /* minSnr(n) = 1/(2^sfbPemin(n)/w(n) - 1.5)*/
	/* we add an offset of 2^16 to the pow functions */
	/* 0xc000 = 1.5*(1 << 15)*/
<<<<<<< HEAD
      
    snr = pow2_xy((pePart - 16*1000),1000) - 0x0000c000;
      
    if(snr > 0x00008000)
	{
		shift = norm_l(snr);
		snr = Div_32(0x00008000 << shift, snr << shift);  
=======

    snr = pow2_xy((pePart - 16*1000),1000) - 0x0000c000;

    if(snr > 0x00008000)
	{
		shift = norm_l(snr);
		snr = Div_32(0x00008000 << shift, snr << shift);
>>>>>>> upstream/master
	}
	else
	{
		snr = 0x7fffffff;
	}
<<<<<<< HEAD
      
=======

>>>>>>> upstream/master
    /* upper limit is -1 dB */
    snr = min(snr, c_maxsnr);
    /* lower limit is -25 dB */
    snr = max(snr, c_minsnr);
    sfbMinSnr[sfb] = round16(snr);
  }

}

/*****************************************************************************
*
* function name: InitPsyConfigurationLong
* description:  init long block psychoacoustic configuration
*
*****************************************************************************/
Word16 InitPsyConfigurationLong(Word32 bitrate,
                                Word32 samplerate,
                                Word16 bandwidth,
                                PSY_CONFIGURATION_LONG *psyConf)
{
  Word32 samplerateindex;
  Word16 sfbBarcVal[MAX_SFB_LONG];
  Word16 sfb;

  /*
    init sfb table
  */
<<<<<<< HEAD
  samplerateindex = GetSRIndex(samplerate);  
=======
  samplerateindex = GetSRIndex(samplerate);
>>>>>>> upstream/master
  psyConf->sfbCnt = sfBandTotalLong[samplerateindex];
  psyConf->sfbOffset = sfBandTabLong + sfBandTabLongOffset[samplerateindex];
  psyConf->sampRateIdx = samplerateindex;

  /*
    calculate barc values for each pb
  */
  initBarcValues(psyConf->sfbCnt,
                 psyConf->sfbOffset,
                 psyConf->sfbOffset[psyConf->sfbCnt],
                 samplerate,
                 sfbBarcVal);

  /*
    init thresholds in quiet
  */
  initThrQuiet(psyConf->sfbCnt,
               psyConf->sfbOffset,
               sfbBarcVal,
               psyConf->sfbThresholdQuiet);

  /*
    calculate spreading function
  */
  initSpreading(psyConf->sfbCnt,
                sfbBarcVal,
                psyConf->sfbMaskLowFactor,
                psyConf->sfbMaskHighFactor,
                psyConf->sfbMaskLowFactorSprEn,
                psyConf->sfbMaskHighFactorSprEn,
                bitrate,
                LONG_WINDOW);

  /*
    init ratio
  */
<<<<<<< HEAD
  psyConf->ratio = c_ratio;      

  psyConf->maxAllowedIncreaseFactor = 2;              
  psyConf->minRemainingThresholdFactor = c_minRemainingThresholdFactor;    /* 0.01 *(1 << 15)*/  

  psyConf->clipEnergy = c_maxClipEnergyLong;                   
=======
  psyConf->ratio = c_ratio;

  psyConf->maxAllowedIncreaseFactor = 2;
  psyConf->minRemainingThresholdFactor = c_minRemainingThresholdFactor;    /* 0.01 *(1 << 15)*/

  psyConf->clipEnergy = c_maxClipEnergyLong;
>>>>>>> upstream/master
  psyConf->lowpassLine = extract_l((bandwidth<<1) * FRAME_LEN_LONG / samplerate);

  for (sfb = 0; sfb < psyConf->sfbCnt; sfb++) {
    if (sub(psyConf->sfbOffset[sfb], psyConf->lowpassLine) >= 0)
      break;
  }
<<<<<<< HEAD
  psyConf->sfbActive = sfb;                 
=======
  psyConf->sfbActive = sfb;
>>>>>>> upstream/master

  /*
    calculate minSnr
  */
  initMinSnr(bitrate,
             samplerate,
             psyConf->sfbOffset[psyConf->sfbCnt],
             psyConf->sfbOffset,
             sfbBarcVal,
             psyConf->sfbActive,
             psyConf->sfbMinSnr);


  return(0);
}

/*****************************************************************************
*
* function name: InitPsyConfigurationShort
* description:  init short block psychoacoustic configuration
*
*****************************************************************************/
Word16 InitPsyConfigurationShort(Word32 bitrate,
                                 Word32 samplerate,
                                 Word16 bandwidth,
<<<<<<< HEAD
                                 PSY_CONFIGURATION_SHORT *psyConf) 
=======
                                 PSY_CONFIGURATION_SHORT *psyConf)
>>>>>>> upstream/master
{
  Word32 samplerateindex;
  Word16 sfbBarcVal[MAX_SFB_SHORT];
  Word16 sfb;
  /*
    init sfb table
  */
<<<<<<< HEAD
  samplerateindex = GetSRIndex(samplerate);  
=======
  samplerateindex = GetSRIndex(samplerate);
>>>>>>> upstream/master
  psyConf->sfbCnt = sfBandTotalShort[samplerateindex];
  psyConf->sfbOffset = sfBandTabShort + sfBandTabShortOffset[samplerateindex];
  psyConf->sampRateIdx = samplerateindex;
  /*
    calculate barc values for each pb
  */
  initBarcValues(psyConf->sfbCnt,
                 psyConf->sfbOffset,
                 psyConf->sfbOffset[psyConf->sfbCnt],
                 samplerate,
                 sfbBarcVal);

  /*
    init thresholds in quiet
  */
  initThrQuiet(psyConf->sfbCnt,
               psyConf->sfbOffset,
               sfbBarcVal,
               psyConf->sfbThresholdQuiet);

  /*
    calculate spreading function
  */
  initSpreading(psyConf->sfbCnt,
                sfbBarcVal,
                psyConf->sfbMaskLowFactor,
                psyConf->sfbMaskHighFactor,
                psyConf->sfbMaskLowFactorSprEn,
                psyConf->sfbMaskHighFactorSprEn,
                bitrate,
                SHORT_WINDOW);

  /*
    init ratio
  */
<<<<<<< HEAD
  psyConf->ratio = c_ratio;                                                      

  psyConf->maxAllowedIncreaseFactor = 2;                                         
  psyConf->minRemainingThresholdFactor = c_minRemainingThresholdFactor;                            	 

  psyConf->clipEnergy = c_maxClipEnergyShort;                                    

  psyConf->lowpassLine = extract_l(((bandwidth << 1) * FRAME_LEN_SHORT) / samplerate);
 
  for (sfb = 0; sfb < psyConf->sfbCnt; sfb++) {
     
    if (psyConf->sfbOffset[sfb] >= psyConf->lowpassLine)
      break;
  }
  psyConf->sfbActive = sfb;                                                      
=======
  psyConf->ratio = c_ratio;

  psyConf->maxAllowedIncreaseFactor = 2;
  psyConf->minRemainingThresholdFactor = c_minRemainingThresholdFactor;

  psyConf->clipEnergy = c_maxClipEnergyShort;

  psyConf->lowpassLine = extract_l(((bandwidth << 1) * FRAME_LEN_SHORT) / samplerate);

  for (sfb = 0; sfb < psyConf->sfbCnt; sfb++) {

    if (psyConf->sfbOffset[sfb] >= psyConf->lowpassLine)
      break;
  }
  psyConf->sfbActive = sfb;
>>>>>>> upstream/master

  /*
    calculate minSnr
  */
  initMinSnr(bitrate,
             samplerate,
             psyConf->sfbOffset[psyConf->sfbCnt],
             psyConf->sfbOffset,
             sfbBarcVal,
             psyConf->sfbActive,
             psyConf->sfbMinSnr);

  return(0);
}

