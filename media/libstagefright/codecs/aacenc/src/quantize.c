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
	File:		quantize.c

	Content:	quantization functions

*******************************************************************************/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "quantize.h"
#include "aac_rom.h"

#define MANT_DIGITS 9
#define MANT_SIZE   (1<<MANT_DIGITS)

static const Word32 XROUND = 0x33e425af; /* final rounding constant (-0.0946f+ 0.5f) */


/*****************************************************************************
*
<<<<<<< HEAD
* function name:pow34 
* description: calculate $x^{\frac{3}{4}}, for 0.5 < x < 1.0$.  
=======
* function name:pow34
* description: calculate $x^{\frac{3}{4}}, for 0.5 < x < 1.0$.
>>>>>>> upstream/master
*
*****************************************************************************/
__inline Word32 pow34(Word32 x)
{
  /* index table using MANT_DIGITS bits, but mask out the sign bit and the MSB
<<<<<<< HEAD
     which is always one */   
=======
     which is always one */
>>>>>>> upstream/master
  return mTab_3_4[(x >> (INT_BITS-2-MANT_DIGITS)) & (MANT_SIZE-1)];
}


/*****************************************************************************
*
<<<<<<< HEAD
* function name:quantizeSingleLine 
* description: quantizes spectrum  
*              quaSpectrum = mdctSpectrum^3/4*2^(-(3/16)*gain)    
=======
* function name:quantizeSingleLine
* description: quantizes spectrum
*              quaSpectrum = mdctSpectrum^3/4*2^(-(3/16)*gain)
>>>>>>> upstream/master
*
*****************************************************************************/
static Word16 quantizeSingleLine(const Word16 gain, const Word32 absSpectrum)
{
  Word32 e, minusFinalExp, finalShift;
  Word32 x;
<<<<<<< HEAD
  Word16 qua = 0;                        

   
=======
  Word16 qua = 0;


>>>>>>> upstream/master
  if (absSpectrum) {
    e = norm_l(absSpectrum);
    x = pow34(absSpectrum << e);

    /* calculate the final fractional exponent times 16 (was 3*(4*e + gain) + (INT_BITS-1)*16) */
    minusFinalExp = (e << 2) + gain;
    minusFinalExp = (minusFinalExp << 1) + minusFinalExp;
    minusFinalExp = minusFinalExp + ((INT_BITS-1) << 4);

    /* separate the exponent into a shift, and a multiply */
    finalShift = minusFinalExp >> 4;
<<<<<<< HEAD
     
=======

>>>>>>> upstream/master
    if (finalShift < INT_BITS) {
      x = L_mpy_wx(x, pow2tominusNover16[minusFinalExp & 15]);

      x += XROUND >> (INT_BITS - finalShift);

      /* shift and quantize */
	  finalShift--;

	  if(finalShift >= 0)
		  x >>= finalShift;
	  else
		  x <<= (-finalShift);
<<<<<<< HEAD
		
=======

>>>>>>> upstream/master
	  qua = saturate(x);
    }
  }

  return qua;
}

