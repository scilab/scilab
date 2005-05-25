#ifndef __WINMAIN_H__
#define __WINMAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <Winuser.h>
#include "..\version.h"

#include "wcommon.h"
#include "plot.h"
#include "FilesAssociations.h"
#include "SciEnv.h"

/*-----------------------------------------------------------------------------------*/
/* Les Definitions */
/*-----------------------------------------------------------------------------------*/


#if (defined __MSC__ ) || (defined __MINGW32__) 
#define putenv(x) _putenv(x)
#endif

#ifdef __ABSC__
#define putenv(x) abs_putenv(x)
#endif

/* limits */
//#define MAXSTR 255
#define MAXPRINTF 512


#define MENUNAME "wscilabE.mnu"
#define GRAPHMENUNAME "wgscilabE.mnu"

#define MIN_STACKSIZE 180000

#define MAXCMDTOKENS 128

/** test if we are using a file or not **/
#define isterm(f) (f==stdin || f==stdout || f==stderr)

#define MORESTR "[More (y or n ) ?] "

/* Les Variables Globales */
/*-----------------------------------------------------------------------------------*/
TW textwin;
GW graphwin;
PW pausewin;
MW menuwin;

LPSTR szModuleName;
LPSTR szMenuName;
LPSTR szGraphMenuName;

static int startupf = 0; /** 0 if we execute startup else 1 **/
static int nointeractive = 0;
static int show_logo = 1;
static int  memory = MIN_STACKSIZE;
static int my_argc = -1;

/*-----------------------------------------------------------------------------------*/
/* Les Fonctions */
/*-----------------------------------------------------------------------------------*/
extern void PrintFile(char *filename);
extern void C2F (tmpdirc) ();
extern void C2F (getwins) (integer *, integer *, integer *);
extern void start_sci_gtk();
extern void sci_tk_activate(void);
extern void ChangeCursorWhenScilabIsReady(void);

static void CheckMemory (LPSTR str);
static void AllGraphWinDelete ();
static LPSTR my_argv[MAXCMDTOKENS];
EXPORT int CALLBACK  ShutDown (void);
int Pause (LPSTR str);
void add_sci_argv(char *p);
int sci_iargc();
int sci_getarg(int *n,char *s,long int ls);
int InteractiveMode ();
int C2F(showlogo) ();
void WinExit (void);
void CreateSplashscreen(void);
/* Tue le Process Scilex si OS est Windows 9x */
void Kill_Scilex_Win98(void);
/* Tue le Process Scilex */
void Kill_Scilex(void);
BOOL ForbiddenToUseScilab(void);

int WINAPI Windows_Main (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow);
int Console_Main(int argc, char **argv);
int MAIN__ ();

/*-----------------------------------------------------------------------------------*/
#endif /*  __WINMAIN_H__ */
