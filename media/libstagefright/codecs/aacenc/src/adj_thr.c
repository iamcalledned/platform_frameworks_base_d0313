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
	File:		adj_thr.c

	Content:	Threshold compensation functions

*******************************************************************************/

#include "basic_op.h"
#include "oper_32b.h"
#include "adj_thr_data.h"
#include "adj_thr.h"
#include "qc_data.h"
#include "line_pe.h"


#define  minSnrLimit    0x6666 /* 1 dB */
#define  PEBITS_COEF	0x170a /* 0.18*(1 << 15)*/

#define  HOLE_THR_LONG	0x2873	/* 0.316*(1 << 15) */
#define  HOLE_THR_SHORT 0x4000  /* 0.5  *(1 << 15) */

#define  MS_THRSPREAD_COEF 0x7333  /* 0.9 * (1 << 15) */

#define	 MIN_SNR_COEF	   0x651f  /* 3.16* (1 << (15 - 2)) */

/* values for avoid hole flag */
enum _avoid_hole_state {
  NO_AH              =0,
  AH_INACTIVE        =1,
  AH_ACTIVE          =2
};

/********************************************************************************
*
* function name:bits2pe
* description: convert from bits to pe
*			   pe = 1.18*desiredBits
*
**********************************************************************************/
Word16 bits2pe(const Word16 bits) {
  return (bits + ((PEBITS_COEF * bits) >> 15));
}

/********************************************************************************
*
* function name:calcThreshExp
* description: loudness calculation (threshold to the power of redExp)
*			   thr(n)^0.25
*
**********************************************************************************/
static void calcThreshExp(Word32 thrExp[MAX_CHANNELS][MAX_GROUPED_SFB],
                          PSY_OUT_CHANNEL psyOutChannel[MAX_CHANNELS],
                          const Word16 nChannels)
{
  Word16 ch, sfb, sfbGrp;
  Word32 *pthrExp, *psfbThre;
  for (ch=0; ch<nChannels; ch++) {
    PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
<<<<<<< HEAD
	 for(sfbGrp = 0; sfbGrp < psyOutChan->sfbCnt; sfbGrp+= psyOutChan->sfbPerGroup)	 
=======
	 for(sfbGrp = 0; sfbGrp < psyOutChan->sfbCnt; sfbGrp+= psyOutChan->sfbPerGroup)
>>>>>>> upstream/master
	  pthrExp = &(thrExp[ch][sfbGrp]);
	  psfbThre = psyOutChan->sfbThreshold + sfbGrp;
	  for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
		*pthrExp = rsqrt(rsqrt(*psfbThre,INT_BITS),INT_BITS);
		pthrExp++; psfbThre++;
      }
  }
}

/********************************************************************************
*
* function name:adaptMinSnr
* description: reduce minSnr requirements for bands with relative low energies
*
**********************************************************************************/
static void adaptMinSnr(PSY_OUT_CHANNEL     psyOutChannel[MAX_CHANNELS],
                        Word16              logSfbEnergy[MAX_CHANNELS][MAX_GROUPED_SFB],
                        MINSNR_ADAPT_PARAM *msaParam,
                        const Word16        nChannels)
{
  Word16 ch, sfb, sfbOffs, shift;
  Word32 nSfb, avgEn;
  Word16 log_avgEn = 0;
  Word32 startRatio_x_avgEn = 0;
<<<<<<< HEAD
                                                                           
=======

>>>>>>> upstream/master

  for (ch=0; ch<nChannels; ch++) {
    PSY_OUT_CHANNEL* psyOutChan = &psyOutChannel[ch];

    /* calc average energy per scalefactor band */
<<<<<<< HEAD
    avgEn = 0;                                                           
    nSfb = 0;                                                            
=======
    avgEn = 0;
    nSfb = 0;
>>>>>>> upstream/master
    for (sfbOffs=0; sfbOffs<psyOutChan->sfbCnt; sfbOffs+=psyOutChan->sfbPerGroup) {
      for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
        avgEn = L_add(avgEn, psyOutChan->sfbEnergy[sfbOffs+sfb]);
        nSfb = nSfb + 1;
      }
    }
<<<<<<< HEAD
     
=======

>>>>>>> upstream/master
    if (nSfb > 0) {
	  avgEn = avgEn / nSfb;

      log_avgEn = iLog4(avgEn);
      startRatio_x_avgEn = fixmul(msaParam->startRatio, avgEn);
    }

<<<<<<< HEAD
    
=======

>>>>>>> upstream/master
    /* reduce minSnr requirement by minSnr^minSnrRed dependent on avgEn/sfbEn */
    for (sfbOffs=0; sfbOffs<psyOutChan->sfbCnt; sfbOffs+=psyOutChan->sfbPerGroup) {
      for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
        if (psyOutChan->sfbEnergy[sfbOffs+sfb] < startRatio_x_avgEn) {
          Word16 dbRatio, minSnrRed;
          Word32 snrRed;
          Word16 newMinSnr;
<<<<<<< HEAD
          
=======

>>>>>>> upstream/master
          dbRatio = log_avgEn - logSfbEnergy[ch][sfbOffs+sfb];
          dbRatio = dbRatio + (dbRatio << 1);

          minSnrRed = 110 - ((dbRatio + (dbRatio << 1)) >> 2);
<<<<<<< HEAD
          minSnrRed = max(minSnrRed, 20); /* 110: (0.375(redOffs)+1)*80,  
                                               3: 0.00375(redRatioFac)*80
                                               20: 0.25(maxRed) * 80 */

          snrRed = minSnrRed * iLog4((psyOutChan->sfbMinSnr[sfbOffs+sfb] << 16)); 
          /* 
             snrRedI si now scaled by 80 (minSnrRed) and 4 (ffr_iLog4)
          */
        
          newMinSnr = round16(pow2_xy(snrRed,80*4));
         
=======
          minSnrRed = max(minSnrRed, 20); /* 110: (0.375(redOffs)+1)*80,
                                               3: 0.00375(redRatioFac)*80
                                               20: 0.25(maxRed) * 80 */

          snrRed = minSnrRed * iLog4((psyOutChan->sfbMinSnr[sfbOffs+sfb] << 16));
          /*
             snrRedI si now scaled by 80 (minSnrRed) and 4 (ffr_iLog4)
          */

          newMinSnr = round16(pow2_xy(snrRed,80*4));

>>>>>>> upstream/master
          psyOutChan->sfbMinSnr[sfbOffs+sfb] = min(newMinSnr, minSnrLimit);
        }
      }
    }
  }

}


