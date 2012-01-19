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
	File:		interface.c

	Content:	Interface psychoaccoustic/quantizer functions

*******************************************************************************/

#include "basic_op.h"
#include "oper_32b.h"
#include "psy_const.h"
#include "interface.h"

/*****************************************************************************
*
* function name: BuildInterface
* description:  update output parameter
*
**********************************************************************************/
void BuildInterface(Word32                  *groupedMdctSpectrum,
                    const Word16             mdctScale,
                    SFB_THRESHOLD           *groupedSfbThreshold,
                    SFB_ENERGY              *groupedSfbEnergy,
                    SFB_ENERGY              *groupedSfbSpreadedEnergy,
                    const SFB_ENERGY_SUM     sfbEnergySumLR,
                    const SFB_ENERGY_SUM     sfbEnergySumMS,
                    const Word16             windowSequence,
                    const Word16             windowShape,
                    const Word16             groupedSfbCnt,
                    const Word16            *groupedSfbOffset,
                    const Word16             maxSfbPerGroup,
                    const Word16            *groupedSfbMinSnr,
                    const Word16             noOfGroups,
                    const Word16            *groupLen,
                    PSY_OUT_CHANNEL         *psyOutCh)
{
  Word32 j;
<<<<<<< HEAD
  Word32 grp; 
=======
  Word32 grp;
>>>>>>> upstream/master
  Word32 mask;
  Word16 *tmpV;

  /*
  copy values to psyOut
  */
<<<<<<< HEAD
  psyOutCh->maxSfbPerGroup    = maxSfbPerGroup;                             
  psyOutCh->sfbCnt            = groupedSfbCnt;   
=======
  psyOutCh->maxSfbPerGroup    = maxSfbPerGroup;
  psyOutCh->sfbCnt            = groupedSfbCnt;
>>>>>>> upstream/master
  if(noOfGroups)
	psyOutCh->sfbPerGroup     = groupedSfbCnt/ noOfGroups;
  else
	psyOutCh->sfbPerGroup     = 0x7fff;
<<<<<<< HEAD
  psyOutCh->windowSequence    = windowSequence;                             
  psyOutCh->windowShape       = windowShape;                                
  psyOutCh->mdctScale         = mdctScale;                                  
=======
  psyOutCh->windowSequence    = windowSequence;
  psyOutCh->windowShape       = windowShape;
  psyOutCh->mdctScale         = mdctScale;
>>>>>>> upstream/master
  psyOutCh->mdctSpectrum      = groupedMdctSpectrum;
  psyOutCh->sfbEnergy         = groupedSfbEnergy->sfbLong;
  psyOutCh->sfbThreshold      = groupedSfbThreshold->sfbLong;
  psyOutCh->sfbSpreadedEnergy = groupedSfbSpreadedEnergy->sfbLong;
<<<<<<< HEAD
  
=======

>>>>>>> upstream/master
  tmpV = psyOutCh->sfbOffsets;
  for(j=0; j<groupedSfbCnt + 1; j++) {
      *tmpV++ = groupedSfbOffset[j];
  }
<<<<<<< HEAD
  
=======

>>>>>>> upstream/master
  tmpV = psyOutCh->sfbMinSnr;
  for(j=0;j<groupedSfbCnt; j++) {
	  *tmpV++ =   groupedSfbMinSnr[j];
  }
<<<<<<< HEAD
  
  /* generate grouping mask */
  mask = 0;                                                                      
=======

  /* generate grouping mask */
  mask = 0;
>>>>>>> upstream/master
  for (grp = 0; grp < noOfGroups; grp++) {
    mask = mask << 1;
    for (j=1; j<groupLen[grp]; j++) {
      mask = mask << 1;
<<<<<<< HEAD
      mask |= 1;                                                                 
    }
  }
  psyOutCh->groupingMask = mask; 
  
  if (windowSequence != SHORT_WINDOW) {
    psyOutCh->sfbEnSumLR =  sfbEnergySumLR.sfbLong;                              
    psyOutCh->sfbEnSumMS =  sfbEnergySumMS.sfbLong;                              
=======
      mask |= 1;
    }
  }
  psyOutCh->groupingMask = mask;

  if (windowSequence != SHORT_WINDOW) {
    psyOutCh->sfbEnSumLR =  sfbEnergySumLR.sfbLong;
    psyOutCh->sfbEnSumMS =  sfbEnergySumMS.sfbLong;
>>>>>>> upstream/master
  }
  else {
    Word32 i;
    Word32 accuSumMS=0;
<<<<<<< HEAD
    Word32 accuSumLR=0;        
=======
    Word32 accuSumLR=0;
>>>>>>> upstream/master
	Word32 *pSumMS = sfbEnergySumMS.sfbShort;
	Word32 *pSumLR = sfbEnergySumLR.sfbShort;

    for (i=TRANS_FAC; i; i--) {
      accuSumLR = L_add(accuSumLR, *pSumLR); pSumLR++;
      accuSumMS = L_add(accuSumMS, *pSumMS); pSumMS++;
    }
<<<<<<< HEAD
    psyOutCh->sfbEnSumMS = accuSumMS;                                            
    psyOutCh->sfbEnSumLR = accuSumLR;                                            
=======
    psyOutCh->sfbEnSumMS = accuSumMS;
    psyOutCh->sfbEnSumLR = accuSumLR;
>>>>>>> upstream/master
  }
}
