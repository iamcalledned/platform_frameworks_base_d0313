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
	File:		sf_estim.c

	Content:	Scale factor estimation functions

*******************************************************************************/

#include "basic_op.h"
#include "oper_32b.h"
#include "sf_estim.h"
#include "quantize.h"
#include "bit_cnt.h"
#include "aac_rom.h"

static const Word16 MAX_SCF_DELTA = 60;

/*!
<<<<<<< HEAD
constants reference in comments 

 C0 = 6.75f;
 C1 = -69.33295f;   -16/3*log(MAX_QUANT+0.5-logCon)/log(2) 
 C2 = 4.0f;
 C3 = 2.66666666f;
 
  PE_C1 = 3.0f;        log(8.0)/log(2) 
  PE_C2 = 1.3219281f;  log(2.5)/log(2) 
  PE_C3 = 0.5593573f;  1-C2/C1 
  
=======
constants reference in comments

 C0 = 6.75f;
 C1 = -69.33295f;   -16/3*log(MAX_QUANT+0.5-logCon)/log(2)
 C2 = 4.0f;
 C3 = 2.66666666f;

  PE_C1 = 3.0f;        log(8.0)/log(2)
  PE_C2 = 1.3219281f;  log(2.5)/log(2)
  PE_C3 = 0.5593573f;  1-C2/C1

>>>>>>> upstream/master
*/

#define FF_SQRT_BITS                    7
#define FF_SQRT_TABLE_SIZE              (1<<FF_SQRT_BITS - 1<<(FF_SQRT_BITS-2))
#define COEF08_31		0x66666666		/* 0.8*(1 << 31) */
#define PE_C1_8			24				/* PE_C1*8 */
#define PE_C2_16		21				/* PE_C2*8/PE_C3 */
#define PE_SCALE		0x059a			/* 0.7 * (1 << (15 - 1 - 3))*/

#define SCALE_ESTIMATE_COEF	0x5555		/* (8.8585/(4*log2(10))) * (1 << 15)*/

/*********************************************************************************
*
* function name: formfac_sqrt
<<<<<<< HEAD
* description:  calculates sqrt(x)/256 
=======
* description:  calculates sqrt(x)/256
>>>>>>> upstream/master
*
**********************************************************************************/
__inline Word32 formfac_sqrt(Word32 x)
{
	Word32 y;
	Word32 preshift, postshift;
<<<<<<< HEAD
	
	
=======


>>>>>>> upstream/master
	if (x==0) return 0;
	preshift  = norm_l(x) - (INT_BITS-1-FF_SQRT_BITS);
	postshift = preshift >> 1;
	preshift  = postshift << 1;
	postshift = postshift + 8;	  /* sqrt/256 */
	if(preshift >= 0)
		y = x << preshift;        /* now 1/4 <= y < 1 */
	else
		y = x >> (-preshift);
	y = formfac_sqrttable[y-32];
<<<<<<< HEAD
	
=======

>>>>>>> upstream/master
	if(postshift >= 0)
		y = y >> postshift;
	else
		y = y << (-postshift);
<<<<<<< HEAD
	
=======

>>>>>>> upstream/master
	return y;
}