/********************************************************************************
*
* function name:initAvoidHoleFlag
* description: determine bands where avoid hole is not necessary resp. possible
*
**********************************************************************************/
static void initAvoidHoleFlag(Word16 ahFlag[MAX_CHANNELS][MAX_GROUPED_SFB],
                              PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS],
                              PSY_OUT_ELEMENT* psyOutElement,
                              const Word16 nChannels,
                              AH_PARAM *ahParam)
{
  Word16 ch, sfb, sfbGrp, shift;
  Word32 threshold;
  Word32* psfbSpreadEn;

  for (ch=0; ch<nChannels; ch++) {
    PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
<<<<<<< HEAD
     
=======

>>>>>>> upstream/master
    if (psyOutChan->windowSequence != SHORT_WINDOW) {
      for(sfbGrp = 0;sfbGrp < psyOutChan->sfbCnt;sfbGrp+= psyOutChan->sfbPerGroup){
         psfbSpreadEn = psyOutChan->sfbSpreadedEnergy + sfbGrp;
		 for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
			*psfbSpreadEn = *psfbSpreadEn >> 1;  /* 0.5 */
			++psfbSpreadEn;
        }
      }
    }
    else {
      for(sfbGrp = 0;sfbGrp < psyOutChan->sfbCnt;sfbGrp+= psyOutChan->sfbPerGroup){
		psfbSpreadEn = psyOutChan->sfbSpreadedEnergy + sfbGrp;
        for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
          *psfbSpreadEn = (*psfbSpreadEn >> 1) + (*psfbSpreadEn >> 3);  /* 0.63 */
		  ++psfbSpreadEn;
        }
      }
    }
  }

  /* increase minSnr for local peaks, decrease it for valleys */
  if (ahParam->modifyMinSnr) {
    for(ch=0; ch<nChannels; ch++) {
      PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
<<<<<<< HEAD
         
=======

>>>>>>> upstream/master
      if (psyOutChan->windowSequence != SHORT_WINDOW)
        threshold = HOLE_THR_LONG;
      else
        threshold = HOLE_THR_SHORT;

      for(sfbGrp = 0;sfbGrp < psyOutChan->sfbCnt;sfbGrp+= psyOutChan->sfbPerGroup){
        Word16 *psfbMinSnr = psyOutChan->sfbMinSnr + sfbGrp;
		for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
          Word32 sfbEn, sfbEnm1, sfbEnp1, avgEn;
<<<<<<< HEAD
             
=======

>>>>>>> upstream/master
          if (sfb > 0)
            sfbEnm1 = psyOutChan->sfbEnergy[sfbGrp+sfb-1];
          else
            sfbEnm1 = psyOutChan->sfbEnergy[sfbGrp];
<<<<<<< HEAD
             
=======

>>>>>>> upstream/master
          if (sfb < (psyOutChan->maxSfbPerGroup-1))
            sfbEnp1 = psyOutChan->sfbEnergy[sfbGrp+sfb+1];
          else
            sfbEnp1 = psyOutChan->sfbEnergy[sfbGrp+sfb];
          avgEn = (sfbEnm1 + sfbEnp1) >> 1;
<<<<<<< HEAD
          sfbEn = psyOutChan->sfbEnergy[sfbGrp+sfb];                             
             
=======
          sfbEn = psyOutChan->sfbEnergy[sfbGrp+sfb];

>>>>>>> upstream/master
          if (sfbEn > avgEn && avgEn > 0) {
            Word32 tmpMinSnr;
            shift = norm_l(sfbEn);
			tmpMinSnr = Div_32(L_mpy_ls(avgEn, minSnrLimit) << shift, sfbEn << shift );
<<<<<<< HEAD
            tmpMinSnr = max(tmpMinSnr, HOLE_THR_LONG);                  
=======
            tmpMinSnr = max(tmpMinSnr, HOLE_THR_LONG);
>>>>>>> upstream/master
            tmpMinSnr = max(tmpMinSnr, threshold);
            *psfbMinSnr = min(*psfbMinSnr, tmpMinSnr);
          }
          /* valley ? */
<<<<<<< HEAD
             
          if ((sfbEn < (avgEn >> 1)) && (sfbEn > 0)) {
            Word32 tmpMinSnr;
            Word32 minSnrEn = L_mpy_wx(avgEn, *psfbMinSnr);                 
             
=======

          if ((sfbEn < (avgEn >> 1)) && (sfbEn > 0)) {
            Word32 tmpMinSnr;
            Word32 minSnrEn = L_mpy_wx(avgEn, *psfbMinSnr);

>>>>>>> upstream/master
            if(minSnrEn < sfbEn) {
			  shift = norm_l(sfbEn);
              tmpMinSnr = Div_32( minSnrEn << shift, sfbEn<<shift);
            }
            else {
<<<<<<< HEAD
              tmpMinSnr = MAX_16;                                             
=======
              tmpMinSnr = MAX_16;
>>>>>>> upstream/master
            }
            tmpMinSnr = min(minSnrLimit, tmpMinSnr);

            *psfbMinSnr =
              (min((tmpMinSnr >>  2), mult(*psfbMinSnr, MIN_SNR_COEF)) << 2);
          }
		  psfbMinSnr++;
        }
      }
    }
  }

  /* stereo: adapt the minimum requirements sfbMinSnr of mid and
     side channels */
<<<<<<< HEAD
   
=======

>>>>>>> upstream/master
  if (nChannels == 2) {
    PSY_OUT_CHANNEL *psyOutChanM = &psyOutChannel[0];
    PSY_OUT_CHANNEL *psyOutChanS = &psyOutChannel[1];
    for (sfb=0; sfb<psyOutChanM->sfbCnt; sfb++) {
      if (psyOutElement->toolsInfo.msMask[sfb]) {
        Word32 sfbEnM = psyOutChanM->sfbEnergy[sfb];
        Word32 sfbEnS = psyOutChanS->sfbEnergy[sfb];
        Word32 maxSfbEn = max(sfbEnM, sfbEnS);
<<<<<<< HEAD
        Word32 maxThr = L_mpy_wx(maxSfbEn, psyOutChanM->sfbMinSnr[sfb]) >> 1;        
         
        if(maxThr >= sfbEnM) {
          psyOutChanM->sfbMinSnr[sfb] = MAX_16;                                          
        }
        else {
          shift = norm_l(sfbEnM); 
		  psyOutChanM->sfbMinSnr[sfb] = min(max(psyOutChanM->sfbMinSnr[sfb], 
			  round16(Div_32(maxThr<<shift, sfbEnM << shift))), minSnrLimit);
        }
         
=======
        Word32 maxThr = L_mpy_wx(maxSfbEn, psyOutChanM->sfbMinSnr[sfb]) >> 1;

        if(maxThr >= sfbEnM) {
          psyOutChanM->sfbMinSnr[sfb] = MAX_16;
        }
        else {
          shift = norm_l(sfbEnM);
		  psyOutChanM->sfbMinSnr[sfb] = min(max(psyOutChanM->sfbMinSnr[sfb],
			  round16(Div_32(maxThr<<shift, sfbEnM << shift))), minSnrLimit);
        }

>>>>>>> upstream/master
        if(maxThr >= sfbEnS) {
          psyOutChanS->sfbMinSnr[sfb] = MAX_16;
        }
        else {
		  shift = norm_l(sfbEnS);
<<<<<<< HEAD
          psyOutChanS->sfbMinSnr[sfb] = min(max(psyOutChanS->sfbMinSnr[sfb], 
			  round16(Div_32(maxThr << shift, sfbEnS << shift))), minSnrLimit);
        }

         
        if (sfbEnM > psyOutChanM->sfbSpreadedEnergy[sfb])
          psyOutChanS->sfbSpreadedEnergy[sfb] = L_mpy_ls(sfbEnS, MS_THRSPREAD_COEF);
         
=======
          psyOutChanS->sfbMinSnr[sfb] = min(max(psyOutChanS->sfbMinSnr[sfb],
			  round16(Div_32(maxThr << shift, sfbEnS << shift))), minSnrLimit);
        }


        if (sfbEnM > psyOutChanM->sfbSpreadedEnergy[sfb])
          psyOutChanS->sfbSpreadedEnergy[sfb] = L_mpy_ls(sfbEnS, MS_THRSPREAD_COEF);

>>>>>>> upstream/master
        if (sfbEnS > psyOutChanS->sfbSpreadedEnergy[sfb])
          psyOutChanM->sfbSpreadedEnergy[sfb] = L_mpy_ls(sfbEnM, MS_THRSPREAD_COEF);
      }
    }
  }


  /* init ahFlag (0: no ah necessary, 1: ah possible, 2: ah active */
  for(ch=0; ch<nChannels; ch++) {
    PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
    for(sfbGrp = 0;sfbGrp < psyOutChan->sfbCnt;sfbGrp+= psyOutChan->sfbPerGroup){
      Word16 *pahFlag = ahFlag[ch] + sfbGrp;
	  for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
<<<<<<< HEAD
               
=======

>>>>>>> upstream/master
        if ((psyOutChan->sfbSpreadedEnergy[sfbGrp+sfb] > psyOutChan->sfbEnergy[sfbGrp+sfb]) ||
            (psyOutChan->sfbEnergy[sfbGrp+sfb] <= psyOutChan->sfbThreshold[sfbGrp+sfb]) ||
            (psyOutChan->sfbMinSnr[sfbGrp+sfb] == MAX_16)) {
          *pahFlag++ = NO_AH;
        }
        else {
          *pahFlag++ = AH_INACTIVE;
        }
      }
      for (sfb=psyOutChan->maxSfbPerGroup; sfb<psyOutChan->sfbPerGroup; sfb++) {
<<<<<<< HEAD
        *pahFlag++ = NO_AH;                                                          
=======
        *pahFlag++ = NO_AH;
>>>>>>> upstream/master
      }
    }
  }
}

