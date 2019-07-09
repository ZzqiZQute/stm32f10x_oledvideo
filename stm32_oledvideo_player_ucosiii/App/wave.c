#include "wave.h"
#include "usart.h"
#include "string.h"
#include "delay.h"
#include "stm32f10x_dma.h"
#include "malloc.h"
#include "timer.h"
#include "flash2.h"
#include "wm8978.h"
#include "adpcm.h"
#include "read.h"
#include "mp3dec.h"
uint32_t buf_addr1,buf_addr2,buf_size;
uint8_t buf_n,buf_fin;
#define OPEN_MCK
uint8_t play_state=0;
#define play 0
#define stop 1
uint8_t stop_buf=0;
#define I2S2_DMA 1
#define I2S_AUDIOFREQ_192K               ((uint32_t)192000)
#define I2S_AUDIOFREQ_96K                ((uint32_t)96000)
#define I2S_AUDIOFREQ_48K                ((uint32_t)48000)
#define I2S_AUDIOFREQ_44K                ((uint32_t)44100)
#define I2S_AUDIOFREQ_32K                ((uint32_t)32000)
#define I2S_AUDIOFREQ_22K                ((uint32_t)22050)
#define I2S_AUDIOFREQ_16K                ((uint32_t)16000)
#define I2S_AUDIOFREQ_11K                ((uint32_t)11025)
#define I2S_AUDIOFREQ_8K                 ((uint32_t)8000)
#define I2S_AUDIOFREQ_DEFAULT            ((uint32_t)2)
#define I2S_DATAFORMAT_16B               ((uint32_t) 0x00000000)
#define I2S_DATAFORMAT_16B_EXTENDED      ((uint32_t) SPI_I2SCFGR_CHLEN)
#define I2S_DATAFORMAT_24B               ((uint32_t)(SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN_0))
#define I2S_DATAFORMAT_32B               ((uint32_t)(SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN_1))
//__align(8) uint8_t WaveFileBuf[WAVEFILEBUFSIZE];
//__align(8) uint8_t TempBuf[WAVEFILEBUFSIZE/2];
__WaveHeader WaveHead;
uint32_t WriteSize;
char RecFileName[30];
uint32_t count1=0;
uint32_t count2=0;
uint32_t count3=0;
uint32_t count4=0;
uint32_t count5=0;
uint32_t count6=0;
uint32_t count7=0;
__IO uint32_t RecDataSize;
__IO uint8_t RecEnd;
uint8_t *buf;
wavctrl WaveCtrlData;
FIL WavFile;
uint8_t CloseFileFlag;									//1:already open file have to close it
uint8_t EndFileFlag;										//1:reach the wave file end;2:wait for last transfer;3:finish transfer stop dma
__IO uint8_t FillBufFlag;								//0:fill first half buf;1:fill second half buf;0xff do nothing
//static uint8_t I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, uint32_t Flag, uint32_t Status, uint32_t Timeout);
u8 timerStart = 0;

volatile u8 canPlayFlag = 0;
volatile u8 recordInterrupted = 0;
volatile u8 playInterrupted = 0;
volatile u8 recordFlag = 0;
volatile u8 playFlag = 0;
void I2S_Configuration(void)
{
 I2S_InitTypeDef I2S_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
  /* Configure SPI2 pins: CK, WS and SD ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure SPI2 pins: MCK ---------------------------------*/
#ifdef 	OPEN_MCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
   /*打开I2S时钟*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_I2S_DeInit(SPI2);
  /* I2S peripheral configuration */
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
#ifdef 	OPEN_MCK	
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#else
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#endif	
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_22k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;//Rx 是在录音模式下使用，Tx 是在播放模式下使用
  I2S_Init(SPI2, &I2S_InitStructure);
  I2S_Cmd(SPI2, ENABLE);
//	SPI_I2S_SendData(SPI2,0xffff);
//	SPI_I2S_SendData(SPI2,0x0000);
//	SPI_I2S_SendData(SPI2,0xffff);
}   
void I2S_Configuration_Record(void)
{
  I2S_InitTypeDef I2S_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
  /* Configure SPI2 pins: CK, WS and SD ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure SPI2 pins: MCK ---------------------------------*/
#ifdef 	OPEN_MCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
   /*打开I2S时钟*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_I2S_DeInit(SPI2);
  /* I2S peripheral configuration */
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
#ifdef 	OPEN_MCK	
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#else
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#endif	
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_22k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;//Rx 是在录音模式下使用，Tx 是在播放模式下使用
  I2S_Init(SPI2, &I2S_InitStructure);
  I2S_Cmd(SPI2, ENABLE);

}   

DMA_InitTypeDef DMA_InitStructure;

