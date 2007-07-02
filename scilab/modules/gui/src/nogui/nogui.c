/*-----------------------------------------------------------------------------------*/
/* INRIA 2006/2007 */
/* @author Allan CORNET */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 
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


int Xorgetchar(int interrupt){
	return getchar();
}


int XEvorgetchar(int interrupt){
	return 0;
}

void main_sci (char *startup, int lstartup,int memory){
	Scierror(999,"GUI interface not installed.\r\n");
}

void InitXsession(void){
	Scierror(999,"GUI interface not installed.\r\n");
}