/********************************************************************************
*
* function name:calcPeNoAH
* description: sum the pe data only for bands where avoid hole is inactive
*
**********************************************************************************/
static void calcPeNoAH(Word16          *pe,
                       Word16          *constPart,
                       Word16          *nActiveLines,
                       PE_DATA         *peData,
                       Word16           ahFlag[MAX_CHANNELS][MAX_GROUPED_SFB],
                       PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS],
                       const Word16     nChannels)
{
  Word16 ch, sfb, sfbGrp;
  int ipe, iconstPart, inActiveLines;

<<<<<<< HEAD
  ipe = 0;                                                       
  iconstPart = 0;                                                
  inActiveLines = 0;                                             
=======
  ipe = 0;
  iconstPart = 0;
  inActiveLines = 0;
>>>>>>> upstream/master
  for(ch=0; ch<nChannels; ch++) {
    PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
    PE_CHANNEL_DATA *peChanData = &peData->peChannelData[ch];
    for(sfbGrp = 0;sfbGrp < psyOutChan->sfbCnt;sfbGrp+= psyOutChan->sfbPerGroup){
      for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
<<<<<<< HEAD
         
=======

>>>>>>> upstream/master
        if (ahFlag[ch][sfbGrp+sfb] < AH_ACTIVE) {
          ipe = ipe + peChanData->sfbPe[sfbGrp+sfb];
          iconstPart = iconstPart + peChanData->sfbConstPart[sfbGrp+sfb];
          inActiveLines = inActiveLines + peChanData->sfbNActiveLines[sfbGrp+sfb];
        }
      }
    }
  }

<<<<<<< HEAD
  *pe = saturate(ipe);                                                       
  *constPart = saturate(iconstPart);                                                
  *nActiveLines = saturate(inActiveLines);  
=======
  *pe = saturate(ipe);
  *constPart = saturate(iconstPart);
  *nActiveLines = saturate(inActiveLines);
>>>>>>> upstream/master
}

/********************************************************************************
*
* function name:reduceThresholds
* description: apply reduction formula
*
**********************************************************************************/
static void reduceThresholds(PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS],
                             Word16           ahFlag[MAX_CHANNELS][MAX_GROUPED_SFB],
                             Word32           thrExp[MAX_CHANNELS][MAX_GROUPED_SFB],
                             const Word16     nChannels,
                             const Word32     redVal)
{
  Word32 sfbThrReduced;
<<<<<<< HEAD
  Word32 *psfbEn, *psfbThr;     
=======
  Word32 *psfbEn, *psfbThr;
>>>>>>> upstream/master
  Word16 ch, sfb, sfbGrp;

  for(ch=0; ch<nChannels; ch++) {
    PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
    for(sfbGrp=0; sfbGrp<psyOutChan->sfbCnt; sfbGrp+=psyOutChan->sfbPerGroup) {
<<<<<<< HEAD
 	  psfbEn  = psyOutChan->sfbEnergy + sfbGrp;                                      
      psfbThr = psyOutChan->sfbThreshold + sfbGrp;
	  for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
         
=======
 	  psfbEn  = psyOutChan->sfbEnergy + sfbGrp;
      psfbThr = psyOutChan->sfbThreshold + sfbGrp;
	  for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {

>>>>>>> upstream/master
        if (*psfbEn > *psfbThr) {
          /* threshold reduction formula */
          Word32 tmp = thrExp[ch][sfbGrp+sfb] + redVal;
          tmp = fixmul(tmp, tmp);
          sfbThrReduced = fixmul(tmp, tmp);
          /* avoid holes */
          tmp = L_mpy_ls(*psfbEn, psyOutChan->sfbMinSnr[sfbGrp+sfb]);
<<<<<<< HEAD
             
          if ((sfbThrReduced > tmp) && 
              (ahFlag[ch][sfbGrp+sfb] != NO_AH)){
            sfbThrReduced = max(tmp, *psfbThr);
            ahFlag[ch][sfbGrp+sfb] = AH_ACTIVE;                                          
=======

          if ((sfbThrReduced > tmp) &&
              (ahFlag[ch][sfbGrp+sfb] != NO_AH)){
            sfbThrReduced = max(tmp, *psfbThr);
            ahFlag[ch][sfbGrp+sfb] = AH_ACTIVE;
>>>>>>> upstream/master
          }
		  *psfbThr = sfbThrReduced;
        }

		psfbEn++;  psfbThr++;
      }
    }
  }
}


/********************************************************************************
*
* function name:correctThresh
* description: if pe difference deltaPe between desired pe and real pe is small enough,
*             the difference can be distributed among the scale factor bands.
*
**********************************************************************************/
static void correctThresh(PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS],
                          Word16           ahFlag[MAX_CHANNELS][MAX_GROUPED_SFB],
                          PE_DATA          *peData,
                          Word32           thrExp[MAX_CHANNELS][MAX_GROUPED_SFB],
                          const Word32     redVal,
                          const Word16     nChannels,
                          const Word32     deltaPe)
{
  Word16 ch, sfb, sfbGrp,shift;
  PSY_OUT_CHANNEL *psyOutChan;
  PE_CHANNEL_DATA *peChanData;
  Word32 deltaSfbPe;
  Word32 normFactor;
  Word32 *psfbPeFactors;
  Word16 *psfbNActiveLines, *pahFlag;
  Word32 sfbEn, sfbThr;
  Word32 sfbThrReduced;

  /* for each sfb calc relative factors for pe changes */
<<<<<<< HEAD
  normFactor = 1;                                                                        
=======
  normFactor = 1;
>>>>>>> upstream/master
  for(ch=0; ch<nChannels; ch++) {
    psyOutChan = &psyOutChannel[ch];
    peChanData = &peData->peChannelData[ch];
    for(sfbGrp = 0;sfbGrp < psyOutChan->sfbCnt;sfbGrp+= psyOutChan->sfbPerGroup){
      psfbPeFactors = peData->sfbPeFactors[ch] + sfbGrp;
	  psfbNActiveLines = peChanData->sfbNActiveLines + sfbGrp;
	  pahFlag = ahFlag[ch] + sfbGrp;
	  for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
        Word32 redThrExp = thrExp[ch][sfbGrp+sfb] + redVal;
<<<<<<< HEAD
             
        if (((*pahFlag < AH_ACTIVE) || (deltaPe > 0)) && (redThrExp > 0) ) {
            
=======

        if (((*pahFlag < AH_ACTIVE) || (deltaPe > 0)) && (redThrExp > 0) ) {

>>>>>>> upstream/master
          *psfbPeFactors = (*psfbNActiveLines) * (0x7fffffff / redThrExp);
          normFactor = L_add(normFactor, *psfbPeFactors);
        }
        else {
<<<<<<< HEAD
          *psfbPeFactors = 0;                                              
        }
		psfbPeFactors++; 
=======
          *psfbPeFactors = 0;
        }
		psfbPeFactors++;
>>>>>>> upstream/master
		pahFlag++; psfbNActiveLines++;
      }
    }
  }

<<<<<<< HEAD
 
=======