void DMA_Configuration(u32 addr, u32 size)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  /*open DMA clock*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	

  DMA_Cmd(DMA1_Channel5, DISABLE);
  I2S_Cmd(SPI2, DISABLE);
  DMA_DeInit(DMA1_Channel5);
  /* DMA Channel configuration ----------------------------------------------*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(0x4000380c);
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32) addr;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = size;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
  /* Enable SPI DMA Tx request */
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
  I2S_Cmd(SPI2, ENABLE);
  DMA_Cmd(DMA1_Channel5, ENABLE);
}
void DMA_Configuration_Record(u32 addr, u32 size)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  /*open DMA clock*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	

  DMA_Cmd(DMA1_Channel4, DISABLE);
  I2S_Cmd(SPI2, DISABLE);
  DMA_DeInit(DMA1_Channel4);
  /* DMA Channel configuration ----------------------------------------------*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(0x4000380c);
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32) addr;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = size;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
  /* Enable SPI DMA Tx request */
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
  I2S_Cmd(SPI2, ENABLE);
  DMA_Cmd(DMA1_Channel4, ENABLE);
}
void DMA_Double_Buf_Init(u32 addr1,u32 addr2, u32 size)
{
	buf_addr1=addr1;
	buf_addr2=addr2;
	buf_size=size;
	buf_n=1;
  //WM8978_Init();
	I2S_Configuration();
	DMA_Configuration(addr1,size);
}	
void DMA_Double_Buf_Init2(u32 addr1,u32 addr2, u32 size,u32 freq)
{
	I2S_InitTypeDef I2S_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	buf_addr1=addr1;
	buf_addr2=addr2;
	buf_size=size;
	buf_n=1;
  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
  /* Configure SPI2 pins: CK, WS and SD ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure SPI2 pins: MCK ---------------------------------*/
#ifdef 	OPEN_MCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
   /*打开I2S时钟*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_I2S_DeInit(SPI2);
  /* I2S peripheral configuration */
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
#ifdef 	OPEN_MCK	
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#else
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#endif	
  I2S_InitStructure.I2S_AudioFreq = freq;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;//Rx 是在录音模式下使用，Tx 是在播放模式下使用
  I2S_Init(SPI2, &I2S_InitStructure);
  I2S_Cmd(SPI2, ENABLE);
	
	DMA_Configuration(addr1,size);
}	
void DMA_Double_Buf_Init_Record(u32 addr1,u32 addr2, u32 size)
{
	buf_addr1=addr1;
	buf_addr2=addr2;
	buf_size=size;
	buf_n=1;
	I2S_Configuration_Record();
	DMA_Configuration_Record(addr1,size);
}	
void DMA_Double_Buf_Change(void)
{
	DMA1_Channel5->CCR &= (uint16_t)(~DMA_CCR1_EN);	
	DMA1_Channel5->CNDTR = buf_size;
	if(buf_n==1)
	{
		DMA1_Channel5->CMAR  = buf_addr2;
		buf_n=2;
	}
	else
	{
		DMA1_Channel5->CMAR  = buf_addr1;
		buf_n=1;
	}	
	DMA1_Channel5->CCR |= DMA_CCR1_EN;
	buf_fin=1;
}
void DMA_Double_Buf_Change_Record(void)
{
	DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);	
	DMA1_Channel4->CNDTR = buf_size;
	if(buf_n==1)
	{
		DMA1_Channel4->CMAR  = buf_addr2;
		buf_n=2;
	}
	else
	{
		DMA1_Channel4->CMAR  = buf_addr1;
		buf_n=1;
	}	
	DMA1_Channel4->CCR |= DMA_CCR1_EN;
	buf_fin=1;
}

uint8_t Disable_DMA(void)
{
	DMA_Cmd(DMA1_Channel5, DISABLE); 
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, DISABLE);
	return buf_fin;
}	
uint8_t Disable_DMA_Record(void)
{
	DMA_Cmd(DMA1_Channel4, DISABLE); 
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, DISABLE);
	return buf_fin;
}	
void DMA_set_size(u32 size)
{
	buf_size=size;
}

void DMA1_Channel5_IRQHandler(void) 
{
	
	//static u8 xx=0;
	if(DMA_GetITStatus(DMA1_IT_TC5) != RESET)
	{
		
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		
		DMA_Double_Buf_Change();
	
	}
	
}
void DMA1_Channel4_IRQHandler(void) 
{
	
	//static u8 xx=0;
	if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)
	{
		
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		
		DMA_Double_Buf_Change_Record();
	
	}
	
}

//FRESULT ScanWavefiles (char* path)
//{
//    FRESULT res;
//    FILINFO fno;
//    DIR dir;
//		uint8_t i;
//		char PathBuf[50];
//		while(1)
//	{    res = f_opendir(&dir, path);                       							/* Open the directory */
//    if (res == FR_OK) 
//		{
//        for (;;) 
//				{
//            res = f_readdir(&dir, &fno);                   					/* Read a directory item */
//            if (res != FR_OK || fno.fname[0] == 0)  								/* Break on error or end of dir */
//							break;
//						//printf("%s\r\n", fno.fname);
//            if ((fno.fname[0] == '.')||((fno.fattrib & AM_DIR)!=0))   
//							continue;																							/* Ignore dot entry and directory*/ 
//						for(i=0;i<13;i++)
//						{
//							if(fno.fname[i]=='.')
//								break;
//						}
//						if(i>8)
//							continue;
//            if (((fno.fname[i+1] == 'w')||(fno.fname[i+1] == 'W'))
//							&&((fno.fname[i+2] == 'a')||(fno.fname[i+2] == 'A'))
//						&&((fno.fname[i+3] == 'v')||(fno.fname[i+3] == 'V'))) 
//						{
//                strcpy(PathBuf,path);
//								strcat(PathBuf,"/");
//								strcat(PathBuf,fno.fname);
//								printf("%s\r\n", PathBuf);
//								wav_play_song(PathBuf);		
//								delay_ms(1000);
//						}
//            
//        }
//        f_closedir(&dir);
//    }
//	}
// 
//}

