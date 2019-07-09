#include "flash2.h"
#include "stm32f10x_it.h"
#include "stm32f10x_flash.h"
//#define 



u8 Write_Flash(u32 addr,u32 *buff, u8 len)
{    
    volatile FLASH_Status FLASHStatus;
    u8 k=0;
    u32 Address;

    Address = addr;
    FLASHStatus = FLASH_COMPLETE;
    FLASH_Unlock();//??
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//??????
  //  FLASHStatus = FLASH_ErasePage(WriteFlashAddress);//????
    if(FLASHStatus == FLASH_COMPLETE)
    {
        for(k=0;(k<len) && (FLASHStatus == FLASH_COMPLETE);k++)
        {
            FLASHStatus = FLASH_ProgramWord(Address, buff[k]);//?????(32?)????????
            Address = Address + 4;//????4???
        }        
        FLASH_Lock();//????,?????
    }
    else
    {
        return 0;
    }
    if(FLASHStatus == FLASH_COMPLETE)
    {
        return 1;
    }
    return 0;
}


void Read_Flash(u32 addr,u32 *buff, u8 len)
{
    u8 k;
    u32 Address;
    
    Address = addr;
    for(k=0;k<len;k++)
    {
        buff[k] =  (*(vu32*) Address);//????????????
        Address += 4;//????4???        
    }
}
void Reset_Flash(void){
	 int i=0;
	 FLASH_Unlock();//??
   FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//??????
	 for(;i<MAX_FLASH_PAGE+1;i++)
		FLASH_ErasePage(SoundSizeAddress+2048*i);
	 FLASH_Lock();
}
u8 Write_Flash_At(u32 addr,u8 *buff, u32 len)
{    
    volatile FLASH_Status FLASHStatus;
    u32 k=0;
		u32 m=0;
	  u32 a[4];
    s32 l = len;
    u32 Address;
		u32 addrt,t,s,u=0;
	  u32 tmpbuf;
    Address = addr;
    FLASHStatus = FLASH_COMPLETE;
    FLASH_Unlock();//??
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//??????
    //FLASHStatus = FLASH_ErasePage(addr);//????
    //if(FLASHStatus == FLASH_COMPLETE)
    //{
				if((t=addr%4)!=0)
				{
						addrt = addr - t;
					  Read_Flash(addrt,&tmpbuf,1);
						for(k=0;k<4;k++)
						{
							if(k>=t){
								a[k]= buff[u];
								u++;
							}
							else
								a[k]=(u8)(tmpbuf>>(8*k));
						}
						tmpbuf=0;
						for(k=0;k<4;k++)
						{
							tmpbuf+= ((u32)(a[k]))<< (8*k);
						}
					
 						FLASHStatus = FLASH_ProgramWord(addrt, tmpbuf);
						l -= 4 - t;
						if(l<=0)return 1;
						Address = addrt +4;
				}
				t = l % 4;
				s = l / 4;
        for(k=0;(k<s);k++)
        {
						tmpbuf = 0;
						for(m=0;m<4;m++)
						{
							tmpbuf+= (buff[u]<< ( 24- 8*m));
							u++;
						}
					
            FLASHStatus = FLASH_ProgramWord(Address, tmpbuf);//?????(32?)????????
            Address = Address + 4;//????4???
        }
				if(t!=0){
					Read_Flash(Address,&tmpbuf,1);
					for(k=0;k<4;k++)
					{
						if(k<t){
							a[k]= buff[u];
							u++;
						}
						else
							a[k]=(u8)(tmpbuf>>(32-8*k));
					}
					tmpbuf=0;
					for(k=0;k<4;k++)
					{
						tmpbuf+= ((u32)(a[k]))<< (8*k);
					}
				
					FLASHStatus = FLASH_ProgramWord(Address, tmpbuf);				
				}
        FLASH_Lock();//????,?????
  //  }
  //  else
  //  {
 //       return 0;
  //  }
    if(FLASHStatus == FLASH_COMPLETE)
    {
        return 1;
    }
    return 0;
}
void Read_Flash_At(u32 addr,u8 *buff, u32 len)
{
		u32 k=0;
    u32 Address;
		u32 m=0;
	  u32 a[4];
    s32 l = len;
		u32 addrt,t,s,u=0;
	  u32 tmpbuf;
    Address = addr; 
    if((t=addr%4)!=0)
				{
						addrt = addr - t;
					  Read_Flash(addrt,&tmpbuf,1);
						for(k=0;k<4;k++)
						{
								a[k]=(u8)(tmpbuf>>(8*k));
						}
					
						for(k=t;k<4;k++)
						{
							  buff[u++]=(a[k]);
						}
						l -= 4 - t;
						if(l<=0) return;
						Address = addrt +4;
				}
				t = l % 4;
				s = l / 4;
        for(k=0;(k<s) ;k++)
        {
						Read_Flash(Address,&tmpbuf,1);
						for(m=0;m<4;m++)
						{
							buff[u++]= (u8)(tmpbuf>>(24-8*m));
						
						}
						
            Address = Address + 4;//????4???
        }
				if(t!=0){
					Read_Flash(Address,&tmpbuf,1);
					for(k=0;k<4;k++)
					{
						if(k<t){
						  buff[u++]=(u8)(tmpbuf>>(8*k));
						
						}
					}
			
				}
      
}
