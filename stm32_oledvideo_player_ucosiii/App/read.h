#ifndef __READ_H
#define __READ_H
#include "ff.h"
FRESULT my_f_open (FIL* fp, const TCHAR* path, BYTE mode);				/* Open or create a file */
FRESULT my_f_close (FIL* fp);									/* Close an open file object */
FRESULT my_f_read (FIL* fp, void* buff, UINT btr, UINT* br);			/* Read data from a file */
FRESULT my_f_lseek (FIL* fp, DWORD ofs);
#endif