uint8_t wave_decode_init(char* fname,wavctrl* wavx)
{
	uint32_t br=0;
	uint8_t res=0;
	ChunkRIFF *riff;
	ChunkFMT *fmt;
	ChunkFACT *fact;
	ChunkDATA *data;
  u8* TempBuf = (u8*)mymalloc(1024);
	res=my_f_open(&WavFile,(TCHAR*)fname,FA_READ);												//打开文件
	if(res==FR_OK)
	{
		CloseFileFlag=1;
		my_f_read(&WavFile,TempBuf,WAVEFILEBUFSIZE/2,&br);									//读取WAVEFILEBUFSIZE/2字节数据
		riff=(ChunkRIFF *)TempBuf;																			//获取RIFF块
		if(riff->Format==0X45564157)																		//是WAV文件
		{
			fmt=(ChunkFMT *)(TempBuf+12);																	//获取FMT块
				if(fmt->AudioFormat==1||fmt->AudioFormat==3)								//线性PCM或32位WAVE=3
				{
					fact=(ChunkFACT *)(TempBuf+12+8+fmt->ChunkSize);					//读取FACT块
					if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)
						wavx->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;	//具有fact/LIST块的时候(未测试)
					else 
						wavx->datastart=12+8+fmt->ChunkSize;  
					data=(ChunkDATA *)(TempBuf+wavx->datastart);					
					if(data->ChunkID==0X61746164)															//读取DATA块成功
					{
						wavx->audioformat=fmt->AudioFormat;											//音频格式
						wavx->nchannels=fmt->NumOfChannels;											//通道数
						wavx->samplerate=fmt->SampleRate;												//采样率
						wavx->bitrate=fmt->ByteRate*8;													//位速率=通道数×每秒数据位数×每样本的数据位数
						wavx->blockalign=fmt->BlockAlign;												//块对齐=通道数×每样本的数据位值／8
						wavx->bps=fmt->BitsPerSample;														//位数,8/16/24/32位
						wavx->datasize=data->ChunkSize;													//音频数据块大小
						wavx->datastart=wavx->datastart+8;											//数据流开始的地方. 
						printf("wavx->audioformat:%d\r\n",wavx->audioformat);
						printf("wavx->nchannels:%d\r\n",wavx->nchannels);
						printf("wavx->samplerate:%d\r\n",wavx->samplerate);
						printf("wavx->bitrate:%d\r\n",wavx->bitrate);
						printf("wavx->blockalign:%d\r\n",wavx->blockalign);
						printf("wavx->bps:%d\r\n",wavx->bps);
						printf("wavx->datasize:%d\r\n",wavx->datasize);
						printf("wavx->datastart:%d\r\n",wavx->datastart);  
					}
					else 
						res=4;																									//data区域未找到.
				}
				else
					res=3;																										//非线性PCM，不支持
		}
		else 
			res=2;																												//非wav文件	
	}
	else 
		res=1;																													//打开文件错误
	myfree(TempBuf);
	return res;
}


u8 wav_play_song(char *path)
{	 
 	FIL* fwav;
  u16 br;
	u8 res,rval;	  
	u8 *databuf;	
	u8 *databuf2;	 
	u32 frame=0;
			   
	rval=0;	   
	WM8978_Init();
	WM8978_ADDA_Cfg(1,0);										//开启DAC
	WM8978_Input_Cfg(0,0,0);								//关闭输入通道
	WM8978_Output_Cfg(1,0);									//开启DAC输出  
	WM8978_SPKvol_Set(60);	
	WM8978_HPvol_Set(35,35);
	if(wave_decode_init(path,&WaveCtrlData)==0)//得到文件的信息,且为wav
	{
		if((WaveCtrlData.bps==16)&&(WaveCtrlData.samplerate>2)&&(WaveCtrlData.samplerate<48100))
		{
			
		}
		else
		{
			
			printf("wav_file_err 1\r\n");	
			return 1;			
		}
	}
	else
	{
		
		printf("not wav\r\n");
		return 1;
	}		
	fwav=(FIL*)mymalloc(sizeof(FIL));//申请内存
	databuf=(u8*)mymalloc(10240);		//开辟4096字节的内存区域
	databuf2=(u8*)mymalloc(10240);		//开辟4096字节的内存区域
	if(databuf==NULL||databuf2==NULL||fwav==NULL)rval=0XFF ;//内存申请失败.
	printf("memory_used_wav_play:%d%%\r\n",mem_perused());
	if(rval==0)
	{	    	 						  				 		   		 						  
		res=my_f_open(fwav,(const TCHAR*)path,FA_READ);//打开文件	 
 		if(res==0)//打开成功.
		{ 
	//		f_read(fwav,databuf,2048,(UINT*)&br);//读出4096个字节
			buf_fin=0;			
			DMA_Double_Buf_Init((uint32_t)databuf,(uint32_t)databuf2,5120);
			while(rval==0)
			{
				res=my_f_read(fwav,databuf2,10240,(UINT*)&br);//读出4096个字节 			
				if(br!=10240||res!=0)
				{
					rval=1;
					break;//读完了.
					
				}
				frame++;
		//		printf("read frame cnt = %d\n",frame);
		//		printf("data = %s\n",databuf2);
				while(buf_fin==0||play_state==stop)
				{
					
				}
				if(timerStart ==0){
					timerStart =1;
				}					
				buf_fin=0;
				res=my_f_read(fwav,databuf,10240,(UINT*)&br);//读出4096个字节 
				if(br!=10240||res!=0)
				{
					rval=1;
					break;//读完了.		  
				}
				frame++;
		//   	printf("read frame cnt = %d\n",frame);	
		//		printf("data = %s\n",databuf);				
				while(buf_fin==0||play_state==stop)
				{
					
				}				
				buf_fin=0;			
				
				//mp3_msg_show(fmp3->fsize);//显示信息	    						 
			}
			my_f_close(fwav);
			Disable_DMA(); 
		}else 
		{
			rval=0XFF;//出现错误	
			printf("open file err");
		}   	  
	}					
	myfree(databuf);	
	myfree(databuf2);	
	myfree(fwav);
	WM8978_DeInit();
	return rval;	  	 		  	    
}

//FIL* RecordFileinit(char* path,u8* status) //初始化WAV头			   
//{
//	FIL* fwav=(FIL*)mymalloc(sizeof(FIL));//申请内存
//	FRESULT Res;
//	WaveHead.riff.ChunkID=0X46464952;				//"RIFF"
//	WaveHead.riff.ChunkSize=0;							//还未确定,最后需要计算
//	WaveHead.riff.Format=0X45564157; 				//"WAVE"
//	WaveHead.fmt.ChunkID=0X20746D66; 				//"fmt "
//	WaveHead.fmt.ChunkSize=16; 							//大小为16个字节
//	WaveHead.fmt.AudioFormat=0X01; 					//0X01,表示PCM;0X01,表示IMA ADPCM
// 	WaveHead.fmt.NumOfChannels=2;						//双声道
// 	WaveHead.fmt.SampleRate=22050;					//16Khz采样率 采样速率
// 	WaveHead.fmt.ByteRate=WaveHead.fmt.SampleRate*4;//字节速率=采样率*通道数*(ADC位数/8)
// 	WaveHead.fmt.BlockAlign=4;							//块大小=通道数*(ADC位数/8)
// 	WaveHead.fmt.BitsPerSample=16;					//16位PCM
//  WaveHead.data.ChunkID=0X61746164;				//"data"
// 	WaveHead.data.ChunkSize=0;							//数据大小,还需要计算 