/*********************************************************************************
*
* function name: CalcFormFactorChannel
* description:  calculate the form factor one channel
*				ffac(n) = sqrt(abs(X(k)) + sqrt(abs(X(k+1)) + ....
*
**********************************************************************************/
static void
CalcFormFactorChannel(Word16 *logSfbFormFactor,
                      Word16 *sfbNRelevantLines,
                      Word16 *logSfbEnergy,
                      PSY_OUT_CHANNEL *psyOutChan)
{
	Word32 sfbw, sfbw1;
	Word32 i, j;
	Word32 sfbOffs, sfb, shift;
<<<<<<< HEAD
	
	sfbw = sfbw1 = 0;
	for (sfbOffs=0; sfbOffs<psyOutChan->sfbCnt; sfbOffs+=psyOutChan->sfbPerGroup){
		for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
			i = sfbOffs+sfb;      
			
=======

	sfbw = sfbw1 = 0;
	for (sfbOffs=0; sfbOffs<psyOutChan->sfbCnt; sfbOffs+=psyOutChan->sfbPerGroup){
		for (sfb=0; sfb<psyOutChan->maxSfbPerGroup; sfb++) {
			i = sfbOffs+sfb;

>>>>>>> upstream/master
			if (psyOutChan->sfbEnergy[i] > psyOutChan->sfbThreshold[i]) {
				Word32 accu, avgFormFactor,iSfbWidth;
				Word32 *mdctSpec;
				sfbw = psyOutChan->sfbOffsets[i+1] - psyOutChan->sfbOffsets[i];
				iSfbWidth = invSBF[(sfbw >> 2) - 1];
				mdctSpec = psyOutChan->mdctSpectrum + psyOutChan->sfbOffsets[i];
<<<<<<< HEAD
				accu = 0;                                                                       
=======
				accu = 0;
>>>>>>> upstream/master
				/* calc sum of sqrt(spec) */
				for (j=sfbw; j; j--) {
					accu += formfac_sqrt(L_abs(*mdctSpec)); mdctSpec++;
				}
				logSfbFormFactor[i] = iLog4(accu);
				logSfbEnergy[i] = iLog4(psyOutChan->sfbEnergy[i]);
				avgFormFactor = fixmul(rsqrt(psyOutChan->sfbEnergy[i],INT_BITS), iSfbWidth);
				avgFormFactor = rsqrt((Word32)avgFormFactor,INT_BITS) >> 10;
				/* result is multiplied by 4 */
				if(avgFormFactor)
					sfbNRelevantLines[i] = accu / avgFormFactor;
				else
					sfbNRelevantLines[i] = 0x7fff;
			}
			else {
				/* set number of lines to zero */
<<<<<<< HEAD
				sfbNRelevantLines[i] = 0;                                                       
=======
				sfbNRelevantLines[i] = 0;
>>>>>>> upstream/master
			}
		}
	}
}

/*********************************************************************************
*
* function name: improveScf
* description:  find better scalefactor with analysis by synthesis
*
**********************************************************************************/
<<<<<<< HEAD
static Word16 improveScf(Word32 *spec, 
                         Word16  sfbWidth, 
                         Word32  thresh, 
                         Word16  scf,
                         Word16  minScf,
                         Word32 *dist, 
=======
static Word16 improveScf(Word32 *spec,
                         Word16  sfbWidth,
                         Word32  thresh,
                         Word16  scf,
                         Word16  minScf,
                         Word32 *dist,
>>>>>>> upstream/master
                         Word16 *minScfCalculated)
{
	Word32 cnt;
	Word32 sfbDist;
	Word32 scfBest;
	Word32 thresh125 = L_add(thresh, (thresh >> 2));
<<<<<<< HEAD
	
	scfBest = scf;                                                       
	
	/* calc real distortion */
	sfbDist = calcSfbDist(spec, sfbWidth, scf);
	*minScfCalculated = scf;     
	if(!sfbDist)
	  return scfBest;
	
	if (sfbDist > thresh125) {
		Word32 scfEstimated;
		Word32 sfbDistBest;
		scfEstimated = scf;                                               
		sfbDistBest = sfbDist;                                            
		
		cnt = 0;                                                          
		while (sfbDist > thresh125 && (cnt < 3)) {
			
			scf = scf + 1;
			sfbDist = calcSfbDist(spec, sfbWidth, scf);
			
			if (sfbDist < sfbDistBest) {
				scfBest = scf;                                              
				sfbDistBest = sfbDist;                                      
			}
			cnt = cnt + 1;
		}
		cnt = 0;                                                          
		scf = scfEstimated;                                               
		sfbDist = sfbDistBest;                                            
		while ((sfbDist > thresh125) && (cnt < 1) && (scf > minScf)) {
			
			scf = scf - 1;
			sfbDist = calcSfbDist(spec, sfbWidth, scf);
			
			if (sfbDist < sfbDistBest) {
				scfBest = scf;                                              
				sfbDistBest = sfbDist;                                      
			}
			*minScfCalculated = scf;                                       
			cnt = cnt + 1;
		}
		*dist = sfbDistBest;                                              
	}
	else {
		Word32 sfbDistBest; 
		Word32 sfbDistAllowed;
		Word32 thresh08 = fixmul(COEF08_31, thresh);
		sfbDistBest = sfbDist;                                            
		
=======

	scfBest = scf;

	/* calc real distortion */
	sfbDist = calcSfbDist(spec, sfbWidth, scf);
	*minScfCalculated = scf;
	if(!sfbDist)
	  return scfBest;

	if (sfbDist > thresh125) {
		Word32 scfEstimated;
		Word32 sfbDistBest;
		scfEstimated = scf;
		sfbDistBest = sfbDist;

		cnt = 0;
		while (sfbDist > thresh125 && (cnt < 3)) {

			scf = scf + 1;
			sfbDist = calcSfbDist(spec, sfbWidth, scf);

			if (sfbDist < sfbDistBest) {
				scfBest = scf;
				sfbDistBest = sfbDist;
			}
			cnt = cnt + 1;
		}
		cnt = 0;
		scf = scfEstimated;
		sfbDist = sfbDistBest;
		while ((sfbDist > thresh125) && (cnt < 1) && (scf > minScf)) {

			scf = scf - 1;
			sfbDist = calcSfbDist(spec, sfbWidth, scf);

			if (sfbDist < sfbDistBest) {
				scfBest = scf;
				sfbDistBest = sfbDist;
			}
			*minScfCalculated = scf;
			cnt = cnt + 1;
		}
		*dist = sfbDistBest;
	}
	else {
		Word32 sfbDistBest;
		Word32 sfbDistAllowed;
		Word32 thresh08 = fixmul(COEF08_31, thresh);
		sfbDistBest = sfbDist;

>>>>>>> upstream/master
		if (sfbDist < thresh08)
			sfbDistAllowed = sfbDist;
		else
			sfbDistAllowed = thresh08;
		for (cnt=0; cnt<3; cnt++) {
			scf = scf + 1;
			sfbDist = calcSfbDist(spec, sfbWidth, scf);
<<<<<<< HEAD
			
			if (fixmul(COEF08_31,sfbDist) < sfbDistAllowed) {
				*minScfCalculated = scfBest + 1;
				scfBest = scf;                                              
				sfbDistBest = sfbDist;                                      
			}
		}
		*dist = sfbDistBest;                                              
	}
	
=======

			if (fixmul(COEF08_31,sfbDist) < sfbDistAllowed) {
				*minScfCalculated = scfBest + 1;
				scfBest = scf;
				sfbDistBest = sfbDist;
			}
		}
		*dist = sfbDistBest;
	}

>>>>>>> upstream/master
	/* return best scalefactor */
	return scfBest;
}

/*********************************************************************************
*
* function name: countSingleScfBits
* description:  count single scf bits in huffum
*
**********************************************************************************/
static Word16 countSingleScfBits(Word16 scf, Word16 scfLeft, Word16 scfRight)
{
	Word16 scfBits;
<<<<<<< HEAD
	
	scfBits = bitCountScalefactorDelta(scfLeft - scf) +
		bitCountScalefactorDelta(scf - scfRight);
	
=======

	scfBits = bitCountScalefactorDelta(scfLeft - scf) +
		bitCountScalefactorDelta(scf - scfRight);

>>>>>>> upstream/master
	return scfBits;
}

/*********************************************************************************
*
* function name: calcSingleSpecPe
* description:  ldRatio = log2(en(n)) - 0,375*scfGain(n)
*				nbits = 0.7*nLines*ldRation for ldRation >= c1
<<<<<<< HEAD
*				nbits = 0.7*nLines*(c2 + c3*ldRatio) for ldRation < c1 
=======
*				nbits = 0.7*nLines*(c2 + c3*ldRatio) for ldRation < c1
>>>>>>> upstream/master
*
**********************************************************************************/
static Word16 calcSingleSpecPe(Word16 scf, Word16 sfbConstPePart, Word16 nLines)
{
	Word32 specPe;
	Word32 ldRatio;
	Word32 scf3;
<<<<<<< HEAD
	
	ldRatio = sfbConstPePart << 3; /*  (sfbConstPePart -0.375*scf)*8 */
	scf3 = scf + scf + scf;
	ldRatio = ldRatio - scf3;
    
	if (ldRatio < PE_C1_8) {
		/* 21 : 2*8*PE_C2, 2*PE_C3 ~ 1*/ 
=======

	ldRatio = sfbConstPePart << 3; /*  (sfbConstPePart -0.375*scf)*8 */
	scf3 = scf + scf + scf;
	ldRatio = ldRatio - scf3;

	if (ldRatio < PE_C1_8) {
		/* 21 : 2*8*PE_C2, 2*PE_C3 ~ 1*/
>>>>>>> upstream/master
		ldRatio = (ldRatio + PE_C2_16) >> 1;
	}
	specPe = nLines * ldRatio;
	specPe = (specPe * PE_SCALE) >> 14;
<<<<<<< HEAD
	
=======

>>>>>>> upstream/master
	return saturate(specPe);
}


/*********************************************************************************
*
* function name: countScfBitsDiff
* description:  count different scf bits used
*
**********************************************************************************/
<<<<<<< HEAD
static Word16 countScfBitsDiff(Word16 *scfOld, Word16 *scfNew, 
=======
static Word16 countScfBitsDiff(Word16 *scfOld, Word16 *scfNew,
>>>>>>> upstream/master
                               Word16 sfbCnt, Word16 startSfb, Word16 stopSfb)
{
	Word32 scfBitsDiff;
	Word32 sfb, sfbLast;
	Word32 sfbPrev, sfbNext;
<<<<<<< HEAD
	
	scfBitsDiff = 0;                                                      
	sfb = 0;                                                              
	
	/* search for first relevant sfb */
	sfbLast = startSfb;                                                   
	while (sfbLast < stopSfb && scfOld[sfbLast] == VOAAC_SHRT_MIN) {
		
=======

	scfBitsDiff = 0;
	sfb = 0;

	/* search for first relevant sfb */
	sfbLast = startSfb;
	while (sfbLast < stopSfb && scfOld[sfbLast] == VOAAC_SHRT_MIN) {

>>>>>>> upstream/master
		sfbLast = sfbLast + 1;
	}
	/* search for previous relevant sfb and count diff */
	sfbPrev = startSfb - 1;
	while ((sfbPrev>=0) && scfOld[sfbPrev] == VOAAC_SHRT_MIN) {
<<<<<<< HEAD
		
		sfbPrev = sfbPrev - 1;
	}
	
=======

		sfbPrev = sfbPrev - 1;
	}

>>>>>>> upstream/master
	if (sfbPrev>=0) {
		scfBitsDiff += bitCountScalefactorDelta(scfNew[sfbPrev] - scfNew[sfbLast]) -
			bitCountScalefactorDelta(scfOld[sfbPrev] - scfOld[sfbLast]);
	}
	/* now loop through all sfbs and count diffs of relevant sfbs */
	for (sfb=sfbLast+1; sfb<stopSfb; sfb++) {
<<<<<<< HEAD
		
		if (scfOld[sfb] != VOAAC_SHRT_MIN) {
			scfBitsDiff += bitCountScalefactorDelta(scfNew[sfbLast] - scfNew[sfb]) -
				bitCountScalefactorDelta(scfOld[sfbLast] - scfOld[sfb]);
			sfbLast = sfb;                                                    
		}
	}
	/* search for next relevant sfb and count diff */
	sfbNext = stopSfb;                                                    
	while (sfbNext < sfbCnt && scfOld[sfbNext] == VOAAC_SHRT_MIN) {
		
		sfbNext = sfbNext + 1;
	}
	
	if (sfbNext < sfbCnt)
		scfBitsDiff += bitCountScalefactorDelta(scfNew[sfbLast] - scfNew[sfbNext]) -
		bitCountScalefactorDelta(scfOld[sfbLast] - scfOld[sfbNext]);
	
=======

		if (scfOld[sfb] != VOAAC_SHRT_MIN) {
			scfBitsDiff += bitCountScalefactorDelta(scfNew[sfbLast] - scfNew[sfb]) -
				bitCountScalefactorDelta(scfOld[sfbLast] - scfOld[sfb]);
			sfbLast = sfb;
		}
	}
	/* search for next relevant sfb and count diff */
	sfbNext = stopSfb;
	while (sfbNext < sfbCnt && scfOld[sfbNext] == VOAAC_SHRT_MIN) {

		sfbNext = sfbNext + 1;
	}

	if (sfbNext < sfbCnt)
		scfBitsDiff += bitCountScalefactorDelta(scfNew[sfbLast] - scfNew[sfbNext]) -
		bitCountScalefactorDelta(scfOld[sfbLast] - scfOld[sfbNext]);

>>>>>>> upstream/master
	return saturate(scfBitsDiff);
}

static Word16 calcSpecPeDiff(Word16 *scfOld,
                             Word16 *scfNew,
                             Word16 *sfbConstPePart,
                             Word16 *logSfbEnergy,
                             Word16 *logSfbFormFactor,
                             Word16 *sfbNRelevantLines,
<<<<<<< HEAD
                             Word16 startSfb, 
=======
                             Word16 startSfb,
>>>>>>> upstream/master
                             Word16 stopSfb)
{
	Word32 specPeDiff;
	Word32 sfb;
<<<<<<< HEAD
	
	specPeDiff = 0;                                                       
	
	/* loop through all sfbs and count pe difference */
	for (sfb=startSfb; sfb<stopSfb; sfb++) {
		
		
		if (scfOld[sfb] != VOAAC_SHRT_MIN) {
			Word32 ldRatioOld, ldRatioNew;
			Word32 scf3;
			
			
=======

	specPeDiff = 0;

	/* loop through all sfbs and count pe difference */
	for (sfb=startSfb; sfb<stopSfb; sfb++) {


		if (scfOld[sfb] != VOAAC_SHRT_MIN) {
			Word32 ldRatioOld, ldRatioNew;
			Word32 scf3;


>>>>>>> upstream/master
			if (sfbConstPePart[sfb] == MIN_16) {
				sfbConstPePart[sfb] = ((logSfbEnergy[sfb] -
					logSfbFormFactor[sfb]) + 11-8*4+3) >> 2;
			}
<<<<<<< HEAD
			
			
=======


>>>>>>> upstream/master
			ldRatioOld = sfbConstPePart[sfb] << 3;
			scf3 = scfOld[sfb] + scfOld[sfb] + scfOld[sfb];
			ldRatioOld = ldRatioOld - scf3;
			ldRatioNew = sfbConstPePart[sfb] << 3;
			scf3 = scfNew[sfb] + scfNew[sfb] + scfNew[sfb];
			ldRatioNew = ldRatioNew - scf3;
<<<<<<< HEAD
			
=======

>>>>>>> upstream/master
			if (ldRatioOld < PE_C1_8) {
				/* 21 : 2*8*PE_C2, 2*PE_C3 ~ 1*/
				ldRatioOld = (ldRatioOld + PE_C2_16) >> 1;
			}
<<<<<<< HEAD
			
=======

>>>>>>> upstream/master
			if (ldRatioNew < PE_C1_8) {
				/* 21 : 2*8*PE_C2, 2*PE_C3 ~ 1*/
				ldRatioNew = (ldRatioNew + PE_C2_16) >> 1;
			}
<<<<<<< HEAD
			
			specPeDiff +=  sfbNRelevantLines[sfb] * (ldRatioNew - ldRatioOld);
		}
	}
	
	specPeDiff = (specPeDiff * PE_SCALE) >> 14;
	
=======

			specPeDiff +=  sfbNRelevantLines[sfb] * (ldRatioNew - ldRatioOld);
		}
	}

	specPeDiff = (specPeDiff * PE_SCALE) >> 14;

>>>>>>> upstream/master
	return saturate(specPeDiff);
}


/*********************************************************************************
*
* function name: assimilateSingleScf
* description:  searched for single scalefactor bands, where the number of bits gained
*				by using a smaller scfgain(n) is greater than the estimated increased
*				bit demand
*
**********************************************************************************/
static void assimilateSingleScf(PSY_OUT_CHANNEL *psyOutChan,
<<<<<<< HEAD
                                Word16 *scf, 
                                Word16 *minScf,
                                Word32 *sfbDist, 
=======
                                Word16 *scf,
                                Word16 *minScf,
                                Word32 *sfbDist,
>>>>>>> upstream/master
                                Word16 *sfbConstPePart,
                                Word16 *logSfbEnergy,
                                Word16 *logSfbFormFactor,
                                Word16 *sfbNRelevantLines,
                                Word16 *minScfCalculated,
                                Flag    restartOnSuccess)
{
	Word32 sfbLast, sfbAct, sfbNext, scfAct, scfMin;
	Word16 *scfLast, *scfNext;
	Word32 sfbPeOld, sfbPeNew;
	Word32 sfbDistNew;
	Word32 j;
	Flag   success;
	Word16 deltaPe, deltaPeNew, deltaPeTmp;
	Word16 *prevScfLast = psyOutChan->prevScfLast;
	Word16 *prevScfNext = psyOutChan->prevScfNext;
	Word16 *deltaPeLast = psyOutChan->deltaPeLast;
	Flag   updateMinScfCalculated;
<<<<<<< HEAD
	
	success = 0;                                                                  
	deltaPe = 0;                                                                  
	
	for(j=0;j<psyOutChan->sfbCnt;j++){
		prevScfLast[j] = MAX_16;                                                    
		prevScfNext[j] = MAX_16;                                                    
		deltaPeLast[j] = MAX_16;                                                    
	}
	
	sfbLast = -1;                                                                 
	sfbAct = -1;                                                                  
	sfbNext = -1;                                                                 
	scfLast = 0;
	scfNext = 0;
	scfMin = MAX_16;                                                              
=======

	success = 0;
	deltaPe = 0;

	for(j=0;j<psyOutChan->sfbCnt;j++){
		prevScfLast[j] = MAX_16;
		prevScfNext[j] = MAX_16;
		deltaPeLast[j] = MAX_16;
	}

	sfbLast = -1;
	sfbAct = -1;
	sfbNext = -1;
	scfLast = 0;
	scfNext = 0;
	scfMin = MAX_16;
>>>>>>> upstream/master
	do {
		/* search for new relevant sfb */
		sfbNext = sfbNext + 1;
		while (sfbNext < psyOutChan->sfbCnt && scf[sfbNext] == MIN_16) {
<<<<<<< HEAD
			
			sfbNext = sfbNext + 1;
		}
		
		if ((sfbLast>=0) && (sfbAct>=0) && sfbNext < psyOutChan->sfbCnt) {
			/* relevant scfs to the left and to the right */
			scfAct  = scf[sfbAct];                                                    
=======

			sfbNext = sfbNext + 1;
		}

		if ((sfbLast>=0) && (sfbAct>=0) && sfbNext < psyOutChan->sfbCnt) {
			/* relevant scfs to the left and to the right */
			scfAct  = scf[sfbAct];
>>>>>>> upstream/master
			scfLast = scf + sfbLast;
			scfNext = scf + sfbNext;
			scfMin  = min(*scfLast, *scfNext);
		}
		else {
<<<<<<< HEAD
			
			if (sfbLast == -1 && (sfbAct>=0) && sfbNext < psyOutChan->sfbCnt) {
				/* first relevant scf */
				scfAct  = scf[sfbAct];                                                  
				scfLast = &scfAct;
				scfNext = scf + sfbNext;
				scfMin  = *scfNext;                                                     
			}
			else {
				
				if ((sfbLast>=0) && (sfbAct>=0) && sfbNext == psyOutChan->sfbCnt) {
					/* last relevant scf */
					scfAct  = scf[sfbAct];                                                
					scfLast = scf + sfbLast;
					scfNext = &scfAct;
					scfMin  = *scfLast;                                                   
				}
			}
		}
		
		if (sfbAct>=0)
			scfMin = max(scfMin, minScf[sfbAct]);
		
		if ((sfbAct >= 0) && 
			(sfbLast>=0 || sfbNext < psyOutChan->sfbCnt) && 
			scfAct > scfMin && 
			(*scfLast != prevScfLast[sfbAct] || 
			*scfNext != prevScfNext[sfbAct] || 
			deltaPe < deltaPeLast[sfbAct])) {
			success = 0;                                                              
			
			/* estimate required bits for actual scf */			
			if (sfbConstPePart[sfbAct] == MIN_16) {
				sfbConstPePart[sfbAct] = logSfbEnergy[sfbAct] -
					logSfbFormFactor[sfbAct] + 11-8*4; /* 4*log2(6.75) - 32 */
				
=======

			if (sfbLast == -1 && (sfbAct>=0) && sfbNext < psyOutChan->sfbCnt) {
				/* first relevant scf */
				scfAct  = scf[sfbAct];
				scfLast = &scfAct;
				scfNext = scf + sfbNext;
				scfMin  = *scfNext;
			}
			else {

				if ((sfbLast>=0) && (sfbAct>=0) && sfbNext == psyOutChan->sfbCnt) {
					/* last relevant scf */
					scfAct  = scf[sfbAct];
					scfLast = scf + sfbLast;
					scfNext = &scfAct;
					scfMin  = *scfLast;
				}
			}
		}

		if (sfbAct>=0)
			scfMin = max(scfMin, minScf[sfbAct]);

		if ((sfbAct >= 0) &&
			(sfbLast>=0 || sfbNext < psyOutChan->sfbCnt) &&
			scfAct > scfMin &&
			(*scfLast != prevScfLast[sfbAct] ||
			*scfNext != prevScfNext[sfbAct] ||
			deltaPe < deltaPeLast[sfbAct])) {
			success = 0;

			/* estimate required bits for actual scf */
			if (sfbConstPePart[sfbAct] == MIN_16) {
				sfbConstPePart[sfbAct] = logSfbEnergy[sfbAct] -
					logSfbFormFactor[sfbAct] + 11-8*4; /* 4*log2(6.75) - 32 */

>>>>>>> upstream/master
				if (sfbConstPePart[sfbAct] < 0)
					sfbConstPePart[sfbAct] = sfbConstPePart[sfbAct] + 3;
				sfbConstPePart[sfbAct] = sfbConstPePart[sfbAct] >> 2;
			}
<<<<<<< HEAD
			
			sfbPeOld = calcSingleSpecPe(scfAct, sfbConstPePart[sfbAct], sfbNRelevantLines[sfbAct]) +
				countSingleScfBits(scfAct, *scfLast, *scfNext);
			deltaPeNew = deltaPe;                                                     
			updateMinScfCalculated = 1;                                               
			do {
				scfAct = scfAct - 1;
				/* check only if the same check was not done before */
				
				if (scfAct < minScfCalculated[sfbAct]) {
					sfbPeNew = calcSingleSpecPe(scfAct, sfbConstPePart[sfbAct], sfbNRelevantLines[sfbAct]) +
						countSingleScfBits(scfAct, *scfLast, *scfNext);
					/* use new scf if no increase in pe and 
					quantization error is smaller */
					deltaPeTmp = deltaPe + sfbPeNew - sfbPeOld;
					
=======

			sfbPeOld = calcSingleSpecPe(scfAct, sfbConstPePart[sfbAct], sfbNRelevantLines[sfbAct]) +
				countSingleScfBits(scfAct, *scfLast, *scfNext);
			deltaPeNew = deltaPe;
			updateMinScfCalculated = 1;
			do {
				scfAct = scfAct - 1;
				/* check only if the same check was not done before */

				if (scfAct < minScfCalculated[sfbAct]) {
					sfbPeNew = calcSingleSpecPe(scfAct, sfbConstPePart[sfbAct], sfbNRelevantLines[sfbAct]) +
						countSingleScfBits(scfAct, *scfLast, *scfNext);
					/* use new scf if no increase in pe and
					quantization error is smaller */
					deltaPeTmp = deltaPe + sfbPeNew - sfbPeOld;

>>>>>>> upstream/master
					if (deltaPeTmp < 10) {
						sfbDistNew = calcSfbDist(psyOutChan->mdctSpectrum+
							psyOutChan->sfbOffsets[sfbAct],
							(psyOutChan->sfbOffsets[sfbAct+1] - psyOutChan->sfbOffsets[sfbAct]),
							scfAct);
						if (sfbDistNew < sfbDist[sfbAct]) {
							/* success, replace scf by new one */
<<<<<<< HEAD
							scf[sfbAct] = scfAct;                                     
							sfbDist[sfbAct] = sfbDistNew;                             
							deltaPeNew = deltaPeTmp;                                  
							success = 1;                                              
						}
						/* mark as already checked */
						
						if (updateMinScfCalculated) {
							minScfCalculated[sfbAct] = scfAct;                        
						}
					}
					else {
						updateMinScfCalculated = 0;                                 
					}
				}
				
			} while (scfAct > scfMin);
			deltaPe = deltaPeNew;                                             
			/* save parameters to avoid multiple computations of the same sfb */
			prevScfLast[sfbAct] = *scfLast;                                   
			prevScfNext[sfbAct] = *scfNext;                                   
			deltaPeLast[sfbAct] = deltaPe;                                    
		}
		
		if (success && restartOnSuccess) {
			/* start again at first sfb */
			sfbLast = -1;                                                     
			sfbAct  = -1;                                                     
			sfbNext = -1;                                                     
			scfLast = 0;
			scfNext = 0;
			scfMin  = MAX_16;                                                 
			success = 0;                                                      
		}
		else {
			/* shift sfbs for next band */
			sfbLast = sfbAct;                                                 
			sfbAct  = sfbNext;                                                
		}
		
=======
							scf[sfbAct] = scfAct;
							sfbDist[sfbAct] = sfbDistNew;
							deltaPeNew = deltaPeTmp;
							success = 1;
						}
						/* mark as already checked */

						if (updateMinScfCalculated) {
							minScfCalculated[sfbAct] = scfAct;
						}
					}
					else {
						updateMinScfCalculated = 0;
					}
				}

			} while (scfAct > scfMin);
			deltaPe = deltaPeNew;
			/* save parameters to avoid multiple computations of the same sfb */
			prevScfLast[sfbAct] = *scfLast;
			prevScfNext[sfbAct] = *scfNext;
			deltaPeLast[sfbAct] = deltaPe;
		}

		if (success && restartOnSuccess) {
			/* start again at first sfb */
			sfbLast = -1;
			sfbAct  = -1;
			sfbNext = -1;
			scfLast = 0;
			scfNext = 0;
			scfMin  = MAX_16;
			success = 0;
		}
		else {
			/* shift sfbs for next band */
			sfbLast = sfbAct;
			sfbAct  = sfbNext;
		}

>>>>>>> upstream/master
  } while (sfbNext < psyOutChan->sfbCnt);
}


