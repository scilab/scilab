#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <Winuser.h>
#include "..\version.h"

#include "wcommon.h"
#include "plot.h"


/*-----------------------------------------------------------------------------------*/
/* Les Definitions */
/*-----------------------------------------------------------------------------------*/

#define LCCEXE		"\\lcc\\bin\\lcc.exe" 
#define LCCBIN		"\\lcc\\bin" 
#define LCCINCLUDE	"\\lcc\\include" 
#define LCCLIB		"\\lcc\\lib" 

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
#define GRAPHMENUNAME "wgscilab.mnu"

#define MIN_STACKSIZE 180000

#define MAXCMDTOKENS 128

/** test if we are using a file or not **/
#define isterm(f) (f==stdin || f==stdout || f==stderr)

#define MORESTR "[More (y or n ) ?] "

/* replacement stdio routines that use Text Window for stdin/stdout */
/* WARNING: Do not write to stdout/stderr with functions not listed 
   in win/wtext.h */
#undef kbhit
#undef getche
#undef getch
#undef putch

#undef fgetc
#undef getchar
#undef getc
#undef fgets
#undef gets

#undef fputc
#undef putchar
#undef putc
#undef fputs
#undef puts

#undef fprintf
#undef printf
#undef vprintf
#undef vfprintf

#undef fwrite
#undef fread



/*-----------------------------------------------------------------------------------*/
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

int  sci_show_banner=1;
int SciPlatformId;

char ScilexConsoleName[MAX_PATH];

/*-----------------------------------------------------------------------------------*/
/* Les Fonctions */
/*-----------------------------------------------------------------------------------*/
extern void PrintFile(char *filename);
extern void C2F (tmpdirc) ();
extern void C2F (getwins) (integer *, integer *, integer *);
//extern void C2F (diary) (char *, int *, int);
extern void set_sci_env(char *p, char *wsci) ;


static void CheckMemory (LPSTR str);
static void AllGraphWinDelete ();
static LPSTR my_argv[MAXCMDTOKENS];
static void SciEnv ();
EXPORT int CALLBACK  ShutDown (void);

void set_sci_env(char *p, char *wsci) ;
int Pause (LPSTR str);
int SciWinGetPlatformId ();



void add_sci_argv(char *p);
int sci_iargc();
int sci_getarg(int *n,char *s,long int ls);
int InteractiveMode ();
int C2F(showlogo) ();
int MyPutCh (int ch);
int MyKBHit (void);
int MyGetCh (void);
int MyGetChE (void);
int MyFGetC (FILE * file);
char *MyGetS (char *str);
char * MyFGetS (char *str, unsigned int size, FILE * file);
int MyFPutC (int ch, FILE * file);
int MyFPutS (char *str, FILE * file);
int MyPutS (char *str);
void Scistring (char *str);
int MyFPrintF (FILE * file, char *fmt,...);
int MyPrintF (char *fmt,...);
void sciprint (char *fmt,...);
void sciprint_nd (char *fmt,...);
int sciprint2 (int iv, char *fmt,...);
size_t MyFWrite (const void *ptr, size_t size, size_t n, FILE * file);
size_t MyFRead (void *ptr, size_t size, size_t n, FILE * file);
extern void Xputchar ();
void Xputchar (c);
void Xputstring (str, n);
void C2F (xscisncr) (str, n, dummy);
void C2F (xscistring) (str, n, dummy);
void C2F (xscimore) (n);
void Scisncr (str);
int ReadKey (void);
int WriteStr (char *s, FILE * file);
int WriteKey (char c, FILE * file);


int WINAPI Windows_Main (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow);
int Console_Main(int argc, char **argv);
int MAIN__ ();

void WinExit (void);
void CreateSplashscreen(void);

/* Tue le Process Scilex si OS est Windows 9x */
void Kill_Scilex_Win98(void);
/* Tue le Process Scilex */
void Kill_Scilex(void);

/* Teste si la chaine de caractere correspond à un fichier existant*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsAFile(char *chainefichier);

/* Teste si le fichier a une extension .sav ou .bin*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsABinOrSavFile(char *chainefichier);

BOOL IsAGraphFile(char *chainefichier);
BOOL IsAGraphFilegraph(char *chainefichier);
BOOL IsAGraphFilegraphb(char *chainefichier);

BOOL IsAScicosFile(char *chainefichier);
BOOL IsAScicosFileCOS(char *chainefichier);
BOOL IsAScicosFileCOSF(char *chainefichier);

/* Retourne un numéro valide pour nommer les fenetres associées à ce process */
int FindFreeScilexNumber(void);

void RenameConsole(void);

/* Suivant la commande Via menu ouvre, edit, imprime ...*/
extern void ReplaceSlash(char *pathout,char *pathin);
int CommandByFileExtension(char *fichier,int OpenCode,char *Cmd);

BOOL IsWindowInterface();

void ExtensionFileIntoLowerCase(char *fichier);

void CreateScilabConsole(BOOL ShowBanner);
void CloseScilabConsole(void);

extern BOOL SendCommandToAnotherScilab(char *ScilabWindowNameSource,char *ScilabWindowNameDestination,char *CommandLine);
extern BOOL HaveAnotherWindowScilab(void);
extern char * ChooseAnotherWindowScilab(void);
/*-----------------------------------------------------------------------------------*/
