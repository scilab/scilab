
/* Copyright (C) 1993-1996, Russell Lang.  All rights reserved.

   This file is part of GSview.
   This program is distributed with NO WARRANTY OF ANY KIND.  No author
   or distributor accepts any responsibility for the consequences of using it,
   or for whether it serves any particular purpose or works at all, unless he
   or she says so in writing.  Refer to the GSview Free Public Licence 
   (the "Licence") for full details.
   Every copy of GSview must include a copy of the Licence, normally in a 
   plain ASCII text file named LICENCE.  The Licence grants you the right 
   to copy, modify and redistribute GSview, but only under certain conditions 
   described in the Licence.  Among other things, the Licence requires that 
   the copyright notice and this notice be preserved on all copies.

   1997 : modified for Scilab
   Jean-Philippe Chancelier 
   See gp_printfile : choose a printer and send a file to printer 
   2005 : Allan CORNET INRIA
 */
/*-----------------------------------------------------------------------------------*/
#include "wresource.h"
#include "wcommon.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
#define MAXSTR 256     /* maximum file name length and general string length */
/* documented in Device Driver Adaptation Guide */
#define PORT_BUF_SIZE 4096
/*-----------------------------------------------------------------------------------*/
char printer_port[32];
#define PRINT_BUF_SIZE 16384u
/*-----------------------------------------------------------------------------------*/
extern char* GetPrinterName(void);
/*-----------------------------------------------------------------------------------*/
BOOL get_queuename (HINSTANCE hInstance, HWND hwnd, char *portname, char *queue)
{
	char *PrinterName=NULL;

	PrinterName=GetPrinterName();
	if (strcmp(PrinterName,"EMPTY")==0) return FALSE;
	wsprintf(portname, "\\\\spool\\%s",PrinterName);
	if (PrinterName) { FREE(PrinterName); PrinterName=NULL;}

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
/******************************************************************
 * Print File to port or queue 
 * port==NULL means prompt for port or queue with dialog box 
 ******************************************************************/
int gp_printfile (HINSTANCE hInstance, HWND hwnd, char *filename, char *port)
{
  DWORD count;
  char *buffer;
  char portname[MAXSTR];
  FILE *f;
  HANDLE printer;
  DOC_INFO_1 di;
  DWORD written;

  if (!get_queuename (hInstance, hwnd, portname, port))    return FALSE;
  port = portname + 8;		/* skip over \\spool\ */

  if ((buffer = MALLOC (PRINT_BUF_SIZE)) == (char *) NULL) return FALSE;
  if (filename != (char *) 0)
    {
      if ((f = fopen (filename, "rb")) == (FILE *) NULL)
	{
	  FREE (buffer);
	  return FALSE;
	}
    }
  else
    f = stdin;

  /* open a printer */
  if (!OpenPrinter (port, &printer, NULL))
    {
      sciprint (MSG_ERROR71, port, GetLastError ());
      FREE (buffer);
      return FALSE;
    }
  /* from here until ClosePrinter, should AbortPrinter on error */

  di.pDocName = filename;
  di.pOutputFile = NULL;
  di.pDatatype = "RAW";		/* for available types see EnumPrintProcessorDatatypes */
  if (!StartDocPrinter (printer, 1, (LPBYTE) & di))
    {
      sciprint (MSG_ERROR72, GetLastError ());
      AbortPrinter (printer);
      FREE (buffer);
      return FALSE;
    }

  while ((count = fread (buffer, 1, PRINT_BUF_SIZE, f)) != 0)
    {
      if (!WritePrinter (printer, (LPVOID) buffer, count, &written))
	{
	  FREE (buffer);
	  fclose (f);
	  AbortPrinter (printer);
	  return FALSE;
	}
    }
  fclose (f);
  FREE (buffer);

  if (!EndDocPrinter (printer))
    {
      sciprint (MSG_ERROR73, GetLastError ());
      AbortPrinter (printer);
      return FALSE;
    }

  if (!ClosePrinter (printer))
    {
      sciprint (MSG_ERROR74, GetLastError ());
      return FALSE;
    }
  return TRUE;
}
/*-----------------------------------------------------------------------------------*/
