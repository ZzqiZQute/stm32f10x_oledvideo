#include "read.h"
#include "os.h"
#include "ff.h"

FRESULT my_f_open (FIL* fp, const TCHAR* path, BYTE mode)				/* Open or create a file */
{
	CPU_SR_ALLOC();
	FRESULT f;
	OS_CRITICAL_ENTER();
	f=f_open(fp,path,mode);
	OS_CRITICAL_EXIT();
	return f;
}
FRESULT my_f_close (FIL* fp)									/* Close an open file object */
{
	CPU_SR_ALLOC();
	FRESULT f;
	OS_CRITICAL_ENTER();
	f=f_close(fp);
	OS_CRITICAL_EXIT();
	return f;
}
FRESULT my_f_read (FIL* fp, void* buff, UINT btr, UINT* br)			/* Read data from a file */
{
	CPU_SR_ALLOC();
	FRESULT f;
	OS_CRITICAL_ENTER();
	f=f_read (fp, buff, btr, br);
	OS_CRITICAL_EXIT();
	return f;
}
FRESULT my_f_lseek (FIL* fp, DWORD ofs)								/* Move file pointer of a file object */
{
	CPU_SR_ALLOC();
	FRESULT f;
	OS_CRITICAL_ENTER();
	f=f_lseek (fp, ofs);
	OS_CRITICAL_EXIT();
	return f;
}