/*****************************************************************************
*
<<<<<<< HEAD
* function name:quantizeLines 
* description: quantizes spectrum lines  
*              quaSpectrum = mdctSpectrum^3/4*2^(-(3/16)*gain)    
*  input: global gain, number of lines to process, spectral data         
=======
* function name:quantizeLines
* description: quantizes spectrum lines
*              quaSpectrum = mdctSpectrum^3/4*2^(-(3/16)*gain)
*  input: global gain, number of lines to process, spectral data
>>>>>>> upstream/master
*  output: quantized spectrum
*
*****************************************************************************/
static void quantizeLines(const Word16 gain,
                          const Word16 noOfLines,
                          const Word32 *mdctSpectrum,
                          Word16 *quaSpectrum)
{
  Word32 line;
  Word32 m = gain&3;
  Word32 g = (gain >> 2) + 4;
  Word32 mdctSpeL;
  Word16 *pquat;
    /* gain&3 */

  pquat = quantBorders[m];

  g += 16;
<<<<<<< HEAD
  
=======

>>>>>>> upstream/master
  if(g >= 0)
  {
	for (line=0; line<noOfLines; line++) {
	  Word32 qua;
<<<<<<< HEAD
	  qua = 0;                                                     
    
	  mdctSpeL = mdctSpectrum[line];
	
=======
	  qua = 0;

	  mdctSpeL = mdctSpectrum[line];

>>>>>>> upstream/master
	  if (mdctSpeL) {
		Word32 sa;
		Word32 saShft;

        sa = L_abs(mdctSpeL);
        //saShft = L_shr(sa, 16 + g);
	    saShft = sa >> g;

        if (saShft > pquat[0]) {
<<<<<<< HEAD
         
          if (saShft < pquat[1]) {
             
            qua = mdctSpeL>0 ? 1 : -1;
		  }
          else {
           
            if (saShft < pquat[2]) {
               
              qua = mdctSpeL>0 ? 2 : -2;
			}
            else {
             
              if (saShft < pquat[3]) {
                 
=======

          if (saShft < pquat[1]) {

            qua = mdctSpeL>0 ? 1 : -1;
		  }
          else {

            if (saShft < pquat[2]) {

              qua = mdctSpeL>0 ? 2 : -2;
			}
            else {

              if (saShft < pquat[3]) {

>>>>>>> upstream/master
                qua = mdctSpeL>0 ? 3 : -3;
			  }
              else {
                qua = quantizeSingleLine(gain, sa);
                /* adjust the sign. Since 0 < qua < 1, this cannot overflow. */
<<<<<<< HEAD
               
=======

>>>>>>> upstream/master
                if (mdctSpeL < 0)
                  qua = -qua;
			  }
			}
		  }
		}
	  }
<<<<<<< HEAD
      quaSpectrum[line] = qua ;                                    
=======
      quaSpectrum[line] = qua ;
>>>>>>> upstream/master
	}
  }
  else
  {
	for (line=0; line<noOfLines; line++) {
	  Word32 qua;
<<<<<<< HEAD
	  qua = 0;                                                     
    
	  mdctSpeL = mdctSpectrum[line];
	
=======
	  qua = 0;

	  mdctSpeL = mdctSpectrum[line];

>>>>>>> upstream/master
	  if (mdctSpeL) {
		Word32 sa;
		Word32 saShft;

        sa = L_abs(mdctSpeL);
        saShft = sa << g;

        if (saShft > pquat[0]) {
<<<<<<< HEAD
         
          if (saShft < pquat[1]) {
             
            qua = mdctSpeL>0 ? 1 : -1;
		  }
          else {
           
            if (saShft < pquat[2]) {
               
              qua = mdctSpeL>0 ? 2 : -2;
			}
            else {
             
              if (saShft < pquat[3]) {
                 
=======

          if (saShft < pquat[1]) {

            qua = mdctSpeL>0 ? 1 : -1;
		  }
          else {

            if (saShft < pquat[2]) {

              qua = mdctSpeL>0 ? 2 : -2;
			}
            else {

              if (saShft < pquat[3]) {

>>>>>>> upstream/master
                qua = mdctSpeL>0 ? 3 : -3;
			  }
              else {
                qua = quantizeSingleLine(gain, sa);
                /* adjust the sign. Since 0 < qua < 1, this cannot overflow. */
<<<<<<< HEAD
               
=======

>>>>>>> upstream/master
                if (mdctSpeL < 0)
                  qua = -qua;
			  }
			}
		  }
		}
	  }
<<<<<<< HEAD
      quaSpectrum[line] = qua ;                                    
	}	  
=======
      quaSpectrum[line] = qua ;
	}
>>>>>>> upstream/master
  }

}


/*****************************************************************************
*
<<<<<<< HEAD
* function name:iquantizeLines 
* description: iquantizes spectrum lines without sign
*              mdctSpectrum = iquaSpectrum^4/3 *2^(0.25*gain) 
* input: global gain, number of lines to process,quantized spectrum        
=======
* function name:iquantizeLines
* description: iquantizes spectrum lines without sign
*              mdctSpectrum = iquaSpectrum^4/3 *2^(0.25*gain)
* input: global gain, number of lines to process,quantized spectrum
>>>>>>> upstream/master
* output: spectral data
*
*****************************************************************************/
static void iquantizeLines(const Word16 gain,
                           const Word16 noOfLines,
                           const Word16 *quantSpectrum,
                           Word32 *mdctSpectrum)
{
  Word32   iquantizermod;
  Word32   iquantizershift;
  Word32   line;

<<<<<<< HEAD
  iquantizermod = gain & 3;                              
  iquantizershift = gain >> 2;

  for (line=0; line<noOfLines; line++) {
     
=======
  iquantizermod = gain & 3;
  iquantizershift = gain >> 2;

  for (line=0; line<noOfLines; line++) {

>>>>>>> upstream/master
    if( quantSpectrum[line] != 0 ) {
      Word32 accu;
      Word32 ex;
	  Word32 tabIndex;
      Word32 specExp;
      Word32 s,t;

      accu = quantSpectrum[line];

      ex = norm_l(accu);
      accu = accu << ex;
      specExp = INT_BITS-1 - ex;

<<<<<<< HEAD
      tabIndex = (accu >> (INT_BITS-2-MANT_DIGITS)) & (~MANT_SIZE);        

      /* calculate "mantissa" ^4/3 */
      s = mTab_4_3[tabIndex];                                                    

      /* get approperiate exponent multiplier for specExp^3/4 combined with scfMod */
      t = specExpMantTableComb_enc[iquantizermod][specExp];                      
=======
      tabIndex = (accu >> (INT_BITS-2-MANT_DIGITS)) & (~MANT_SIZE);

      /* calculate "mantissa" ^4/3 */
      s = mTab_4_3[tabIndex];

      /* get approperiate exponent multiplier for specExp^3/4 combined with scfMod */
      t = specExpMantTableComb_enc[iquantizermod][specExp];
>>>>>>> upstream/master

      /* multiply "mantissa" ^4/3 with exponent multiplier */
      accu = MULHIGH(s, t);

      /* get approperiate exponent shifter */
<<<<<<< HEAD
      specExp = specExpTableComb_enc[iquantizermod][specExp];                    
=======
      specExp = specExpTableComb_enc[iquantizermod][specExp];
>>>>>>> upstream/master

      specExp += iquantizershift + 1;
	  if(specExp >= 0)
		  mdctSpectrum[line] = accu << specExp;
	  else
		  mdctSpectrum[line] = accu >> (-specExp);
    }
    else {
<<<<<<< HEAD
      mdctSpectrum[line] = 0;                                                    
=======
      mdctSpectrum[line] = 0;
>>>>>>> upstream/master
    }
  }
}

/*****************************************************************************
*
* function name: QuantizeSpectrum
* description: quantizes the entire spectrum
* returns:
* input: number of scalefactor bands to be quantized, ...
* output: quantized spectrum
*
*****************************************************************************/
void QuantizeSpectrum(Word16 sfbCnt,
                      Word16 maxSfbPerGroup,
                      Word16 sfbPerGroup,
                      Word16 *sfbOffset,
                      Word32 *mdctSpectrum,
                      Word16 globalGain,
                      Word16 *scalefactors,
                      Word16 *quantizedSpectrum)
{
  Word32 sfbOffs, sfb;

  for(sfbOffs=0;sfbOffs<sfbCnt;sfbOffs+=sfbPerGroup) {
    Word32 sfbNext ;
    for (sfb = 0; sfb < maxSfbPerGroup; sfb = sfbNext) {
<<<<<<< HEAD
      Word16 scalefactor = scalefactors[sfbOffs+sfb];                          
=======
      Word16 scalefactor = scalefactors[sfbOffs+sfb];
>>>>>>> upstream/master
      /* coalesce sfbs with the same scalefactor */
      for (sfbNext = sfb+1;
           sfbNext < maxSfbPerGroup && scalefactor == scalefactors[sfbOffs+sfbNext];
           sfbNext++) ;

      quantizeLines(globalGain - scalefactor,
                    sfbOffset[sfbOffs+sfbNext] - sfbOffset[sfbOffs+sfb],
                    mdctSpectrum + sfbOffset[sfbOffs+sfb],
                    quantizedSpectrum + sfbOffset[sfbOffs+sfb]);
    }
  }
}


/*****************************************************************************
*
<<<<<<< HEAD
* function name:calcSfbDist 
=======
* function name:calcSfbDist
>>>>>>> upstream/master
* description: quantizes and requantizes lines to calculate distortion
* input:  number of lines to be quantized, ...
* output: distortion
*
*****************************************************************************/
Word32 calcSfbDist(const Word32 *spec,
                   Word16  sfbWidth,
                   Word16  gain)
{
  Word32 line;
  Word32 dist;
  Word32 m = gain&3;
  Word32 g = (gain >> 2) + 4;
  Word32 g2 = (g << 1) + 1;
  Word16 *pquat, *repquat;
    /* gain&3 */

  pquat = quantBorders[m];
  repquat = quantRecon[m];
<<<<<<< HEAD
	
  dist = 0;  
  g += 16;
  if(g2 < 0 && g >= 0)
  {	  
	  g2 = -g2;
	  for(line=0; line<sfbWidth; line++) {		  
		  if (spec[line]) {			  
=======

  dist = 0;
  g += 16;
  if(g2 < 0 && g >= 0)
  {
	  g2 = -g2;
	  for(line=0; line<sfbWidth; line++) {
		  if (spec[line]) {
>>>>>>> upstream/master
			  Word32 diff;
			  Word32 distSingle;
			  Word32 sa;
			  Word32 saShft;
			  sa = L_abs(spec[line]);
			  //saShft = round16(L_shr(sa, g));
			  //saShft = L_shr(sa, 16+g);
			  saShft = sa >> g;

			  if (saShft < pquat[0]) {
				  distSingle = (saShft * saShft) >> g2;
			  }
			  else {
<<<<<<< HEAD
				  
=======

>>>>>>> upstream/master
				  if (saShft < pquat[1]) {
					  diff = saShft - repquat[0];
					  distSingle = (diff * diff) >> g2;
				  }
				  else {
<<<<<<< HEAD
					  
=======

>>>>>>> upstream/master
					  if (saShft < pquat[2]) {
						  diff = saShft - repquat[1];
						  distSingle = (diff * diff) >> g2;
					  }
					  else {
<<<<<<< HEAD
						  
=======

>>>>>>> upstream/master
						  if (saShft < pquat[3]) {
							  diff = saShft - repquat[2];
							  distSingle = (diff * diff) >> g2;
						  }
						  else {
							  Word16 qua = quantizeSingleLine(gain, sa);
							  Word32 iqval, diff32;
							  /* now that we have quantized x, re-quantize it. */
							  iquantizeLines(gain, 1, &qua, &iqval);
							  diff32 = sa - iqval;
							  distSingle = fixmul(diff32, diff32);
						  }
					  }
				  }
			  }
<<<<<<< HEAD
			  
=======

>>>>>>> upstream/master
			  dist = L_add(dist, distSingle);
		  }
	  }
  }
  else
  {
<<<<<<< HEAD
	  for(line=0; line<sfbWidth; line++) {		  
		  if (spec[line]) {			  
=======
	  for(line=0; line<sfbWidth; line++) {
		  if (spec[line]) {
>>>>>>> upstream/master
			  Word32 diff;
			  Word32 distSingle;
			  Word32 sa;
			  Word32 saShft;
			  sa = L_abs(spec[line]);
			  //saShft = round16(L_shr(sa, g));
			  saShft = L_shr(sa, g);

			  if (saShft < pquat[0]) {
				  distSingle = L_shl((saShft * saShft), g2);
			  }
			  else {
<<<<<<< HEAD
				  
=======

>>>>>>> upstream/master
				  if (saShft < pquat[1]) {
					  diff = saShft - repquat[0];
					  distSingle = L_shl((diff * diff), g2);
				  }
				  else {
<<<<<<< HEAD
					  
=======

>>>>>>> upstream/master
					  if (saShft < pquat[2]) {
						  diff = saShft - repquat[1];
						  distSingle = L_shl((diff * diff), g2);
					  }
					  else {
<<<<<<< HEAD
						  
=======

>>>>>>> upstream/master
						  if (saShft < pquat[3]) {
							  diff = saShft - repquat[2];
							  distSingle = L_shl((diff * diff), g2);
						  }
						  else {
							  Word16 qua = quantizeSingleLine(gain, sa);
							  Word32 iqval, diff32;
							  /* now that we have quantized x, re-quantize it. */
							  iquantizeLines(gain, 1, &qua, &iqval);
							  diff32 = sa - iqval;
							  distSingle = fixmul(diff32, diff32);
						  }
					  }
				  }
			  }
			  dist = L_add(dist, distSingle);
		  }
<<<<<<< HEAD
	  }	  
=======
	  }
>>>>>>> upstream/master
  }

  return dist;
}
