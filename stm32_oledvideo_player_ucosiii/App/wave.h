#ifndef _WAVE_H
#define _WAVE_H
#include "ff.h"
#include "stm32f10x.h"
//RIFF��
typedef __packed struct
{
	uint32_t ChunkID;		   		//chunk id�̶�Ϊ"RIFF",��0X46464952
	uint32_t ChunkSize ;		  //���ϴ�С=�ļ��ܴ�С-8
	uint32_t Format;	   			//��ʽ;WAVE,��0X45564157
}ChunkRIFF ;
//fmt��
typedef __packed struct
{
	uint32_t ChunkID;		   		//chunk id;����̶�Ϊ"fmt ",��0X20746D66
	uint32_t ChunkSize ;		  //�Ӽ��ϴ�С(������ID��Size);����Ϊ:20.
	uint16_t AudioFormat;	  	//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	uint16_t NumOfChannels;		//ͨ������;1,��ʾ������;2,��ʾ˫����;
	uint32_t SampleRate;			//������;0X1F40,��ʾ8Khz
	uint32_t ByteRate;				//�ֽ�����; 
	uint16_t BlockAlign;			//�����(�ֽ�); 
	uint16_t BitsPerSample;		//�����������ݴ�С;4λADPCM,����Ϊ4
	//uint16_t ByteExtraData;		//���ӵ������ֽ�;2��; ����PCM,û���������
}ChunkFMT;	   
//fact�� 
typedef __packed struct 
{
	uint32_t ChunkID;		   		//chunk id;����̶�Ϊ"fact",��0X74636166;����PCMû���������
	uint32_t ChunkSize ;		  //�Ӽ��ϴ�С(������ID��Size);����Ϊ:4.
	uint32_t FactSize;	  		//ת����PCM���ļ���С; 
}ChunkFACT;
//LIST�� 
typedef __packed struct 
{
	uint32_t ChunkID;		   		//chunk id;����̶�Ϊ"LIST",��0X74636166;
	uint32_t ChunkSize ;		  //�Ӽ��ϴ�С(������ID��Size);����Ϊ:4. 
}ChunkLIST;

//data�� 
typedef __packed struct 
{
	uint32_t ChunkID;		   		//chunk id;����̶�Ϊ"data",��0X5453494C
	uint32_t ChunkSize ;		  //�Ӽ��ϴ�С(������ID��Size) 
}ChunkDATA;

//wavͷ
typedef __packed struct
{ 
	ChunkRIFF riff;						//riff��
	ChunkFMT fmt;  						//fmt��
//	ChunkFACT fact;						//fact�� ����PCM,û������ṹ��	 
	ChunkDATA data;						//data��		 
}__WaveHeader; 

//wav ���ſ��ƽṹ��
typedef __packed struct
{ 
	uint16_t audioformat;			//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	uint16_t nchannels;				//ͨ������;1,��ʾ������;2,��ʾ˫����; 
	uint16_t blockalign;			//�����(�ֽ�);  
	uint32_t datasize;				//WAV���ݴ�С 
	uint32_t totsec ;					//���׸�ʱ��,��λ:��
	uint32_t cursec ;					//��ǰ����ʱ��
	uint32_t bitrate;	   			//������(λ��)
	uint32_t samplerate;			//������ 
	uint16_t bps;							//λ��,����16bit,24bit,32bit
	uint32_t datastart;				//����֡��ʼ��λ��(���ļ������ƫ��)
}wavctrl; 

#define WAVEFILEBUFSIZE		2048
//extern uint8_t WaveFileBuf[WAVEFILEBUFSIZE];
//extern uint8_t TempBuf[WAVEFILEBUFSIZE/2];
extern wavctrl WaveCtrlData;
#define WAV_MAX_LENGTH 0xFFFF
uint8_t wave_decode_init(char* fname,wavctrl* wavx);
FRESULT ScanWavefiles (char* path);
u8 wav_play_song(char *path);
u8 wav_record_song(char *path,u32 ms);
u8 wav_record_song_to_flash(u32 ms,u32 *actualTime);
u8 wav_play_song_from_flash(void);
u8 wav_record_song_to_flash_without_compress(u32 ms);
u8 wav_play_song_from_flash_without_compress(void);
u8 wav_record_song_to_flash_with_interrupt(u32 ms,u32 *actualTime);
u8 wav_play_song_from_flash_with_interrupt(void);
u8 mp3_play_song(char* path);
extern volatile u8 canPlayFlag;
extern volatile u8 recordInterrupted;
extern volatile u8 playInterrupted;
extern volatile u8 recordFlag;
extern volatile u8 playFlag;
#endif