>>>>>>> upstream/master
  /* calculate new thresholds */
  for(ch=0; ch<nChannels; ch++) {
    psyOutChan = &psyOutChannel[ch];
    peChanData = &peData->peChannelData[ch];
    for(sfbGrp = 0;sfbGrp < psyOutChan->sfbCnt;sfbGrp+= psyOutChan->sfbPerGroup){
      psfbPeFactors = peData->sfbPeFactors[ch] + sfbGrp;
	  psfbNActiveLines = peChanData->sfbNActiveLines + sfbGrp;
	  pahFlag = ahFlag[ch] + sfbGrp;
	  for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
        /* pe difference for this sfb */
        deltaSfbPe = *psfbPeFactors * deltaPe;

<<<<<<< HEAD
		/* thr3(n) = thr2(n)*2^deltaSfbPe/b(n) */         
=======
		/* thr3(n) = thr2(n)*2^deltaSfbPe/b(n) */
>>>>>>> upstream/master
        if (*psfbNActiveLines > 0) {
          /* new threshold */
          Word32 thrFactor;
          sfbEn  = psyOutChan->sfbEnergy[sfbGrp+sfb];
          sfbThr = psyOutChan->sfbThreshold[sfbGrp+sfb];

           if(deltaSfbPe >= 0){
            /*
              reduce threshold
            */
            thrFactor = pow2_xy(L_negate(deltaSfbPe), (normFactor* (*psfbNActiveLines)));
<<<<<<< HEAD
              
=======

>>>>>>> upstream/master
            sfbThrReduced = L_mpy_ls(sfbThr, round16(thrFactor));
          }
          else {
            /*
              increase threshold
            */
            thrFactor = pow2_xy(deltaSfbPe, (normFactor * (*psfbNActiveLines)));
<<<<<<< HEAD
              
             
=======


>>>>>>> upstream/master
            if(thrFactor > sfbThr) {
              shift = norm_l(thrFactor);
			  sfbThrReduced = Div_32( sfbThr << shift, thrFactor<<shift );
            }
            else {
<<<<<<< HEAD
              sfbThrReduced = MAX_32;                                                                            
            }

          }
            
          /* avoid hole */
          sfbEn = L_mpy_ls(sfbEn, psyOutChan->sfbMinSnr[sfbGrp+sfb]);
             
          if ((sfbThrReduced > sfbEn) &&
              (*pahFlag == AH_INACTIVE)) {
            sfbThrReduced = max(sfbEn, sfbThr);
            *pahFlag = AH_ACTIVE;                                                                  
          }

          psyOutChan->sfbThreshold[sfbGrp+sfb] = sfbThrReduced;  
=======
              sfbThrReduced = MAX_32;
            }

          }

          /* avoid hole */
          sfbEn = L_mpy_ls(sfbEn, psyOutChan->sfbMinSnr[sfbGrp+sfb]);

          if ((sfbThrReduced > sfbEn) &&
              (*pahFlag == AH_INACTIVE)) {
            sfbThrReduced = max(sfbEn, sfbThr);
            *pahFlag = AH_ACTIVE;
          }

          psyOutChan->sfbThreshold[sfbGrp+sfb] = sfbThrReduced;
>>>>>>> upstream/master
        }

		pahFlag++; psfbNActiveLines++; psfbPeFactors++;
      }
    }
  }
}


/********************************************************************************
*
* function name:reduceMinSnr
* description: if the desired pe can not be reached, reduce pe by reducing minSnr
*
**********************************************************************************/
<<<<<<< HEAD
static void reduceMinSnr(PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS], 
                         PE_DATA         *peData, 
=======
static void reduceMinSnr(PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS],
                         PE_DATA         *peData,