/*********************************************************************************
*
* function name: assimilateMultipleScf
* description:  scalefactor difference reduction
*
**********************************************************************************/
static void assimilateMultipleScf(PSY_OUT_CHANNEL *psyOutChan,
<<<<<<< HEAD
                                  Word16 *scf, 
                                  Word16 *minScf,
                                  Word32 *sfbDist, 
=======
                                  Word16 *scf,
                                  Word16 *minScf,
                                  Word32 *sfbDist,
>>>>>>> upstream/master
                                  Word16 *sfbConstPePart,
                                  Word16 *logSfbEnergy,
                                  Word16 *logSfbFormFactor,
                                  Word16 *sfbNRelevantLines)
{
	Word32 sfb, startSfb, stopSfb, scfMin, scfMax, scfAct;
	Flag   possibleRegionFound;
	Word32 deltaScfBits;
	Word32 deltaSpecPe;
	Word32 deltaPe, deltaPeNew;
	Word32 sfbCnt;
	Word32 *sfbDistNew = psyOutChan->sfbDistNew;
	Word16 *scfTmp = psyOutChan->prevScfLast;

<<<<<<< HEAD
	deltaPe = 0;                                                          
	sfbCnt = psyOutChan->sfbCnt;                                          
	
	/* calc min and max scalfactors */
	scfMin = MAX_16;                                                      
	scfMax = MIN_16;                                                      
	for (sfb=0; sfb<sfbCnt; sfb++) {
		
=======
	deltaPe = 0;
	sfbCnt = psyOutChan->sfbCnt;

	/* calc min and max scalfactors */
	scfMin = MAX_16;
	scfMax = MIN_16;
	for (sfb=0; sfb<sfbCnt; sfb++) {

>>>>>>> upstream/master
		if (scf[sfb] != MIN_16) {
			scfMin = min(scfMin, scf[sfb]);
			scfMax = max(scfMax, scf[sfb]);
		}
	}
<<<<<<< HEAD
	
	if (scfMax !=  MIN_16) {
		
		scfAct = scfMax;                                             
		
		do {
			scfAct = scfAct - 1;
			for (sfb=0; sfb<sfbCnt; sfb++) {
				scfTmp[sfb] = scf[sfb];                                         
			}
			stopSfb = 0;                                                      
			do {
				sfb = stopSfb;                                                  
				
				while (sfb < sfbCnt && (scf[sfb] == MIN_16 || scf[sfb] <= scfAct)) {
					sfb = sfb + 1;
				}
				startSfb = sfb;                                                 
				sfb = sfb + 1;
				
				while (sfb < sfbCnt && (scf[sfb] == MIN_16 || scf[sfb] > scfAct)) {
					sfb = sfb + 1;
				}
				stopSfb = sfb;                                                  
				
				possibleRegionFound = 0;                                        
				
				if (startSfb < sfbCnt) {
					possibleRegionFound = 1;                                      
					for (sfb=startSfb; sfb<stopSfb; sfb++) {
						
						if (scf[sfb]!=MIN_16) {
							
							if (scfAct < minScf[sfb]) {
								possibleRegionFound = 0;                                
=======

	if (scfMax !=  MIN_16) {

		scfAct = scfMax;

		do {
			scfAct = scfAct - 1;
			for (sfb=0; sfb<sfbCnt; sfb++) {
				scfTmp[sfb] = scf[sfb];
			}
			stopSfb = 0;
			do {
				sfb = stopSfb;

				while (sfb < sfbCnt && (scf[sfb] == MIN_16 || scf[sfb] <= scfAct)) {
					sfb = sfb + 1;
				}
				startSfb = sfb;
				sfb = sfb + 1;

				while (sfb < sfbCnt && (scf[sfb] == MIN_16 || scf[sfb] > scfAct)) {
					sfb = sfb + 1;
				}
				stopSfb = sfb;

				possibleRegionFound = 0;

				if (startSfb < sfbCnt) {
					possibleRegionFound = 1;
					for (sfb=startSfb; sfb<stopSfb; sfb++) {

						if (scf[sfb]!=MIN_16) {

							if (scfAct < minScf[sfb]) {
								possibleRegionFound = 0;
>>>>>>> upstream/master
								break;
							}
						}
					}
				}
<<<<<<< HEAD
				
				
				if (possibleRegionFound) { /* region found */
					
					/* replace scfs in region by scfAct */
					for (sfb=startSfb; sfb<stopSfb; sfb++) {
						
						if (scfTmp[sfb]!=MIN_16)
							scfTmp[sfb] = scfAct;                                     
					}
					
					/* estimate change in bit demand for new scfs */
					deltaScfBits = countScfBitsDiff(scf,scfTmp,sfbCnt,startSfb,stopSfb);
					deltaSpecPe = calcSpecPeDiff(scf, scfTmp, sfbConstPePart,
						logSfbEnergy, logSfbFormFactor, sfbNRelevantLines, 
						startSfb, stopSfb);
					deltaPeNew = deltaPe + deltaScfBits + deltaSpecPe;
					
					
					if (deltaPeNew < 10) {
						Word32 distOldSum, distNewSum;
						
						/* quantize and calc sum of new distortion */
						distOldSum = 0;                                                     
						distNewSum = 0;                                                     
						for (sfb=startSfb; sfb<stopSfb; sfb++) {
							
							if (scfTmp[sfb] != MIN_16) {
								distOldSum = L_add(distOldSum, sfbDist[sfb]);
								
								sfbDistNew[sfb] = calcSfbDist(psyOutChan->mdctSpectrum +
									psyOutChan->sfbOffsets[sfb], 
									(psyOutChan->sfbOffsets[sfb+1] - psyOutChan->sfbOffsets[sfb]),
									scfAct);
								
								
=======


				if (possibleRegionFound) { /* region found */

					/* replace scfs in region by scfAct */
					for (sfb=startSfb; sfb<stopSfb; sfb++) {

						if (scfTmp[sfb]!=MIN_16)
							scfTmp[sfb] = scfAct;
					}

					/* estimate change in bit demand for new scfs */
					deltaScfBits = countScfBitsDiff(scf,scfTmp,sfbCnt,startSfb,stopSfb);
					deltaSpecPe = calcSpecPeDiff(scf, scfTmp, sfbConstPePart,
						logSfbEnergy, logSfbFormFactor, sfbNRelevantLines,
						startSfb, stopSfb);
					deltaPeNew = deltaPe + deltaScfBits + deltaSpecPe;


					if (deltaPeNew < 10) {
						Word32 distOldSum, distNewSum;

						/* quantize and calc sum of new distortion */
						distOldSum = 0;
						distNewSum = 0;
						for (sfb=startSfb; sfb<stopSfb; sfb++) {

							if (scfTmp[sfb] != MIN_16) {
								distOldSum = L_add(distOldSum, sfbDist[sfb]);

								sfbDistNew[sfb] = calcSfbDist(psyOutChan->mdctSpectrum +
									psyOutChan->sfbOffsets[sfb],
									(psyOutChan->sfbOffsets[sfb+1] - psyOutChan->sfbOffsets[sfb]),
									scfAct);


>>>>>>> upstream/master
								if (sfbDistNew[sfb] > psyOutChan->sfbThreshold[sfb]) {
									distNewSum = distOldSum << 1;
									break;
								}
								distNewSum = L_add(distNewSum, sfbDistNew[sfb]);
							}
						}
<<<<<<< HEAD
						
						if (distNewSum < distOldSum) {
							deltaPe = deltaPeNew;                                             
							for (sfb=startSfb; sfb<stopSfb; sfb++) {
								
								if (scf[sfb]!=MIN_16) {
									scf[sfb] = scfAct;                                            
									sfbDist[sfb] = sfbDistNew[sfb];                               
=======

						if (distNewSum < distOldSum) {
							deltaPe = deltaPeNew;
							for (sfb=startSfb; sfb<stopSfb; sfb++) {

								if (scf[sfb]!=MIN_16) {
									scf[sfb] = scfAct;
									sfbDist[sfb] = sfbDistNew[sfb];
>>>>>>> upstream/master
								}
							}
						}
					}
<<<<<<< HEAD
				}        
			} while (stopSfb <= sfbCnt);      
=======
				}
			} while (stopSfb <= sfbCnt);
>>>>>>> upstream/master
		} while (scfAct > scfMin);
	}
}

/*********************************************************************************
*
* function name: EstimateScaleFactorsChannel
* description:  estimate scale factors for one channel
*
**********************************************************************************/
static void
EstimateScaleFactorsChannel(PSY_OUT_CHANNEL *psyOutChan,
                            Word16          *scf,
                            Word16          *globalGain,
                            Word16          *logSfbEnergy,
                            Word16          *logSfbFormFactor,
                            Word16          *sfbNRelevantLines)
{
	Word32 i, j;
	Word32 thresh, energy;
	Word32 energyPart, thresholdPart;
	Word32 scfInt, minScf, maxScf, maxAllowedScf, lastSf;
	Word32 maxSpec;
	Word32 *sfbDist = psyOutChan->sfbDist;
	Word16 *minSfMaxQuant = psyOutChan->minSfMaxQuant;
	Word16 *minScfCalculated = psyOutChan->minScfCalculated;
<<<<<<< HEAD
	
	
	for (i=0; i<psyOutChan->sfbCnt; i++) {
		Word32 sbfwith, sbfStart;
		Word32 *mdctSpec;
		thresh = psyOutChan->sfbThreshold[i];                                       
		energy = psyOutChan->sfbEnergy[i];                                          
		
		sbfStart = psyOutChan->sfbOffsets[i];
		sbfwith = psyOutChan->sfbOffsets[i+1] - sbfStart;
		mdctSpec = psyOutChan->mdctSpectrum+sbfStart;
		
		maxSpec = 0;                                                                
		/* maximum of spectrum */
		for (j=sbfwith; j; j-- ) {
			Word32 absSpec = L_abs(*mdctSpec); mdctSpec++;
			maxSpec |= absSpec;                                                       
		}
		
		/* scfs without energy or with thresh>energy are marked with MIN_16 */
		scf[i] = MIN_16;                                                            
		minSfMaxQuant[i] = MIN_16;    
		
		if ((maxSpec > 0) && (energy > thresh)) {
			
			energyPart = logSfbFormFactor[i];                                         
			thresholdPart = iLog4(thresh);  
			/* -20 = 4*log2(6.75) - 32 */
			scfInt = ((thresholdPart - energyPart - 20) * SCALE_ESTIMATE_COEF) >> 15;
			
			minSfMaxQuant[i] = iLog4(maxSpec) - 68; /* 68  -16/3*log(MAX_QUANT+0.5-logCon)/log(2) + 1 */
			
			
			if (minSfMaxQuant[i] > scfInt) {
				scfInt = minSfMaxQuant[i];                                              
			}
			
			/* find better scalefactor with analysis by synthesis */
			scfInt = improveScf(psyOutChan->mdctSpectrum+sbfStart,
				sbfwith,
				thresh, scfInt, minSfMaxQuant[i], 
				&sfbDist[i], &minScfCalculated[i]);
			
			scf[i] = scfInt;                                                          
		}
	}
	
	
=======


	for (i=0; i<psyOutChan->sfbCnt; i++) {
		Word32 sbfwith, sbfStart;
		Word32 *mdctSpec;
		thresh = psyOutChan->sfbThreshold[i];
		energy = psyOutChan->sfbEnergy[i];

		sbfStart = psyOutChan->sfbOffsets[i];
		sbfwith = psyOutChan->sfbOffsets[i+1] - sbfStart;
		mdctSpec = psyOutChan->mdctSpectrum+sbfStart;

		maxSpec = 0;
		/* maximum of spectrum */
		for (j=sbfwith; j; j-- ) {
			Word32 absSpec = L_abs(*mdctSpec); mdctSpec++;
			maxSpec |= absSpec;
		}

		/* scfs without energy or with thresh>energy are marked with MIN_16 */
		scf[i] = MIN_16;
		minSfMaxQuant[i] = MIN_16;

		if ((maxSpec > 0) && (energy > thresh)) {

			energyPart = logSfbFormFactor[i];
			thresholdPart = iLog4(thresh);
			/* -20 = 4*log2(6.75) - 32 */
			scfInt = ((thresholdPart - energyPart - 20) * SCALE_ESTIMATE_COEF) >> 15;

			minSfMaxQuant[i] = iLog4(maxSpec) - 68; /* 68  -16/3*log(MAX_QUANT+0.5-logCon)/log(2) + 1 */


			if (minSfMaxQuant[i] > scfInt) {
				scfInt = minSfMaxQuant[i];
			}

			/* find better scalefactor with analysis by synthesis */
			scfInt = improveScf(psyOutChan->mdctSpectrum+sbfStart,
				sbfwith,
				thresh, scfInt, minSfMaxQuant[i],
				&sfbDist[i], &minScfCalculated[i]);

			scf[i] = scfInt;
		}
	}


>>>>>>> upstream/master
	/* scalefactor differece reduction  */
	{
		Word16 sfbConstPePart[MAX_GROUPED_SFB];
		for(i=0;i<psyOutChan->sfbCnt;i++) {
<<<<<<< HEAD
			sfbConstPePart[i] = MIN_16;                                               
		}
		
		assimilateSingleScf(psyOutChan, scf, 
			minSfMaxQuant, sfbDist, sfbConstPePart, logSfbEnergy,
			logSfbFormFactor, sfbNRelevantLines, minScfCalculated, 1);
		
		assimilateMultipleScf(psyOutChan, scf, 
=======
			sfbConstPePart[i] = MIN_16;
		}

		assimilateSingleScf(psyOutChan, scf,
			minSfMaxQuant, sfbDist, sfbConstPePart, logSfbEnergy,
			logSfbFormFactor, sfbNRelevantLines, minScfCalculated, 1);

		assimilateMultipleScf(psyOutChan, scf,
>>>>>>> upstream/master
			minSfMaxQuant, sfbDist, sfbConstPePart, logSfbEnergy,
			logSfbFormFactor, sfbNRelevantLines);
	}

	/* get max scalefac for global gain */
<<<<<<< HEAD
	maxScf = MIN_16;                                                              
	minScf = MAX_16;                                                              
	for (i=0; i<psyOutChan->sfbCnt; i++) {
		
		if (maxScf < scf[i]) {
			maxScf = scf[i];                                                          
		}
		
		if ((scf[i] != MIN_16) && (minScf > scf[i])) {
			minScf = scf[i];                                                          
=======
	maxScf = MIN_16;
	minScf = MAX_16;
	for (i=0; i<psyOutChan->sfbCnt; i++) {

		if (maxScf < scf[i]) {
			maxScf = scf[i];
		}

		if ((scf[i] != MIN_16) && (minScf > scf[i])) {
			minScf = scf[i];
>>>>>>> upstream/master
		}
	}
	/* limit scf delta */
	maxAllowedScf = minScf + MAX_SCF_DELTA;
	for(i=0; i<psyOutChan->sfbCnt; i++) {
<<<<<<< HEAD
		
		if ((scf[i] != MIN_16) && (maxAllowedScf < scf[i])) {
			scf[i] = maxAllowedScf;                                                   
		}
	}
	/* new maxScf if any scf has been limited */
	
	if (maxAllowedScf < maxScf) {
		maxScf = maxAllowedScf;                                                     
	}
	
	/* calc loop scalefactors */
	
	if (maxScf > MIN_16) {
		*globalGain = maxScf;                                                       
		lastSf = 0;                                                                 
		
		for(i=0; i<psyOutChan->sfbCnt; i++) {
			
			if (scf[i] == MIN_16) {
				scf[i] = lastSf;                                                        
				/* set band explicitely to zero */
				for (j=psyOutChan->sfbOffsets[i]; j<psyOutChan->sfbOffsets[i+1]; j++) {
					psyOutChan->mdctSpectrum[j] = 0;                                      
=======

		if ((scf[i] != MIN_16) && (maxAllowedScf < scf[i])) {
			scf[i] = maxAllowedScf;
		}
	}
	/* new maxScf if any scf has been limited */

	if (maxAllowedScf < maxScf) {
		maxScf = maxAllowedScf;
	}

	/* calc loop scalefactors */

	if (maxScf > MIN_16) {
		*globalGain = maxScf;
		lastSf = 0;

		for(i=0; i<psyOutChan->sfbCnt; i++) {

			if (scf[i] == MIN_16) {
				scf[i] = lastSf;
				/* set band explicitely to zero */
				for (j=psyOutChan->sfbOffsets[i]; j<psyOutChan->sfbOffsets[i+1]; j++) {
					psyOutChan->mdctSpectrum[j] = 0;
>>>>>>> upstream/master
				}
			}
			else {
				scf[i] = maxScf - scf[i];
<<<<<<< HEAD
				lastSf = scf[i];                                                        
=======
				lastSf = scf[i];
>>>>>>> upstream/master
			}
		}
	}
	else{
<<<<<<< HEAD
		*globalGain = 0;                                                            
		/* set spectrum explicitely to zero */
		for(i=0; i<psyOutChan->sfbCnt; i++) {
			scf[i] = 0;                                                               
			for (j=psyOutChan->sfbOffsets[i]; j<psyOutChan->sfbOffsets[i+1]; j++) {
				psyOutChan->mdctSpectrum[j] = 0;                                        
=======
		*globalGain = 0;
		/* set spectrum explicitely to zero */
		for(i=0; i<psyOutChan->sfbCnt; i++) {
			scf[i] = 0;
			for (j=psyOutChan->sfbOffsets[i]; j<psyOutChan->sfbOffsets[i+1]; j++) {
				psyOutChan->mdctSpectrum[j] = 0;
>>>>>>> upstream/master
			}
		}
	}
}

/*********************************************************************************
*
* function name: CalcFormFactor
* description:  estimate Form factors for all channel
*
**********************************************************************************/
void
CalcFormFactor(Word16 logSfbFormFactor[MAX_CHANNELS][MAX_GROUPED_SFB],
               Word16 sfbNRelevantLines[MAX_CHANNELS][MAX_GROUPED_SFB],
               Word16 logSfbEnergy[MAX_CHANNELS][MAX_GROUPED_SFB],
               PSY_OUT_CHANNEL psyOutChannel[MAX_CHANNELS],
               const Word16 nChannels)
{
	Word16 j;
<<<<<<< HEAD
	
=======

>>>>>>> upstream/master
	for (j=0; j<nChannels; j++) {
		CalcFormFactorChannel(logSfbFormFactor[j], sfbNRelevantLines[j], logSfbEnergy[j], &psyOutChannel[j]);
	}
}

/*********************************************************************************
*
* function name: EstimateScaleFactors
* description:  estimate scale factors for all channel
*
**********************************************************************************/
void
EstimateScaleFactors(PSY_OUT_CHANNEL psyOutChannel[MAX_CHANNELS],
                     QC_OUT_CHANNEL  qcOutChannel[MAX_CHANNELS],
                     Word16          logSfbEnergy[MAX_CHANNELS][MAX_GROUPED_SFB],
                     Word16          logSfbFormFactor[MAX_CHANNELS][MAX_GROUPED_SFB],
                     Word16          sfbNRelevantLines[MAX_CHANNELS][MAX_GROUPED_SFB],
                     const Word16    nChannels)
{
	Word16 j;
<<<<<<< HEAD
	
=======

>>>>>>> upstream/master
	for (j=0; j<nChannels; j++) {
		EstimateScaleFactorsChannel(&psyOutChannel[j],
			qcOutChannel[j].scf,
			&(qcOutChannel[j].globalGain),
			logSfbEnergy[j],
			logSfbFormFactor[j],
			sfbNRelevantLines[j]);
	}
}

