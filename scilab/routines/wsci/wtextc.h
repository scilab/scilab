/************************************
 * redefine functions that use console 
 * Scilab 1997
 *   Jean-Philippe Chancelier 
 ************************************/

/* 
   With MSVC++ or  mingwin32 the following redefinition 
   for console mode are maybe useless 
**/
/** 
#define getche() ReadKey()
#define getch()  ReadKey()
#define _getch()  ReadKey()
#undef  getc
#define getc(file)  ReadKey()
#define getc(file)  ReadKey()
**/
/** 
#define fputc(ch,file)   WriteKey(ch,file)
#define fputs(str,file)  WriteStr(str,file)
**/

/* now cause errors for some unimplemented functions */
#define puts(str)        dont_usePutS(str)
/** #define fgets(str,sz,file)  dont_useFGetS(str,sz,file) **/
#define gets(str)  	    dont_useGetS(str)
#define fgetc(file) dont_useFGetC(file)
#undef  getchar
#define getchar()   dont_useFGetC(stdin)
#define putch(ch)  dont_usePutCh(ch)
#define kbhit()  dont_useKBHit()
#define vprintf dontuse_vprintf
#define vfprintf dontuse_vfprintf
#define fscanf dontuse_fscanf
#define scanf dontuse_scanf
#define clreol dontuse_clreol
#define clrscr dontuse_clrscr
#define gotoxy dontuse_gotoxy
#define wherex dontuse_wherex
#define wherey dontuse_wherey
#define cgets dontuse_cgets
#define cprintf dontuse_cprintf
#define cputs dontuse_cputs
#define cscanf dontuse_cscanf
#define ungetch dontuse_ungetch
#define fprintf dont_useFPrintF
#define printf dont_usePrintF
#define fwrite(ptr, size, n, stream) dont_useFWrite(ptr, size, n, stream)
#define fread(ptr, size, n, stream) dont_useFRead(ptr, size, n, stream)

/* now for the prototypes */


