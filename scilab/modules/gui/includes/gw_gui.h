/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_GUI__
#define __GW_GUI__
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
int C2F(gw_gui)(void);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_x_dialog) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_message) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_choose) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_mdialog) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_xchoicesi) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_addmenu_old) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_delmenu_old) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_setmenu_old) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_unsetmenu_old) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_xgetfile) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_message_modeless) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_raise_window) _PARAMS(( char * fname, unsigned long fname_len )) ;
int C2F(sci_getlookandfeel) _PARAMS(( char * fname, unsigned long fname_len )) ;
int C2F(sci_getinstalledlookandfeels) _PARAMS(( char * fname, unsigned long fname_len )) ;
int C2F(sci_setlookandfeel) _PARAMS(( char * fname, unsigned long fname_len )) ;
/*-----------------------------------------------------------------------------------*/
#endif /*  __GW_GUI__ */
/*-----------------------------------------------------------------------------------*/

