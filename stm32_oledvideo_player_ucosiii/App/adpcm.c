#include "adpcm.h"


/* Quantizer step size lookup table */
static const uint16_t StepSizeTable[89]={7,8,9,10,11,12,13,14,16,17,
                            19,21,23,25,28,31,34,37,41,45,
                            50,55,60,66,73,80,88,97,107,118,
                            130,143,157,173,190,209,230,253,279,307,
                            337,371,408,449,494,544,598,658,724,796,
                            876,963,1060,1166,1282,1411,1552,1707,1878,2066,
                            2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,
                            5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,
                            15289,16818,18500,20350,22385,24623,27086,29794,32767};
/* Table of index changes */
static const int8_t IndexTable[16]={0xff,0xff,0xff,0xff,2,4,6,8,0xff,0xff,0xff,0xff,2,4,6,8};

/**
  * @brief  ADPCM_Encode.
  * @param sample: a 16-bit PCM sample
  * @retval : a 4-bit ADPCM sample
  */
uint8_t ADPCM_Encode(int32_t sample)
{
  static int16_t  index = 0;
  static int32_t predsample = 0;
  uint8_t code=0;
  uint16_t tmpstep=0;
  int32_t diff=0;
  int32_t diffq=0;
  uint16_t step=0;
  
  step = StepSizeTable[index];

  /* 2. compute diff and record sign and absolut value */
  diff = sample-predsample;
  if (diff < 0)  
  {
    code=8;
    diff = -diff;
  }    
  
  /* 3. quantize the diff into ADPCM code */
  /* 4. inverse quantize the code into a predicted diff */
  tmpstep = step;
  diffq = (step >> 3);

  if (diff >= tmpstep)
  {
    code |= 0x04;
    diff -= tmpstep;
    diffq += step;
  }
  
  tmpstep = tmpstep >> 1;

  if (diff >= tmpstep)
  {
    code |= 0x02;
    diff -= tmpstep;
    diffq+=(step >> 1);
  }
  
  tmpstep = tmpstep >> 1;
  
  if (diff >= tmpstep)
  {
    code |=0x01;
    diffq+=(step >> 2);
  }
  
  /* 5. fixed predictor to get new predicted sample*/
  if (code & 8)
  {
    predsample -= diffq;
  }
  else
  {
    predsample += diffq;
  }  

  /* check for overflow*/
  if (predsample > 32767)
  {
    predsample = 32767;
  }
  else if (predsample < -32768)
  {
    predsample = -32768;
  }
  
  /* 6. find new stepsize index */
  index += IndexTable[code];
  /* check for overflow*/
  if (index <0)
  {
    index = 0;
  }
  else if (index > 88)
  {
    index = 88;
  }
  
  /* 8. return new ADPCM code*/
  return (code & 0x0f);
}



/**
  * @brief  ADPCM_Decode.
  * @param code: a byte containing a 4-bit ADPCM sample. 
  * @retval : 16-bit ADPCM sample
  */
int16_t ADPCM_Decode(uint8_t code)
{
  static int16_t  index = 0;
  static int32_t predsample = 0;
  uint16_t step=0;
  int32_t diffq=0;
  
  step = StepSizeTable[index];

  /* 2. inverse code into diff */
  diffq = step>> 3;
  if (code&4)
  {
    diffq += step;
  }
  
  if (code&2)
  {
    diffq += step>>1;
  }
  
  if (code&1)
  {
    diffq += step>>2;
  }

  /* 3. add diff to predicted sample*/
  if (code&8)
  {
    predsample -= diffq;
  }
  else
  {
    predsample += diffq;
  }
  
  /* check for overflow*/
  if (predsample > 32767)
  {
    predsample = 32767;
  }
  else if (predsample < -32768)
  {
    predsample = -32768;
  }

  /* 4. find new quantizer step size */
  index += IndexTable [code];
  /* check for overflow*/
  if (index < 0)
  {
    index = 0;
  }
  if (index > 88)
  {
    index = 88;
  }
  
  /* 5. save predict sample and index for next iteration */
  /* done! static variables */
  
  /* 6. return new speech sample*/
  return ((int16_t)predsample);
}


void Adpcm_FrameEncode(const int16_t *PCMBuffer, void *EncodeBuffer, int32_t Len)
{
	int32_t i;
	uint8_t HighBits;
	uint8_t Code;
	uint8_t *pCode;
	
	HighBits = 0;
	pCode = (uint8_t *)EncodeBuffer;
	for (i=0; i<Len; i++) {
		Code = ADPCM_Encode(PCMBuffer[i]);
		if (HighBits) {
			*pCode |= Code << 4;
			pCode++;
			HighBits = 0;
		} else {
			*pCode = Code;
			HighBits = 1;
		}
	}
}

void Adpcm_FrameDecode(int16_t *PCMBuffer, const void *EncodeBuffer, int32_t Len)
{
	int32_t i;
	uint8_t HighBits;
	uint8_t Code;
	const uint8_t *pCode;
	
	HighBits = 0;
	pCode = EncodeBuffer;
	for (i=0; i<Len; i++) {
		if (HighBits) {
			Code = *pCode >> 4;
			pCode++;
			HighBits = 0;
		} else {
			Code = *pCode & 0xf;
			HighBits = 1;
		}
		PCMBuffer[i] = ADPCM_Decode(Code);
	}	
}