//		sprintf(RecFileName,path);
//		Res=f_open(fwav,RecFileName,FA_CREATE_ALWAYS|FA_WRITE);
//		if(Res==FR_OK)
//		{
//			CloseFileFlag=1;
//			Res=f_write(fwav,(const void*)(&WaveHead),sizeof(__WaveHeader),&WriteSize);//写入头数据
//			
//		}
//		else if(status!=0){
//				*status=1;
//				myfree(fwav);
//				return 0;
//			}				//新建文件出错
//	
//	return fwav;																//文件初始化成功
//} 
//u8 wav_record_song(char *path,u32 ms)
//{	 
// 	FIL* fwav;
//  u16 br;
//	u8 res,rval;	  
//	u8 *databuf;	
//	u8 *databuf2;	 
//	u32 frame=0;
//	u32 tick=0;
//	u32 currtick=0;
//	u32 size = 0;
//	WM8978_Init();
//	WM8978_ADDA_Cfg(0,1);										//开启ADC
//	WM8978_Input_Cfg(1,1,0);								//开启输入通道(MIC&LINE IN)
//	WM8978_Output_Cfg(0,1);									//开启BYPASS输出 
//	WM8978_MIC_Gain(46);										//MIC增益设置 
//	WM8978_SPKvol_Set(0);										//喇叭音量设置为0，避免啸叫		   
//	rval=0;	   
//	fwav=RecordFileinit(path,0);
//	databuf=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	databuf2=(u8*)mymalloc(2048);		//开辟4096字节的内存区域

//	if(databuf==NULL||databuf2==NULL||fwav==NULL)rval=0XFF ;//内存申请失败.
//	printf("memory_used_wav_play:%d%%\r\n",mem_perused());
//	if(rval==0)
//	{	    	 						  				 		   		 						  
//	//		f_read(fwav,databuf,2048,(UINT*)&br);//读出4096个字节
//			buf_fin=0;			
//			tick=Get_Tick();
//			DMA_Double_Buf_Init_Record((uint32_t)databuf,(uint32_t)databuf2,1024);
//			while(rval==0)
//			{
//				currtick=Get_Tick();
//				printf("current time = %d ; total_time = %d\n",currtick-tick,ms);
//				if(currtick - tick > ms)
//				{
//					rval=1;
//					break;
//				}
////				res=f_read(fwav,databuf2,2048,(UINT*)&br);//读出4096个字节 			
////				if(br!=2048||res!=0)
////				{
////					rval=1;
////					break;//读完了.
////					
////				}
//				
//		//		printf("data = %s\n",databuf2);
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}		
//				res = f_write(fwav,databuf2,2048,(UINT*)&br);
//				frame++;
//				printf("write frame cnt = %d\n",frame);
//				//printf("%s\n",databuf2);
//				size+=br;
//				if(br!=2048||res!=0)
//				{
//					rval=1;
//					break;//读完了.
//					
//				}
//				buf_fin=0;
//			
//		//		printf("data = %s\n",databuf2);
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}		
//				frame++;
//				printf("write frame cnt = %d\n",frame);
//				res = f_write(fwav,databuf,2048,(UINT*)&br);
//				//printf("%s\n",databuf);
//				size+=br;
//				if(br!=2048||res!=0)
//				{
//					rval=1;
//					break;//读完了.
//					
//				}
//				buf_fin=0;			
//				
//				//mp3_msg_show(fmp3->fsize);//显示信息	    						 
//			}
//			WaveHead.riff.ChunkSize=size+36;			//整个文件的大小-8;
//			WaveHead.data.ChunkSize=size;				//数据大小
//			printf("RecDataSize=%d\n",size);
//			f_lseek(fwav,0);	
//			printf("sizeof(__WavHeader) = %d",sizeof(__WaveHeader));
//			f_write(fwav,(const void*)(&WaveHead),sizeof(__WaveHeader),&WriteSize);
//			f_close(fwav);
//			Disable_DMA_Record();  	  
//	}					
//	myfree(databuf);	
//	myfree(databuf2);	
//	myfree(fwav);
//	WM8978_DeInit();
//	return rval;	  	 		  	    
//}
//u8 wav_play_song_from_flash()
//{	 
// 
//	u8 rval;	  
//	u8 *databuf;	
//	u8 *databuf2;
//  u8 buf[1024];	
//	u32 frame=0;
//  u32 size=0;
//	u32 readsize=0;
//	u32 sizeAddr = SoundSizeAddress;
//  u32 addr = WriteFlashAddress;	
//	rval=0;
//	WM8978_Init();	
//	WM8978_ADDA_Cfg(1,0);										//开启DAC
//	WM8978_Input_Cfg(0,0,0);								//关闭输入通道
//	WM8978_Output_Cfg(1,0);									//开启DAC输出  
//	WM8978_SPKvol_Set(63);										//喇叭音量设置为0，避免啸叫		   
//	Read_Flash(sizeAddr,&size,1);
//	databuf=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	databuf2=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	if(databuf==NULL||databuf2==NULL)rval=0XFF ;//内存申请失败.
//	printf("memory_used_wav_play:%d%%\r\n",mem_perused());
//	if(rval==0)
//	{	    	 						  				 		   		 						  
//	//		f_read(fwav,databuf,2048,(UINT*)&br);//读出4096个字节
//			buf_fin=0;			
//			DMA_Double_Buf_Init((uint32_t)databuf,(uint32_t)databuf2,1024);
//			while(rval==0)
//			{
//			  Read_Flash_At(addr+readsize,buf,512);//读出4096个字节 
//				Adpcm_FrameDecode((s16*)databuf2,buf,1024);
//				readsize+=512;
//				if(readsize>=size)
//				{
//					rval=1;
//					break;
//					
//				}
//			
//			//	for(j=0;j<2048;j++)
//			//		printf("%d ",databuf[j]);
//			//	printf("\n");
//			
//				frame++;
//				printf("read frame cnt = %d\n",frame);
//		//		printf("data = %s\n",databuf2);
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}					
//				buf_fin=0;
//				Read_Flash_At(addr+readsize,buf,512);//读出4096个字节 
//				Adpcm_FrameDecode((s16*)databuf,buf,1024);
//				readsize+=512;
//				if(readsize>=size)
//				{
//					rval=1;
//					break;
//					
//				}
//				frame++;
//		   	printf("read frame cnt = %d\n",frame);	
//		//		printf("data = %s\n",databuf);				
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}				
//				buf_fin=0;			
//				
//				//mp3_msg_show(fmp3->fsize);//显示信息	    						 
//			}
//		
//			Disable_DMA(); 
//		}else 
//		{
//			rval=0XFF;//出现错误	
//			printf("open file err");
//		}   	  
//			
//	myfree(databuf);	
//	myfree(databuf2);	
//	WM8978_DeInit();
//	return rval;	  	 		  	    
//}
//u8 wav_record_song_to_flash(u32 ms,u32 *actualTime)
//{	 
// 
//	u8 rval;	
//  u8 buf[1024];  
//	u8 *databuf;	
//	u8 *databuf2;	 
//	u32 frame=0;
//	u32 tick=0;
//	u32 currtick=0;
//	u32 size = 0;
//	u32 sizeAddr = SoundSizeAddress;
//  u32 addr = WriteFlashAddress;	
//	rval=0;	  
//	Reset_Flash();
//	WM8978_Init();	
//	WM8978_ADDA_Cfg(0,1);										//开启ADC
//	WM8978_Input_Cfg(1,1,0);								//开启输入通道(MIC&LINE IN)
//	WM8978_Output_Cfg(0,1);									//开启BYPASS输出 
//	WM8978_MIC_Gain(46);										//MIC增益设置 
//	WM8978_SPKvol_Set(0);										//喇叭音量设置为0，避免啸叫		
//	databuf=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	databuf2=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	if(databuf==NULL||databuf2==NULL)rval=0XFF ;//内存申请失败.
//	printf("memory_used_wav_play:%d%%\r\n",mem_perused());
//	if(rval==0)
//	{	    	 					
//	//		f_read(fwav,databuf,2048,(UINT*)&br);//读出4096个字节
//			buf_fin=0;		
//			tick=Get_Tick();		
//			DMA_Double_Buf_Init_Record((uint32_t)databuf,(uint32_t)databuf2,1024);
//			while(rval==0)
//			{
//			  currtick=Get_Tick();
//				if(currtick - tick > ms)
//				{
//					rval=1;
//					break;
//				}
//			  while(buf_fin==0||play_state==stop)
//				{
//					
//				}	
//				Adpcm_FrameEncode((s16*)databuf2,buf,1024);
//				Write_Flash_At(addr+size,buf,512);
//				//res = f_write(fwav,databuf2,2048,(UINT*)&br);
//				frame++;
//				printf("write frame cnt = %d    current time = %d total_time = %d\n",frame,currtick-tick,ms);
//				//printf("%s\n",databuf2);
//				
//				size+=512;
//				if(size>= MAX_FLASH_SIZE)
//				{
//					rval=1;
//					break;
//					
//				}
//				buf_fin=0;
//			
//		//		printf("data = %s\n",databuf2);
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}		
//				Adpcm_FrameEncode((s16*)databuf,buf,1024);
//				Write_Flash_At(addr+size,buf,512);
//				frame++;
//				printf("write frame cnt = %d    current time = %d total_time = %d\n",frame,currtick-tick,ms);
//				//res = f_write(fwav,databuf,2048,(UINT*)&br);
//				//printf("%s\n",databuf);
//				size+=512;
//				if(size>= MAX_FLASH_SIZE)
//				{
//					rval=1;
//					break;
//					
//				}
//				buf_fin=0;			
//				
//				
//				//mp3_msg_show(fmp3->fsize);//显示信息	    						 
//			}
//	
//			Disable_DMA_Record(); 
//			Write_Flash(sizeAddr,&size,1);
//		
//		
//		}else 
//		{
//			rval=0XFF;//出现错误	
//			printf("open file err");
//		}   	  
//			
//	myfree(databuf);	
//	myfree(databuf2);	
//	if(actualTime!=0)
//		*actualTime = currtick-tick;
//	WM8978_DeInit();
//	return rval;	  	 		  	    
//}