>>>>>>> upstream/master
                         Word16           ahFlag[MAX_CHANNELS][MAX_GROUPED_SFB],
                         const Word16     nChannels,
                         const Word16     desiredPe)
{
  Word16 ch, sfb, sfbSubWin;
  Word16 deltaPe;

  /* start at highest freq down to 0 */
<<<<<<< HEAD
  sfbSubWin = psyOutChannel[0].maxSfbPerGroup;                                                 
  while (peData->pe > desiredPe && sfbSubWin > 0) {
       
=======
  sfbSubWin = psyOutChannel[0].maxSfbPerGroup;
  while (peData->pe > desiredPe && sfbSubWin > 0) {

>>>>>>> upstream/master
    sfbSubWin = sfbSubWin - 1;
    /* loop over all subwindows */
    for (sfb=sfbSubWin; sfb<psyOutChannel[0].sfbCnt;
        sfb+=psyOutChannel[0].sfbPerGroup) {
      /* loop over all channels */
		PE_CHANNEL_DATA* peChan = peData->peChannelData;
		PSY_OUT_CHANNEL* psyOutCh = psyOutChannel;
<<<<<<< HEAD
		for (ch=0; ch<nChannels; ch++) {           
        if (ahFlag[ch][sfb] != NO_AH &&
            psyOutCh->sfbMinSnr[sfb] < minSnrLimit) {
          psyOutCh->sfbMinSnr[sfb] = minSnrLimit;                                      
=======
		for (ch=0; ch<nChannels; ch++) {
        if (ahFlag[ch][sfb] != NO_AH &&
            psyOutCh->sfbMinSnr[sfb] < minSnrLimit) {
          psyOutCh->sfbMinSnr[sfb] = minSnrLimit;
>>>>>>> upstream/master
          psyOutCh->sfbThreshold[sfb] =
            L_mpy_ls(psyOutCh->sfbEnergy[sfb], psyOutCh->sfbMinSnr[sfb]);

          /* calc new pe */
          deltaPe = ((peChan->sfbNLines4[sfb] + (peChan->sfbNLines4[sfb] >> 1)) >> 2) -
              peChan->sfbPe[sfb];
          peData->pe = peData->pe + deltaPe;
<<<<<<< HEAD
          peChan->pe = peChan->pe + deltaPe;		  
=======
          peChan->pe = peChan->pe + deltaPe;
>>>>>>> upstream/master
        }
		peChan += 1; psyOutCh += 1;
      }
      /* stop if enough has been saved */
<<<<<<< HEAD
       
=======

>>>>>>> upstream/master
      if (peData->pe <= desiredPe)
        break;
    }
  }
}

/********************************************************************************
*
* function name:allowMoreHoles
<<<<<<< HEAD
* description: if the desired pe can not be reached, some more scalefactor bands  
*              have to be quantized to zero
*
**********************************************************************************/
static void allowMoreHoles(PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS], 
                           PSY_OUT_ELEMENT *psyOutElement,
                           PE_DATA         *peData, 
=======
* description: if the desired pe can not be reached, some more scalefactor bands
*              have to be quantized to zero
*
**********************************************************************************/
static void allowMoreHoles(PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS],
                           PSY_OUT_ELEMENT *psyOutElement,
                           PE_DATA         *peData,
>>>>>>> upstream/master
                           Word16           ahFlag[MAX_CHANNELS][MAX_GROUPED_SFB],
                           const AH_PARAM  *ahParam,
                           const Word16     nChannels,
                           const Word16     desiredPe)
{
  Word16 ch, sfb;
  Word16 actPe, shift;

<<<<<<< HEAD
  actPe = peData->pe;                                                                    

  /* for MS allow hole in the channel with less energy */
     
=======
  actPe = peData->pe;

  /* for MS allow hole in the channel with less energy */

>>>>>>> upstream/master
  if (nChannels==2 &&
      psyOutChannel[0].windowSequence==psyOutChannel[1].windowSequence) {
    PSY_OUT_CHANNEL *psyOutChanL = &psyOutChannel[0];
    PSY_OUT_CHANNEL *psyOutChanR = &psyOutChannel[1];
    for (sfb=0; sfb<psyOutChanL->sfbCnt; sfb++) {
      Word32 minEn;
<<<<<<< HEAD
       
      if (psyOutElement->toolsInfo.msMask[sfb]) {
        /* allow hole in side channel ? */
        minEn = L_mpy_ls(psyOutChanL->sfbEnergy[sfb], (minSnrLimit * psyOutChanL->sfbMinSnr[sfb]) >> 16);
           
        if (ahFlag[1][sfb] != NO_AH &&
            minEn > psyOutChanR->sfbEnergy[sfb]) {
          ahFlag[1][sfb] = NO_AH;                                                                
=======

      if (psyOutElement->toolsInfo.msMask[sfb]) {
        /* allow hole in side channel ? */
        minEn = L_mpy_ls(psyOutChanL->sfbEnergy[sfb], (minSnrLimit * psyOutChanL->sfbMinSnr[sfb]) >> 16);

        if (ahFlag[1][sfb] != NO_AH &&
            minEn > psyOutChanR->sfbEnergy[sfb]) {
          ahFlag[1][sfb] = NO_AH;
>>>>>>> upstream/master
          psyOutChanR->sfbThreshold[sfb] = L_add(psyOutChanR->sfbEnergy[sfb], psyOutChanR->sfbEnergy[sfb]);
          actPe = actPe - peData->peChannelData[1].sfbPe[sfb];
        }
        /* allow hole in mid channel ? */
        else {
        minEn = L_mpy_ls(psyOutChanR->sfbEnergy[sfb], (minSnrLimit * psyOutChanR->sfbMinSnr[sfb]) >> 16);
<<<<<<< HEAD
             
          if (ahFlag[0][sfb]!= NO_AH &&
              minEn > psyOutChanL->sfbEnergy[sfb]) {
            ahFlag[0][sfb] = NO_AH;                                                              
=======

          if (ahFlag[0][sfb]!= NO_AH &&
              minEn > psyOutChanL->sfbEnergy[sfb]) {
            ahFlag[0][sfb] = NO_AH;
>>>>>>> upstream/master
            psyOutChanL->sfbThreshold[sfb] = L_add(psyOutChanL->sfbEnergy[sfb], psyOutChanL->sfbEnergy[sfb]);
            actPe = actPe - peData->peChannelData[0].sfbPe[sfb];
          }
        }
<<<<<<< HEAD
         
=======

>>>>>>> upstream/master
        if (actPe < desiredPe)
          break;
      }
    }
  }

<<<<<<< HEAD
  /* subsequently erase bands */   
=======
  /* subsequently erase bands */
>>>>>>> upstream/master
  if (actPe > desiredPe) {
    Word16 startSfb[2];
    Word32 avgEn, minEn;
    Word16 ahCnt;
    Word16 enIdx;
    Word16 enDiff;
    Word32 en[4];
    Word16 minSfb, maxSfb;
    Flag   done;

    /* do not go below startSfb */
    for (ch=0; ch<nChannels; ch++) {
<<<<<<< HEAD
         
=======

>>>>>>> upstream/master
      if (psyOutChannel[ch].windowSequence != SHORT_WINDOW)
        startSfb[ch] = ahParam->startSfbL;
      else
        startSfb[ch] = ahParam->startSfbS;
    }

<<<<<<< HEAD
    avgEn = 0;                                                           
    minEn = MAX_32;                                                      
    ahCnt = 0;                                                           
    for (ch=0; ch<nChannels; ch++) {
      PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
      for (sfb=startSfb[ch]; sfb<psyOutChan->sfbCnt; sfb++) {
           
=======
    avgEn = 0;
    minEn = MAX_32;
    ahCnt = 0;
    for (ch=0; ch<nChannels; ch++) {
      PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
      for (sfb=startSfb[ch]; sfb<psyOutChan->sfbCnt; sfb++) {

>>>>>>> upstream/master
        if ((ahFlag[ch][sfb] != NO_AH) &&
            (psyOutChan->sfbEnergy[sfb] > psyOutChan->sfbThreshold[sfb])) {
          minEn = min(minEn, psyOutChan->sfbEnergy[sfb]);
          avgEn = L_add(avgEn, psyOutChan->sfbEnergy[sfb]);
          ahCnt++;
        }
      }
    }
<<<<<<< HEAD
     
=======

>>>>>>> upstream/master
    if(ahCnt) {
      Word32 iahCnt;
      shift = norm_l(ahCnt);
	  iahCnt = Div_32( 1 << shift, ahCnt << shift );
      avgEn = fixmul(avgEn, iahCnt);
    }

    enDiff = iLog4(avgEn) - iLog4(minEn);
    /* calc some energy borders between minEn and avgEn */
    for (enIdx=0; enIdx<4; enIdx++) {
      Word32 enFac;
      enFac = ((6-(enIdx << 1)) * enDiff);
      en[enIdx] = fixmul(avgEn, pow2_xy(L_negate(enFac),7*4));
    }

    /* start with lowest energy border at highest sfb */
    maxSfb = psyOutChannel[0].sfbCnt - 1;
<<<<<<< HEAD
    minSfb = startSfb[0];                                                                
     
=======
    minSfb = startSfb[0];

>>>>>>> upstream/master
    if (nChannels == 2) {
      maxSfb = max(maxSfb, (psyOutChannel[1].sfbCnt - 1));
      minSfb = min(minSfb, startSfb[1]);
    }

<<<<<<< HEAD
    sfb = maxSfb;                                                                        
    enIdx = 0;                                                                           
    done = 0;                                                                            
    while (!done) {
       
      for (ch=0; ch<nChannels; ch++) {
        PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];
           
        if (sfb>=startSfb[ch] && sfb<psyOutChan->sfbCnt) {
          /* sfb energy below border ? */
             
          if (ahFlag[ch][sfb] != NO_AH && psyOutChan->sfbEnergy[sfb] < en[enIdx]){
            /* allow hole */
            ahFlag[ch][sfb] = NO_AH;                                                     
            psyOutChan->sfbThreshold[sfb] = L_add(psyOutChan->sfbEnergy[sfb], psyOutChan->sfbEnergy[sfb]);
            actPe = actPe - peData->peChannelData[ch].sfbPe[sfb];
          }
           
          if (actPe < desiredPe) {
            done = 1;                                                                    
=======
    sfb = maxSfb;
    enIdx = 0;
    done = 0;
    while (!done) {

      for (ch=0; ch<nChannels; ch++) {
        PSY_OUT_CHANNEL *psyOutChan = &psyOutChannel[ch];

        if (sfb>=startSfb[ch] && sfb<psyOutChan->sfbCnt) {
          /* sfb energy below border ? */

          if (ahFlag[ch][sfb] != NO_AH && psyOutChan->sfbEnergy[sfb] < en[enIdx]){
            /* allow hole */
            ahFlag[ch][sfb] = NO_AH;
            psyOutChan->sfbThreshold[sfb] = L_add(psyOutChan->sfbEnergy[sfb], psyOutChan->sfbEnergy[sfb]);
            actPe = actPe - peData->peChannelData[ch].sfbPe[sfb];
          }

          if (actPe < desiredPe) {
            done = 1;
>>>>>>> upstream/master
            break;
          }
        }
      }
      sfb = sfb - 1;
<<<<<<< HEAD
       
      if (sfb < minSfb) {
        /* restart with next energy border */
        sfb = maxSfb;                                                                    
        enIdx = enIdx + 1;
         
        if (enIdx - 4 >= 0)
          done = 1;                                                                      
=======

      if (sfb < minSfb) {
        /* restart with next energy border */
        sfb = maxSfb;
        enIdx = enIdx + 1;

        if (enIdx - 4 >= 0)
          done = 1;
>>>>>>> upstream/master
      }
    }
  }
}

/********************************************************************************
*
* function name:adaptThresholdsToPe
* description: two guesses for the reduction value and one final correction of the
*              thresholds
*
**********************************************************************************/
static void adaptThresholdsToPe(PSY_OUT_CHANNEL     psyOutChannel[MAX_CHANNELS],
                                PSY_OUT_ELEMENT    *psyOutElement,
                                Word16              logSfbEnergy[MAX_CHANNELS][MAX_GROUPED_SFB],
                                PE_DATA            *peData,
                                const Word16        nChannels,
                                const Word16        desiredPe,
                                AH_PARAM           *ahParam,
                                MINSNR_ADAPT_PARAM *msaParam)
{
  Word16 noRedPe, redPe, redPeNoAH;
  Word16 constPart, constPartNoAH;
  Word16 nActiveLines, nActiveLinesNoAH;
  Word16 desiredPeNoAH;
  Word32 redVal, avgThrExp;
  Word32 iter;

  calcThreshExp(peData->thrExp, psyOutChannel, nChannels);

  adaptMinSnr(psyOutChannel, logSfbEnergy, msaParam, nChannels);

  initAvoidHoleFlag(peData->ahFlag, psyOutChannel, psyOutElement, nChannels, ahParam);

<<<<<<< HEAD
  noRedPe = peData->pe;                                                          
  constPart = peData->constPart;                                                 
  nActiveLines = peData->nActiveLines;       

  /* first guess of reduction value t^0.25 = 2^((a-pen)/4*b) */
  avgThrExp = pow2_xy((constPart - noRedPe), (nActiveLines << 2));
  
=======
  noRedPe = peData->pe;
  constPart = peData->constPart;
  nActiveLines = peData->nActiveLines;

  /* first guess of reduction value t^0.25 = 2^((a-pen)/4*b) */
  avgThrExp = pow2_xy((constPart - noRedPe), (nActiveLines << 2));

>>>>>>> upstream/master
  /* r1 = 2^((a-per)/4*b) - t^0.25 */
  redVal = pow2_xy((constPart - desiredPe), (nActiveLines << 2)) - avgThrExp;

  /* reduce thresholds */
  reduceThresholds(psyOutChannel, peData->ahFlag, peData->thrExp, nChannels, redVal);

  /* pe after first guess */
  calcSfbPe(peData, psyOutChannel, nChannels);
<<<<<<< HEAD
  redPe = peData->pe;                                                            

  iter = 0;                                                                      
=======
  redPe = peData->pe;

  iter = 0;
>>>>>>> upstream/master
  do {
    /* pe for bands where avoid hole is inactive */
    calcPeNoAH(&redPeNoAH, &constPartNoAH, &nActiveLinesNoAH,
               peData, peData->ahFlag, psyOutChannel, nChannels);

    desiredPeNoAH = desiredPe -(redPe - redPeNoAH);
<<<<<<< HEAD
     
    if (desiredPeNoAH < 0) {
      desiredPeNoAH = 0;                                                         
    }

    /* second guess */
     
    if (nActiveLinesNoAH > 0) {
		
		avgThrExp = pow2_xy((constPartNoAH - redPeNoAH), (nActiveLinesNoAH << 2));
		
		redVal = (redVal + pow2_xy((constPartNoAH - desiredPeNoAH), (nActiveLinesNoAH << 2))) - avgThrExp;
		
=======

    if (desiredPeNoAH < 0) {
      desiredPeNoAH = 0;
    }

    /* second guess */

    if (nActiveLinesNoAH > 0) {

		avgThrExp = pow2_xy((constPartNoAH - redPeNoAH), (nActiveLinesNoAH << 2));

		redVal = (redVal + pow2_xy((constPartNoAH - desiredPeNoAH), (nActiveLinesNoAH << 2))) - avgThrExp;

>>>>>>> upstream/master
		/* reduce thresholds */
		reduceThresholds(psyOutChannel, peData->ahFlag, peData->thrExp, nChannels, redVal);
    }

    calcSfbPe(peData, psyOutChannel, nChannels);
<<<<<<< HEAD
    redPe = peData->pe;                                                          

    iter = iter+1;
       
  } while ((20 * abs_s(redPe - desiredPe) > desiredPe) && (iter < 2));

   
=======
    redPe = peData->pe;

    iter = iter+1;

  } while ((20 * abs_s(redPe - desiredPe) > desiredPe) && (iter < 2));


>>>>>>> upstream/master
  if ((100 * redPe < 115 * desiredPe)) {
    correctThresh(psyOutChannel, peData->ahFlag, peData, peData->thrExp, redVal,
                  nChannels, desiredPe - redPe);
  }
  else {
    Word16 desiredPe105 = (105 * desiredPe) / 100;
    reduceMinSnr(psyOutChannel, peData, peData->ahFlag,
                 nChannels, desiredPe105);
    allowMoreHoles(psyOutChannel, psyOutElement, peData, peData->ahFlag,
                   ahParam, nChannels, desiredPe105);
  }
}


/*****************************************************************************
*
* function name: calcBitSave
* description:  Calculates percentage of bit save, see figure below
* returns:
* input:        parameters and bitres-fullness
* output:       percentage of bit save
*
*****************************************************************************/
static Word16 calcBitSave(Word16 fillLevel,
                          const Word16 clipLow,
                          const Word16 clipHigh,
                          const Word16 minBitSave,
                          const Word16 maxBitSave)
{
  Word16 bitsave = 0;

  fillLevel = max(fillLevel, clipLow);
  fillLevel = min(fillLevel, clipHigh);

  if(clipHigh-clipLow)
  bitsave = (maxBitSave - (((maxBitSave-minBitSave)*(fillLevel-clipLow))/
                              (clipHigh-clipLow)));

  return (bitsave);
}



/*****************************************************************************
*
* function name: calcBitSpend
* description:  Calculates percentage of bit spend, see figure below
* returns:
* input:        parameters and bitres-fullness
* output:       percentage of bit spend
*
*****************************************************************************/
static Word16 calcBitSpend(Word16 fillLevel,
                           const Word16 clipLow,
                           const Word16 clipHigh,
                           const Word16 minBitSpend,
                           const Word16 maxBitSpend)
{
  Word16 bitspend = 1;

  fillLevel = max(fillLevel, clipLow);
  fillLevel = min(fillLevel, clipHigh);

  if(clipHigh-clipLow)
  bitspend = (minBitSpend + ((maxBitSpend - minBitSpend)*(fillLevel - clipLow) /
                                (clipHigh-clipLow)));
<<<<<<< HEAD
                            
=======

>>>>>>> upstream/master
  return (bitspend);
}


/*****************************************************************************
*
* function name: adjustPeMinMax()
* description:  adjusts peMin and peMax parameters over time
* returns:
* input:        current pe, peMin, peMax
* output:       adjusted peMin/peMax
*
*****************************************************************************/
static void adjustPeMinMax(const Word16 currPe,
                           Word16      *peMin,
                           Word16      *peMax)
{
  Word16 minFacHi, maxFacHi, minFacLo, maxFacLo;
  Word16 diff;
  Word16 minDiff = extract_l(currPe / 6);
<<<<<<< HEAD
  minFacHi = 30;                                                         
  maxFacHi = 100;                                                        
  minFacLo = 14;                                                         
  maxFacLo = 7;                                                          

  diff = currPe - *peMax ;
   
=======
  minFacHi = 30;
  maxFacHi = 100;
  minFacLo = 14;
  maxFacLo = 7;

  diff = currPe - *peMax ;

>>>>>>> upstream/master
  if (diff > 0) {
    *peMin = *peMin + ((diff * minFacHi) / 100);
    *peMax = *peMax + ((diff * maxFacHi) / 100);
  } else {
    diff = *peMin - currPe;
<<<<<<< HEAD
     
=======

>>>>>>> upstream/master
    if (diff > 0) {
      *peMin = *peMin - ((diff * minFacLo) / 100);
      *peMax = *peMax - ((diff * maxFacLo) / 100);
    } else {
      *peMin = *peMin + ((currPe - *peMin) * minFacHi / 100);
      *peMax = *peMax - ((*peMax - currPe) * maxFacLo / 100);
    }
  }

<<<<<<< HEAD
   
=======

>>>>>>> upstream/master
  if ((*peMax - *peMin) < minDiff) {
    Word16 partLo, partHi;

    partLo = max(0, (currPe - *peMin));
    partHi = max(0, (*peMax - currPe));

    *peMax = currPe + ((partHi * minDiff) / (partLo + partHi));
    *peMin = currPe - ((partLo * minDiff) / (partLo + partHi));
    *peMin = max(0, *peMin);
  }
}


/*****************************************************************************
*
* function name: BitresCalcBitFac
* description:  calculates factor of spending bits for one frame
*                1.0 : take all frame dynpart bits
*                >1.0 : take all frame dynpart bits + bitres
*                <1.0 : put bits in bitreservoir
*  returns:      BitFac*100
*  input:        bitres-fullness, pe, blockType, parameter-settings
*  output:
*
*****************************************************************************/
static Word16 bitresCalcBitFac( const Word16   bitresBits,
                                const Word16   maxBitresBits,
                                const Word16   pe,
                                const Word16   windowSequence,
                                const Word16   avgBits,
                                const Word16   maxBitFac,
                                ADJ_THR_STATE *AdjThr,
                                ATS_ELEMENT   *adjThrChan)
{
  BRES_PARAM *bresParam;
  Word16 pex;
  Word16 fillLevel;
  Word16 bitSave, bitSpend, bitresFac;

  fillLevel = extract_l((100* bitresBits) / maxBitresBits);

  if (windowSequence != SHORT_WINDOW)
    bresParam = &(AdjThr->bresParamLong);
  else
    bresParam = &(AdjThr->bresParamShort);

  pex = max(pe, adjThrChan->peMin);
  pex = min(pex,adjThrChan->peMax);

  bitSave = calcBitSave(fillLevel,
                        bresParam->clipSaveLow, bresParam->clipSaveHigh,
                        bresParam->minBitSave, bresParam->maxBitSave);

  bitSpend = calcBitSpend(fillLevel,
                          bresParam->clipSpendLow, bresParam->clipSpendHigh,
                          bresParam->minBitSpend, bresParam->maxBitSpend);

  if(adjThrChan->peMax != adjThrChan->peMin)
	bitresFac = (100 - bitSave) + extract_l(((bitSpend + bitSave) * (pex - adjThrChan->peMin)) /
                    (adjThrChan->peMax - adjThrChan->peMin));
  else
	bitresFac = 0x7fff;
<<<<<<< HEAD
               
=======

>>>>>>> upstream/master
  bitresFac = min(bitresFac,
                    (100-30 + extract_l((100 * bitresBits) / avgBits)));

  bitresFac = min(bitresFac, maxBitFac);

  adjustPeMinMax(pe, &adjThrChan->peMin, &adjThrChan->peMax);

  return bitresFac;
}

/*****************************************************************************
*
* function name: AdjThrInit
* description:  init thresholds parameter
*
*****************************************************************************/
void AdjThrInit(ADJ_THR_STATE *hAdjThr,
                const Word32   meanPe,
                Word32         chBitrate)
{
  ATS_ELEMENT* atsElem = &hAdjThr->adjThrStateElem;
  MINSNR_ADAPT_PARAM *msaParam = &atsElem->minSnrAdaptParam;

  /* common for all elements: */
  /* parameters for bitres control */
<<<<<<< HEAD
  hAdjThr->bresParamLong.clipSaveLow   =  20;                    
  hAdjThr->bresParamLong.clipSaveHigh  =  95;                    
  hAdjThr->bresParamLong.minBitSave    =  -5;                    
  hAdjThr->bresParamLong.maxBitSave    =  30;                    
  hAdjThr->bresParamLong.clipSpendLow  =  20;                    
  hAdjThr->bresParamLong.clipSpendHigh =  95;                    
  hAdjThr->bresParamLong.minBitSpend   = -10;                    
  hAdjThr->bresParamLong.maxBitSpend   =  40;                    

  hAdjThr->bresParamShort.clipSaveLow   =  20;                   
  hAdjThr->bresParamShort.clipSaveHigh  =  75;                   
  hAdjThr->bresParamShort.minBitSave    =   0;                   
  hAdjThr->bresParamShort.maxBitSave    =  20;                   
  hAdjThr->bresParamShort.clipSpendLow  =  20;                   
  hAdjThr->bresParamShort.clipSpendHigh =  75;                   
  hAdjThr->bresParamShort.minBitSpend   = -5;                    
  hAdjThr->bresParamShort.maxBitSpend   =  50;                   
=======
  hAdjThr->bresParamLong.clipSaveLow   =  20;
  hAdjThr->bresParamLong.clipSaveHigh  =  95;
  hAdjThr->bresParamLong.minBitSave    =  -5;
  hAdjThr->bresParamLong.maxBitSave    =  30;
  hAdjThr->bresParamLong.clipSpendLow  =  20;
  hAdjThr->bresParamLong.clipSpendHigh =  95;
  hAdjThr->bresParamLong.minBitSpend   = -10;
  hAdjThr->bresParamLong.maxBitSpend   =  40;

  hAdjThr->bresParamShort.clipSaveLow   =  20;
  hAdjThr->bresParamShort.clipSaveHigh  =  75;
  hAdjThr->bresParamShort.minBitSave    =   0;
  hAdjThr->bresParamShort.maxBitSave    =  20;
  hAdjThr->bresParamShort.clipSpendLow  =  20;
  hAdjThr->bresParamShort.clipSpendHigh =  75;
  hAdjThr->bresParamShort.minBitSpend   = -5;
  hAdjThr->bresParamShort.maxBitSpend   =  50;
>>>>>>> upstream/master

  /* specific for each element: */

  /* parameters for bitres control */
  atsElem->peMin = extract_l(((80*meanPe) / 100));
  atsElem->peMax = extract_l(((120*meanPe) / 100));

  /* additional pe offset to correct pe2bits for low bitrates */
<<<<<<< HEAD
  atsElem->peOffset = 0;                             
=======
  atsElem->peOffset = 0;
>>>>>>> upstream/master
  if (chBitrate < 32000) {
    atsElem->peOffset = max(50, (100 - extract_l((100 * chBitrate) / 32000)));
  }

  /* avoid hole parameters */
  if (chBitrate > 20000) {
    atsElem->ahParam.modifyMinSnr = TRUE;
    atsElem->ahParam.startSfbL = 15;
    atsElem->ahParam.startSfbS = 3;
  }
  else {
    atsElem->ahParam.modifyMinSnr = FALSE;
    atsElem->ahParam.startSfbL = 0;
    atsElem->ahParam.startSfbS = 0;
  }

  /* minSnr adaptation */
  /* maximum reduction of minSnr goes down to minSnr^maxRed */
<<<<<<< HEAD
  msaParam->maxRed = 0x20000000;     /* *0.25f /                        
  /* start adaptation of minSnr for avgEn/sfbEn > startRatio */
  msaParam->startRatio = 0x0ccccccd; /* 10 */                        
  /* maximum minSnr reduction to minSnr^maxRed is reached for
     avgEn/sfbEn >= maxRatio */
  msaParam->maxRatio =  0x0020c49c; /* 1000 */                         
  /* helper variables to interpolate minSnr reduction for
     avgEn/sfbEn between startRatio and maxRatio */

  msaParam->redRatioFac = 0xfb333333; /* -0.75/20 */         

  msaParam->redOffs = 0x30000000;  /* msaParam->redRatioFac * 10*log10(msaParam->startRatio) */  

       
  /* pe correction */
  atsElem->peLast = 0;                                                 
  atsElem->dynBitsLast = 0;                                            
  atsElem->peCorrectionFactor = 100; /* 1.0 */                         
=======
  msaParam->maxRed = 0x20000000;     /* *0.25f /
  /* start adaptation of minSnr for avgEn/sfbEn > startRatio */
  msaParam->startRatio = 0x0ccccccd; /* 10 */
  /* maximum minSnr reduction to minSnr^maxRed is reached for
     avgEn/sfbEn >= maxRatio */
  msaParam->maxRatio =  0x0020c49c; /* 1000 */
  /* helper variables to interpolate minSnr reduction for
     avgEn/sfbEn between startRatio and maxRatio */

  msaParam->redRatioFac = 0xfb333333; /* -0.75/20 */

  msaParam->redOffs = 0x30000000;  /* msaParam->redRatioFac * 10*log10(msaParam->startRatio) */


  /* pe correction */
  atsElem->peLast = 0;
  atsElem->dynBitsLast = 0;
  atsElem->peCorrectionFactor = 100; /* 1.0 */
>>>>>>> upstream/master

}

/*****************************************************************************
*
* function name: calcPeCorrection
* description:  calculates the desired perceptual entropy factor
*				It is between 0.85 and 1.15
*
*****************************************************************************/
static void calcPeCorrection(Word16 *correctionFac,
                             const Word16 peAct,
<<<<<<< HEAD
                             const Word16 peLast, 
                             const Word16 bitsLast) 
=======
                             const Word16 peLast,
                             const Word16 bitsLast)
>>>>>>> upstream/master
{
  Word32 peAct100 = 100 * peAct;
  Word32 peLast100 = 100 * peLast;
  Word16 peBitsLast = bits2pe(bitsLast);
<<<<<<< HEAD
           
=======

>>>>>>> upstream/master
  if ((bitsLast > 0) &&
      (peAct100 < (150 * peLast)) &&  (peAct100 > (70 * peLast)) &&
      ((120 * peBitsLast) > peLast100 ) && (( 65 * peBitsLast) < peLast100))
    {
      Word16 newFac = (100 * peLast) / peBitsLast;
      /* dead zone */
<<<<<<< HEAD
       
=======

>>>>>>> upstream/master
      if (newFac < 100) {
        newFac = min(((110 * newFac) / 100), 100);
        newFac = max(newFac, 85);
      }
      else {
        newFac = max(((90 * newFac) / 100), 100);
        newFac = min(newFac, 115);
      }
<<<<<<< HEAD
         
      if ((newFac > 100 && *correctionFac < 100) ||
          (newFac < 100 && *correctionFac > 100)) {
        *correctionFac = 100;                                                    
      }
      /* faster adaptation towards 1.0, slower in the other direction */
             
=======

      if ((newFac > 100 && *correctionFac < 100) ||
          (newFac < 100 && *correctionFac > 100)) {
        *correctionFac = 100;
      }
      /* faster adaptation towards 1.0, slower in the other direction */

>>>>>>> upstream/master
      if ((*correctionFac < 100 && newFac < *correctionFac) ||
          (*correctionFac > 100 && newFac > *correctionFac))
        *correctionFac = (85 * *correctionFac + 15 * newFac) / 100;
      else
        *correctionFac = (70 * *correctionFac + 30 * newFac) / 100;
      *correctionFac = min(*correctionFac, 115);
      *correctionFac = max(*correctionFac, 85);
    }
  else {
<<<<<<< HEAD
    *correctionFac = 100;                                                        
=======
    *correctionFac = 100;
>>>>>>> upstream/master
  }
}

/********************************************************************************
*
* function name: AdjustThresholds
* description:  Adjust thresholds to the desired bitrate
*
**********************************************************************************/
void AdjustThresholds(ADJ_THR_STATE   *adjThrState,
                      ATS_ELEMENT     *AdjThrStateElement,
                      PSY_OUT_CHANNEL  psyOutChannel[MAX_CHANNELS],
                      PSY_OUT_ELEMENT *psyOutElement,
                      Word16          *chBitDistribution,
                      Word16           logSfbEnergy[MAX_CHANNELS][MAX_GROUPED_SFB],
<<<<<<< HEAD
                      Word16           sfbNRelevantLines[MAX_CHANNELS][MAX_GROUPED_SFB],                      
=======
                      Word16           sfbNRelevantLines[MAX_CHANNELS][MAX_GROUPED_SFB],
>>>>>>> upstream/master
                      QC_OUT_ELEMENT  *qcOE,
					  ELEMENT_BITS	  *elBits,
					  const Word16     nChannels,
                      const Word16     maxBitFac)
{
<<<<<<< HEAD
  PE_DATA peData;  
=======
  PE_DATA peData;
>>>>>>> upstream/master
  Word16 noRedPe, grantedPe, grantedPeCorr;
  Word16 curWindowSequence;
  Word16 bitFactor;
  Word16 avgBits = (elBits->averageBits - (qcOE->staticBitsUsed + qcOE->ancBitsUsed));
<<<<<<< HEAD
  Word16 bitresBits = elBits->bitResLevel; 
  Word16 maxBitresBits = elBits->maxBits;
  Word16 sideInfoBits = (qcOE->staticBitsUsed + qcOE->ancBitsUsed);
  Word16 ch;
   
  prepareSfbPe(&peData, psyOutChannel, logSfbEnergy, sfbNRelevantLines, nChannels, AdjThrStateElement->peOffset);
   
  /* pe without reduction */
  calcSfbPe(&peData, psyOutChannel, nChannels);
  noRedPe = peData.pe;                                                   


  curWindowSequence = LONG_WINDOW;                                       
   
  if (nChannels == 2) {
       
    if ((psyOutChannel[0].windowSequence == SHORT_WINDOW) ||
        (psyOutChannel[1].windowSequence == SHORT_WINDOW)) {
      curWindowSequence = SHORT_WINDOW;                                  
    }
  }
  else {
    curWindowSequence = psyOutChannel[0].windowSequence;                 
=======
  Word16 bitresBits = elBits->bitResLevel;
  Word16 maxBitresBits = elBits->maxBits;
  Word16 sideInfoBits = (qcOE->staticBitsUsed + qcOE->ancBitsUsed);
  Word16 ch;

  prepareSfbPe(&peData, psyOutChannel, logSfbEnergy, sfbNRelevantLines, nChannels, AdjThrStateElement->peOffset);

  /* pe without reduction */
  calcSfbPe(&peData, psyOutChannel, nChannels);
  noRedPe = peData.pe;


  curWindowSequence = LONG_WINDOW;

  if (nChannels == 2) {

    if ((psyOutChannel[0].windowSequence == SHORT_WINDOW) ||
        (psyOutChannel[1].windowSequence == SHORT_WINDOW)) {
      curWindowSequence = SHORT_WINDOW;
    }
  }
  else {
    curWindowSequence = psyOutChannel[0].windowSequence;
>>>>>>> upstream/master
  }


  /* bit factor */
  bitFactor = bitresCalcBitFac(bitresBits, maxBitresBits, noRedPe+5*sideInfoBits,
                               curWindowSequence, avgBits, maxBitFac,
                               adjThrState,
                               AdjThrStateElement);

  /* desired pe */
  grantedPe = ((bitFactor * bits2pe(avgBits)) / 100);

  /* correction of pe value */
<<<<<<< HEAD
  calcPeCorrection(&(AdjThrStateElement->peCorrectionFactor), 
                   min(grantedPe, noRedPe),
                   AdjThrStateElement->peLast, 
                   AdjThrStateElement->dynBitsLast);
  grantedPeCorr = (grantedPe * AdjThrStateElement->peCorrectionFactor) / 100;

     
=======
  calcPeCorrection(&(AdjThrStateElement->peCorrectionFactor),
                   min(grantedPe, noRedPe),
                   AdjThrStateElement->peLast,
                   AdjThrStateElement->dynBitsLast);
  grantedPeCorr = (grantedPe * AdjThrStateElement->peCorrectionFactor) / 100;


>>>>>>> upstream/master
  if (grantedPeCorr < noRedPe && noRedPe > peData.offset) {
    /* calc threshold necessary for desired pe */
    adaptThresholdsToPe(psyOutChannel,
                        psyOutElement,
                        logSfbEnergy,
                        &peData,
                        nChannels,
                        grantedPeCorr,
                        &AdjThrStateElement->ahParam,
                        &AdjThrStateElement->minSnrAdaptParam);
  }

  /* calculate relative distribution */
  for (ch=0; ch<nChannels; ch++) {
    Word16 peOffsDiff = peData.pe - peData.offset;
<<<<<<< HEAD
    chBitDistribution[ch] = 200;                                                 
     
=======
    chBitDistribution[ch] = 200;

>>>>>>> upstream/master
    if (peOffsDiff > 0) {
      Word32 temp = 1000 - (nChannels * 200);
      chBitDistribution[ch] = chBitDistribution[ch] +
		  (temp * peData.peChannelData[ch].pe) / peOffsDiff;
    }
  }

  /* store pe */
<<<<<<< HEAD
  qcOE->pe = noRedPe;                                                            

  /* update last pe */
  AdjThrStateElement->peLast = grantedPe;                                        
=======
  qcOE->pe = noRedPe;

  /* update last pe */
  AdjThrStateElement->peLast = grantedPe;
>>>>>>> upstream/master
}

/********************************************************************************
*
* function name: AdjThrUpdate
* description:  save dynBitsUsed for correction of bits2pe relation
*
**********************************************************************************/
void AdjThrUpdate(ATS_ELEMENT *AdjThrStateElement,
                  const Word16 dynBitsUsed)
{
<<<<<<< HEAD
  AdjThrStateElement->dynBitsLast = dynBitsUsed;                                 
=======
  AdjThrStateElement->dynBitsLast = dynBitsUsed;
>>>>>>> upstream/master
}


