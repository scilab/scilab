/*
 * 
 */
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#ifndef __ABSC__
#include <process.h>
#endif

#define MAXSTR 256     /* maximum file name length and general string length */

/* in gvwprn.c */

extern int gp_printfile(HINSTANCE hInstance,HWND hwnd,char *filename, char *port);

#ifdef __GNUC__XXX
/** XXXXXX Missing declarations for gcwin32 **/
typedef PRINTER_DEFAULTS *LPPRINTER_DEFAULTS;

#define PRINTER_ENUM_DEFAULT     0x00000001
#define PRINTER_ENUM_LOCAL       0x00000002
#define PRINTER_ENUM_CONNECTIONS 0x00000004

#ifndef UNICODE_ONLY
WINBOOL
STDCALL
OpenPrinterA(
   LPSTR    pPrinterName,
   LPHANDLE phPrinter,
   LPPRINTER_DEFAULTS pDefault
);
#endif //!UNICODE_ONLY
#ifndef ANSI_ONLY
WINBOOL
STDCALL
OpenPrinterW(
   LPWSTR    pPrinterName,
   LPHANDLE phPrinter,
   LPPRINTER_DEFAULTS pDefault
);
#endif //!ANSI_ONLY
#ifdef UNICODE
#define OpenPrinter  OpenPrinterW
#else
#define OpenPrinter  OpenPrinterA
#endif // !UNICODE

WINBOOL STDCALL AbortPrinter(HANDLE   hPrinter);
WINBOOL STDCALL EndDocPrinter(HANDLE   hPrinter);
WINBOOL STDCALL ClosePrinter(HANDLE hPrinter);

WINBOOL STDCALL WritePrinter(
    HANDLE  hPrinter,
    LPVOID  pBuf,
    DWORD   cbBuf,
    LPDWORD pcWritten
);


#ifndef UNICODE_ONLY
WINBOOL STDCALL 
EnumPrintersA(
    DWORD   Flags,
    LPSTR   Name,
    DWORD   Level,
    LPBYTE  pPrinterEnum,
    DWORD   cbBuf,
    LPDWORD pcbNeeded,
    LPDWORD pcReturned
);
#endif //!UNICODE_ONLY
#ifndef ANSI_ONLY
WINBOOL STDCALL EnumPrintersW(
    DWORD   Flags,
    LPWSTR   Name,
    DWORD   Level,
    LPBYTE  pPrinterEnum,
    DWORD   cbBuf,
    LPDWORD pcbNeeded,
    LPDWORD pcReturned
);
#endif //!ANSI_ONLY
#ifdef UNICODE
#define EnumPrinters  EnumPrintersW
#else
#define EnumPrinters  EnumPrintersA
#endif // !UNICODE

#ifndef UNICODE_ONLY
DWORD
STDCALL
StartDocPrinterA(
    HANDLE  hPrinter,
    DWORD   Level,
    LPBYTE  pDocInfo
);
#endif //!UNICODE_ONLY
#ifndef ANSI_ONLY
DWORD
STDCALL
StartDocPrinterW(
    HANDLE  hPrinter,
    DWORD   Level,
    LPBYTE  pDocInfo
);
#endif //!ANSI_ONLY
#ifdef UNICODE
#define StartDocPrinter  StartDocPrinterW
#else
#define StartDocPrinter  StartDocPrinterA
#endif // !UNICODE

#endif /* __GNUC__ */