//u8 wav_play_song_from_flash_without_compress()
//{	 
// 
//	u8 rval;	  
//	u8 *databuf;	
//	u8 *databuf2;
//  u8 buf[2048];	
//	u32 frame=0;
//  u32 size=0;
//	u32 readsize=0;
//	u32 sizeAddr = SoundSizeAddress;
//  u32 addr = WriteFlashAddress;	
//	rval=0;	   
//	WM8978_Init();
//	WM8978_ADDA_Cfg(1,0);										//开启DAC
//	WM8978_Input_Cfg(0,0,0);								//关闭输入通道
//	WM8978_Output_Cfg(1,0);									//开启DAC输出  
//	Read_Flash(sizeAddr,&size,1);
//	databuf=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	databuf2=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	if(databuf==NULL||databuf2==NULL)rval=0XFF ;//内存申请失败.
//	printf("memory_used_wav_play:%d%%\r\n",mem_perused());
//	if(rval==0)
//	{	    	 						  				 		   		 						  
//	//		f_read(fwav,databuf,2048,(UINT*)&br);//读出4096个字节
//			buf_fin=0;			
//			DMA_Double_Buf_Init((uint32_t)databuf,(uint32_t)databuf2,1024);
//			while(rval==0)
//			{
//			  Read_Flash_At(addr+readsize,buf,2048);//读出4096个字节 
//				memcpy(databuf2,buf,2048);
//				readsize+=2048;
//				if(readsize>=size)
//				{
//					rval=1;
//					break;
//					
//				}
//			
//			//	for(j=0;j<2048;j++)
//			//		printf("%d ",databuf[j]);
//			//	printf("\n");
//			
//				frame++;
//				printf("read frame cnt = %d\n",frame);
//		//		printf("data = %s\n",databuf2);
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}					
//				buf_fin=0;
//				Read_Flash_At(addr+readsize,buf,2048);//读出4096个字节 
//				memcpy(databuf,buf,2048);
//				readsize+=2048;
//				if(readsize>=size)
//				{
//					rval=1;
//					break;
//					
//				}
//				frame++;
//		   	printf("read frame cnt = %d\n",frame);	
//		//		printf("data = %s\n",databuf);				
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}				
//				buf_fin=0;			
//				
//				//mp3_msg_show(fmp3->fsize);//显示信息	    						 
//			}
//		
//			Disable_DMA(); 
//		}else 
//		{
//			rval=0XFF;//出现错误	
//			printf("open file err");
//		}   	  
//			
//	myfree(databuf);	
//	myfree(databuf2);
//	WM8978_DeInit();		
//	return rval;	  	 		  	    
//}
//u8 wav_record_song_to_flash_without_compress(u32 ms)
//{	 
// 
//	u8 rval;	
//	u8 *databuf;	
//	u8 *databuf2;	 
//	u32 frame=0;
//	u32 tick=0;
//	u32 currtick=0;
//	u32 size = 0;
//	u32 sizeAddr = SoundSizeAddress;
//  u32 addr = WriteFlashAddress;	
//	rval=0;	   
//	Reset_Flash();
//	WM8978_Init();
//	WM8978_ADDA_Cfg(0,1);										//开启ADC
//	WM8978_Input_Cfg(1,1,0);								//开启输入通道(MIC&LINE IN)
//	WM8978_Output_Cfg(0,1);									//开启BYPASS输出 
//	WM8978_MIC_Gain(46);										//MIC增益设置 
//	WM8978_SPKvol_Set(0);										//喇叭音量设置为0，避免啸叫		
//	databuf=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	databuf2=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	if(databuf==NULL||databuf2==NULL)rval=0XFF ;//内存申请失败.
//	printf("memory_used_wav_play:%d%%\r\n",mem_perused());
//	if(rval==0)
//	{	    	 					
//	//		f_read(fwav,databuf,2048,(UINT*)&br);//读出4096个字节
//			buf_fin=0;		
//			tick=Get_Tick();		
//			DMA_Double_Buf_Init_Record((uint32_t)databuf,(uint32_t)databuf2,1024);
//			while(rval==0)
//			{
//			  currtick=Get_Tick();
//				if(currtick - tick > ms)
//				{
//					rval=1;
//					break;
//				}
//			  while(buf_fin==0||play_state==stop)
//				{
//					
//				}	
//				Write_Flash_At(addr+size,databuf2,2048);
//				//res = f_write(fwav,databuf2,2048,(UINT*)&br);
//				frame++;
//				printf("write frame cnt = %d    current time = %d total_time = %d\n",frame,currtick-tick,ms);
//				//printf("%s\n",databuf2);
//				
//				size+=2048;
//				if(size>= MAX_FLASH_SIZE)
//				{
//					rval=1;
//					break;
//					
//				}
//				buf_fin=0;
//			
//		//		printf("data = %s\n",databuf2);
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}		
//				Write_Flash_At(addr+size,databuf,2048);
//				frame++;
//				printf("write frame cnt = %d    current time = %d total_time = %d\n",frame,currtick-tick,ms);
//				//res = f_write(fwav,databuf,2048,(UINT*)&br);
//				//printf("%s\n",databuf);
//				size+=2048;
//				if(size>= MAX_FLASH_SIZE)
//				{
//					rval=1;
//					break;
//					
//				}
//				buf_fin=0;			
//				
//				
//				//mp3_msg_show(fmp3->fsize);//显示信息	    						 
//			}
//	
//			Disable_DMA_Record(); 
//			Write_Flash(sizeAddr,&size,1);
//		
//		
//		}else 
//		{
//			rval=0XFF;//出现错误	
//			printf("open file err");
//		}   	  
//			
//	myfree(databuf);	
//	myfree(databuf2);	
//	WM8978_DeInit();
//	return rval;	  	 		  	    
//}

