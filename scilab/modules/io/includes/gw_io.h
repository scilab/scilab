/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_IO__
#define __GW_IO__
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
int C2F(gw_io)(void);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_oldload) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_read) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getf) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_exec) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lib) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_diary) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_oldsave) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_write) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_print) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_deff) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rat) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_file) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_host) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_unix) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_readb) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_writb) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_execstr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_disp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getpid) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getenv) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_read4b) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_write4b) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_save) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_load) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_mgetl) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fileinfo) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getio) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_setenv) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intexec)();
int C2F(intexecstr)();
int C2F(intgetf)();
int C2F(intsave)();
int C2F(intload)();
int C2F(intdisp)();
int C2F(intexec)();
int C2F(sci_getrelativefilename) _PARAMS((char *fname,unsigned long l));
int C2F(sci_newest) _PARAMS((char *fname,unsigned long l));
int C2F(sci_isdir) _PARAMS((char *fname,unsigned long l));
int C2F(sci_get_absolute_file_path) _PARAMS((char *fname,unsigned long l));
/*-----------------------------------------------------------------------------------*/
#endif /*  __GW_IO__ */
/*-----------------------------------------------------------------------------------*/

