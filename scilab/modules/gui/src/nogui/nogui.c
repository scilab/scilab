/*-----------------------------------------------------------------------------------*/
/* INRIA 2006/2007 */
/* @author Allan CORNET */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
#include <errno.h>
#include "machine.h" 
#include "Scierror.h"
#include "sciprint.h"
#include "nogui.h"
#include "sxevents.h"
#include "xscimore.h"
#include "checkevts.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_gui)()
{
	Scierror(999,"GUI interface not installed.\r\n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL TerminateGUI(void)
{
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/ 
int XClearScreenConsole(char *fname)
{
	sciprint("\n Only in Window Mode \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int C2F(sxevents)(){
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void C2F(xscimore)(int *n){
	n=0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(checkevts)(int *i){
	i=0;
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void str_to_xterm ( register char * string, int nbytes){

}
/*-----------------------------------------------------------------------------------*/ 
void Click_menu(int n){
	
}
/*-----------------------------------------------------------------------------------*/ 
int GetScreenProperty(char *prop, char *value){
	return 0;
}
/*-----------------------------------------------------------------------------------*/
#define LF                    0x000a
int Xorgetchar(int interrupt){
	int i;
int inter_max_plus1 = 0;
	static fd_set Select_mask_ref,select_mask,Write_mask_ref,write_mask;
	static struct timeval select_timeout;
	static int fd_in=0,fd_out=0,fd_err=0 ;
  static int state = 0;
  fd_in = fileno(stdin) ;
  fd_out = fileno(stdout);
  fd_err = fileno(stderr);

  FD_ZERO(&Select_mask_ref);
  FD_SET(fd_in , &Select_mask_ref);
  FD_ZERO(&Write_mask_ref);

  inter_max_plus1 = fd_in;

  inter_max_plus1 = Max(fd_out,inter_max_plus1);
  inter_max_plus1 = Max(fd_err,inter_max_plus1);
  inter_max_plus1++;

	  for( ; ; ) {
    fflush(stdout); 
    fflush(stderr); 

    /* Initialize masks  */
    select_mask = Select_mask_ref;
    write_mask  = Write_mask_ref;

    select_timeout.tv_sec = 0;
    select_timeout.tv_usec = 10;
    i = select(inter_max_plus1, &select_mask, &write_mask, (fd_set *)NULL, &select_timeout);
    if (i < 0) {
		if (errno != EINTR) /* EINTR  A signal was caught. */
	{ 
	  sciprint("Error.\n");
	  exit(0);
	  continue;
	}
    } 

    /* if there's something to output */
    if ( FD_ISSET(fd_out,&write_mask)) { 
      fflush(stdout); 
    }
    if ( FD_ISSET(fd_err,&write_mask)) { 
      fflush(stderr); 
    }

    /* if there's something to read */
    if (FD_ISSET(fd_in,&select_mask)) 
      state=1;

    if (state) {
		i=getchar();
      if (i==LF) state=0;
      return(i);
    }
	
    if (interrupt&&(ismenu()==1)) return(-1);

  }
}
/*-----------------------------------------------------------------------------------*/
int XEvorgetchar(int interrupt){
	return 0;
}
/*-----------------------------------------------------------------------------------*/
void main_sci (char *startup, int lstartup,int memory){
	Scierror(999,"GUI interface not installed.\r\n");
}
/*-----------------------------------------------------------------------------------*/
void InitXsession(void){
	Scierror(999,"GUI interface not installed.\r\n");
}