//u8 wav_record_song_to_flash_with_interrupt(u32 ms,u32 *actualTime)
//{	 
//  
//	u8 rval;	
//  u8 buf[1024];  
//	u8 *databuf;	
//	u8 *databuf2;	 
//	u32 frame=0;
//	u32 tick=0;
//	u32 currtick=0;
//	u32 size = 0;
//	u32 sizeAddr = SoundSizeAddress;
//  u32 addr = WriteFlashAddress;	
//	recordInterrupted = 0;
//	rval=0;	  
//	Reset_Flash();
//	WM8978_Init();	
//	WM8978_ADDA_Cfg(0,1);										//开启ADC
//	WM8978_Input_Cfg(1,1,0);								//开启输入通道(MIC&LINE IN)
//	WM8978_Output_Cfg(0,1);									//开启BYPASS输出 
//	WM8978_MIC_Gain(46);										//MIC增益设置 
//	WM8978_SPKvol_Set(0);										//喇叭音量设置为0，避免啸叫		
//	databuf=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	databuf2=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	if(databuf==NULL||databuf2==NULL)rval=0XFF ;//内存申请失败.
//	printf("memory_used_wav_play:%d%%\r\n",mem_perused());
//	if(rval==0)
//	{	    	 					
//	//		f_read(fwav,databuf,2048,(UINT*)&br);//读出4096个字节
//			buf_fin=0;		
//			tick=Get_Tick();		
//			DMA_Double_Buf_Init_Record((uint32_t)databuf,(uint32_t)databuf2,1024);
//		//	RecordOnHint();
//			while(rval==0)
//			{
//			  currtick=Get_Tick();
//				if(currtick - tick > ms)
//				{
//					rval=1;
//					break;
//				}
//			  while(buf_fin==0||play_state==stop)
//				{
//					
//				}	
//				Adpcm_FrameEncode((s16*)databuf2,buf,1024);
//				Write_Flash_At(addr+size,buf,512);
//				//res = f_write(fwav,databuf2,2048,(UINT*)&br);
//				frame++;
//				printf("write frame cnt = %d    current time = %d total_time = %d\n",frame,currtick-tick,ms);
//				//printf("%s\n",databuf2);
//				
//				size+=512;
//				if(size>= MAX_FLASH_SIZE || recordInterrupted)
//				{
//					rval=1;
//					break;
//					
//				}
//				buf_fin=0;
//			
//		//		printf("data = %s\n",databuf2);
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}		
//				Adpcm_FrameEncode((s16*)databuf,buf,1024);
//				Write_Flash_At(addr+size,buf,512);
//				frame++;
//				printf("write frame cnt = %d    current time = %d total_time = %d\n",frame,currtick-tick,ms);
//				//res = f_write(fwav,databuf,2048,(UINT*)&br);
//				//printf("%s\n",databuf);
//				size+=512;
//				if(size>= MAX_FLASH_SIZE || recordInterrupted)
//				{
//					rval=1;
//					break;
//					
//				}
//				buf_fin=0;			
//				
//				
//				//mp3_msg_show(fmp3->fsize);//显示信息	    						 
//			}
//	
//			Disable_DMA_Record(); 
//			Write_Flash(sizeAddr,&size,1);
//		
//		
//		}else 
//		{
//			rval=0XFF;//出现错误	
//			printf("open file err");
//		}   	  
//			
//	myfree(databuf);	
//	myfree(databuf2);	
//	if(actualTime!=0)
//		*actualTime = currtick-tick;
//	recordInterrupted=0;
//	//RecordOffHint();
//	WM8978_DeInit();
//	return rval;	  	 		  	    
//}
//u8 wav_play_song_from_flash_with_interrupt()
//{	 
// 
//	u8 rval;	  
//	u8 *databuf;	
//	u8 *databuf2;
//  u8 buf[1024];	
//	u32 frame=0;
//  u32 size=0;
//	u32 readsize=0;
//	u32 sizeAddr = SoundSizeAddress;
//  u32 addr = WriteFlashAddress;	
//	rval=0;	   
//	playInterrupted = 0;
//	WM8978_Init();
//	WM8978_ADDA_Cfg(1,0);										//开启DAC
//	WM8978_Input_Cfg(0,0,0);								//关闭输入通道
//	WM8978_Output_Cfg(1,0);									//开启DAC输出  
//	WM8978_SPKvol_Set(63);										//喇叭音量设置为0，避免啸叫		   
//	Read_Flash(sizeAddr,&size,1);
//	databuf=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	databuf2=(u8*)mymalloc(2048);		//开辟4096字节的内存区域
//	if(databuf==NULL||databuf2==NULL)rval=0XFF ;//内存申请失败.
//	printf("memory_used_wav_play:%d%%\r\n",mem_perused());
//	if(rval==0)
//	{	    	 						  				 		   		 						  
//	//		f_read(fwav,databuf,2048,(UINT*)&br);//读出4096个字节
//			buf_fin=0;			
//			DMA_Double_Buf_Init((uint32_t)databuf,(uint32_t)databuf2,1024);
//			while(rval==0)
//			{
//			  Read_Flash_At(addr+readsize,buf,512);//读出4096个字节 
//				Adpcm_FrameDecode((s16*)databuf2,buf,1024);
//				readsize+=512;
//				if(readsize>=size)
//				{
//					rval=1;
//					break;
//					
//				}
//			
//			//	for(j=0;j<2048;j++)
//			//		printf("%d ",databuf[j]);
//			//	printf("\n");
//			
//				frame++;
//				printf("read frame cnt = %d\n",frame);
//		//		printf("data = %s\n",databuf2);
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}					
//				buf_fin=0;
//				Read_Flash_At(addr+readsize,buf,512);//读出4096个字节 
//				Adpcm_FrameDecode((s16*)databuf,buf,1024);
//				readsize+=512;
//				if(readsize>=size)
//				{
//					rval=1;
//					break;
//					
//				}
//				frame++;
//		   	printf("read frame cnt = %d\n",frame);	
//		//		printf("data = %s\n",databuf);				
//				while(buf_fin==0||play_state==stop)
//				{
//					
//				}				
//				buf_fin=0;			
//				
//				//mp3_msg_show(fmp3->fsize);//显示信息	    						 
//			}
//		
//			Disable_DMA(); 
//		}else 
//		{
//			rval=0XFF;//出现错误	
//			printf("open file err");
//		}   	  
//			
//	myfree(databuf);	
//	myfree(databuf2);	
//  playInterrupted = 0;
//	WM8978_DeInit();
//	return rval;	  	 		  	    
//}
u8 mp3_play_song(char* path)
{
#define read_buf_size 2048
#define pcm_buf_each_size 2560
	
	/* MP3解码时用到的变量 */	
	int err, i, current_sample_rate = 0;	

	int						read_offset = 0;				/* 读偏移指针				*/
	int						bytes_left = 0;					/* 剩余字节数				*/	
	unsigned long	Frames = 0;							/* mP3帧计数				*/
	unsigned char	*read_ptr;							/* 缓冲区指针				*/
	HMP3Decoder		Mp3Decoder;						  /* mp3解码器指针		*/	

	uint8_t bufflag=0;										//用于切换buffer的标志
	static MP3FrameInfo		Mp3FrameInfo;		//mP3帧信息      

	FIL* fmp3;
	u8 res,rval=0;
	u16 br;
	u8 *data_read_buf;// read文件缓冲区
	short *data_pcm_buf;
	short *data_pcm_buf2;// PCM流缓冲，使用两个缓冲区 
	WM8978_Init();	
	WM8978_ADDA_Cfg(1,0);										//开启DAC
	WM8978_Input_Cfg(0,0,0);								//关闭输入通道
	WM8978_Output_Cfg(1,0);									//开启DAC输出  
	WM8978_SPKvol_Set(63);										//喇叭音量设置为0，避免啸叫		
  WM8978_HPvol_Set(50,50);	
	
	fmp3=(FIL*)mymalloc(sizeof(FIL));//申请内存
	data_read_buf=(u8*)mymalloc(read_buf_size);		//开辟内存区域
	data_pcm_buf=(short*)mymalloc(pcm_buf_each_size);
	data_pcm_buf2=(short*)mymalloc(pcm_buf_each_size);
	
	if(data_read_buf==NULL||data_pcm_buf==NULL||data_pcm_buf2==NULL||fmp3==NULL)return 0XFF;//内存申请失败.
	
	res=my_f_open(fmp3,path,FA_READ);//打开文件	 	
	if (res!=FR_OK)
	{
		printf("open file %s error \r\n",path);	
		myfree(fmp3);			
		myfree(data_read_buf);	
		myfree(data_pcm_buf);	
		myfree(data_pcm_buf2);
		return 1;										//文件无法打开，终止解码。进入下一次循环，读取下一个文件
	}

	res = my_f_read(fmp3,data_read_buf,read_buf_size,(UINT*)&br);//读2048个字节
	if (res!=FR_OK)
	{
		printf("read file %s error \r\n",path);	
		myfree(fmp3);			
		myfree(data_read_buf);	
		myfree(data_pcm_buf);	
		myfree(data_pcm_buf2);
		return 1;										//文件无法打开，终止解码。进入下一次循环，读取下一个文件
	}
	
	//打开成功
	//初始化MP3解码器
	Mp3Decoder = MP3InitDecoder();
	if (Mp3Decoder==0)
	{
		printf("ERR_MP3_Allocate Buffers\r\n");
		myfree(fmp3);			
		myfree(data_read_buf);	
		myfree(data_pcm_buf);	
		myfree(data_pcm_buf2);
		return 1;										
	}	
	printf("memory_used_mp3_play:%d%%\r\n",mem_perused());
	read_ptr = data_read_buf;									//指向mp3输入流
	bytes_left = br;								//实际读到的输入流大小大小
	
	while(rval==0)
	{
		read_offset = MP3FindSyncWord(read_ptr, bytes_left);	//寻找帧同步，返回第一个同步字的位置
		if(read_offset < 0)																		//没有找到同步字
		{
			rval=1;
			break;																							//跳出循环2，回到循环1	
		}
		read_ptr += read_offset;					//偏移至同步字的位置
		bytes_left -= read_offset;				//同步字之后的数据大小	
		if(bytes_left < 512)							//补充数据
		{
			/* 注意这个地方因为采用的是DMA读取，所以一定要4字节对齐  */
			i=(uint32_t)(bytes_left)&3;									//判断多余的字节
			if(i) i=4-i;														//需要补充的字节
			memcpy(data_read_buf+i, read_ptr, bytes_left);	//从对齐位置开始复制
			read_ptr = data_read_buf+i;										//指向数据对齐位置
			res = my_f_read(fmp3, data_read_buf+bytes_left+i, read_buf_size-(bytes_left+i), (UINT*)&br);//补充数据
			bytes_left += br;										//有效数据流大小
		}	
		if(bufflag)
		{
			err = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, data_pcm_buf, 0);					//开始解码 参数：mp3解码结构体、输入流指针、输入流大小、输出流指针、数据格式
		}
		else
		{
			err = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, data_pcm_buf2, 0);		
		}
		if(Frames==0)
		{
			DMA_Double_Buf_Init((uint32_t)data_pcm_buf,(uint32_t)data_pcm_buf2,pcm_buf_each_size/2);//在第一侦位置，初始化DMA
		}
		Frames++;
		if (err != ERR_MP3_NONE)									//错误处理
		{
			switch (err)
			{
				case ERR_MP3_INDATA_UNDERFLOW:
					printf("ERR_MP3_INDATA_UNDERFLOW\r\n");
					read_ptr = data_read_buf;
					res = my_f_read(fmp3, data_read_buf, read_buf_size, (UINT*)&br);
					bytes_left = br;
					break;
		
				case ERR_MP3_MAINDATA_UNDERFLOW:
					rval=1;	
					/* do nothing - next call to decode will provide more mainData */
					printf("ERR_MP3_MAINDATA_UNDERFLOW\r\n");
					break;
		
				default:
					rval=1;	
					printf("UNKNOWN ERROR:%d\r\n", err);
		
					// 跳过此帧
					if (bytes_left > 0)
					{
						bytes_left --;
						read_ptr ++;
					}	
					break;
			}
		}
		else
		{
				MP3GetLastFrameInfo(Mp3Decoder, &Mp3FrameInfo);		//获取解码信息				

		    /* 根据解码信息设置采样率 */
				if (Mp3FrameInfo.samprate != current_sample_rate)	//采样率 
				{
					current_sample_rate = Mp3FrameInfo.samprate;

					printf(" \r\n Bitrate       %dKbps", Mp3FrameInfo.bitrate/1000);
				  printf(" \r\n Samprate      %dHz", current_sample_rate);
					printf(" \r\n BitsPerSample %db", Mp3FrameInfo.bitsPerSample);
					printf(" \r\n nChans        %d", Mp3FrameInfo.nChans);
					//printf(" \r\n Layer         %d", Mp3FrameInfo.layer);
					//printf(" \r\n Version       %d", Mp3FrameInfo.version);
					printf(" \r\n OutputSamps   %d\r\n", Mp3FrameInfo.outputSamps);
					if((Mp3FrameInfo.bitsPerSample==16)&&(Mp3FrameInfo.nChans==2)&&(current_sample_rate>2)&&(current_sample_rate<48100)&&(Mp3FrameInfo.outputSamps==1152)){
					DMA_set_size(Mp3FrameInfo.outputSamps);
					}
					else
					rval=1;
					if(current_sample_rate>=2){
				   DMA_Double_Buf_Init2((uint32_t)data_pcm_buf,(uint32_t)data_pcm_buf2,Mp3FrameInfo.outputSamps,current_sample_rate);
					}
			}		
				
					while(buf_fin==0||play_state==stop)
				{
				
				}	
				buf_fin=0;
			bufflag = 1 -bufflag;																			//切换buffer
		}
	}
	
	my_f_close(fmp3);
	Disable_DMA();
	MP3FreeDecoder(Mp3Decoder);	
	myfree(fmp3);			
	myfree(data_read_buf);	
	myfree(data_pcm_buf);	
	myfree(data_pcm_buf2);	
	return rval;
}
