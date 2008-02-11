/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_GUI__
#define __GW_GUI__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_gui)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_x_dialog) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_message) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_choose) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_mdialog) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_xchoicesi) _PARAMS((char *fname,unsigned long fname_len));
int sci_delmenu(char *fname,unsigned long fname_len);
int sci_setmenu(char *fname,unsigned long fname_len);
int sci_unsetmenu(char *fname,unsigned long fname_len);
int C2F(sci_xgetfile) _PARAMS((char *fname,unsigned long fname_len));
int sci_uigetdir(char *fname,unsigned long l);
int C2F(sci_x_message_modeless) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_raise_window) _PARAMS(( char * fname, unsigned long fname_len )) ;
int C2F(sci_getlookandfeel) _PARAMS(( char * fname, unsigned long fname_len )) ;
int C2F(sci_getinstalledlookandfeels) _PARAMS(( char * fname, unsigned long fname_len )) ;
int C2F(sci_setlookandfeel) _PARAMS(( char * fname, unsigned long fname_len )) ;
int C2F(sci_ClipBoard) _PARAMS((char *fname,unsigned long l));
int C2F(sci_hidetoolbar) _PARAMS((char *fname,unsigned long l));
int C2F(sci_toolbar) _PARAMS((char *fname,unsigned long l));
int C2F(sci_about) _PARAMS((char *fname,unsigned long l));
int sci_uicontrol(char *fname,unsigned long l);
int sci_uimenu(char *fname,unsigned long l);
int sci_mpopup(char *fname,unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_GUI__ */
/*--------------------------------------------------------------------------*/

