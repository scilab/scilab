
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

#include "wresource.h"
#include "wcommon.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

#include "win_mem_alloc.h" /* MALLOC */

#define MAXSTR 256     /* maximum file name length and general string length */
/* documented in Device Driver Adaptation Guide */

#define PORT_BUF_SIZE 4096

/* Dialog box to select printer port */

char printer_port[32];

EXPORT BOOL CALLBACK SpoolDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  LPSTR entry;
  switch (message)
    {
    case WM_INITDIALOG:
      entry = (LPSTR) lParam;
      while (*entry)
	{
	  SendDlgItemMessage (hDlg, SPOOL_PORT, LB_ADDSTRING, 0, (LPARAM) entry);
	  entry += lstrlen (entry) + 1;
	}
      if ((printer_port == '\0') ||
	  (SendDlgItemMessage (hDlg, SPOOL_PORT, LB_SELECTSTRING, 0, (LPARAM) (LPSTR) printer_port)
	   == LB_ERR))
	SendDlgItemMessage (hDlg, SPOOL_PORT, LB_SETCURSEL, 0, (LPARAM) 0);
      return TRUE;
    case WM_COMMAND:
      switch (LOWORD (wParam))
	{
	case SPOOL_PORT:
	  if (HIWORD (wParam) == LBN_DBLCLK)
	    PostMessage (hDlg, WM_COMMAND, IDOK, 0L);
	  return FALSE;
	case IDOK:
	  SendDlgItemMessage (hDlg, SPOOL_PORT, LB_GETTEXT,
	   (int) SendDlgItemMessage (hDlg, SPOOL_PORT, LB_GETCURSEL, 0, 0L),
			      (LPARAM) (LPSTR) printer_port);
	  EndDialog (hDlg, 1 + (int) SendDlgItemMessage (hDlg, SPOOL_PORT, LB_GETCURSEL, 0, 0L));
	  return TRUE;
	case IDCANCEL:
	  EndDialog (hDlg, 0);
	  return TRUE;
	}
    }
  return FALSE;
}

char *
get_queues (void)
{
  int i;
  DWORD count, needed;
  PRINTER_INFO_1 *prinfo;
  char *enumbuffer;
  char *buffer;
  char *p;
  /* enumerate all available printers */
  EnumPrinters (PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL, NULL, 1, NULL, 0, &needed, &count);
  enumbuffer = MALLOC (needed);
  if (enumbuffer == (char *) NULL)
    return FALSE;
  if (!EnumPrinters (PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL, NULL, 1, (LPBYTE) enumbuffer, needed, &needed, &count))
    {
      FREE (enumbuffer);
      sciprint (MSG_ERROR70, GetLastError ());
      return NULL;
    }
  prinfo = (PRINTER_INFO_1 *) enumbuffer;
  if ((buffer = MALLOC (PORT_BUF_SIZE)) == (char *) NULL)
    {
      FREE (enumbuffer);
      return NULL;
    }
  /* copy printer names to single buffer */
  p = buffer;
  for (i = 0; i < (int) count; i++)
    {
      if ((int) strlen (prinfo[i].pName) + 1 < (PORT_BUF_SIZE - (p - buffer)))
	{
	  strcpy (p, prinfo[i].pName);
	  p += strlen (p) + 1;
	}
    }
  *p = '\0';			/* double null at end */
  FREE (enumbuffer);
  return buffer;
}

/* return TRUE if queuename available */
/* return FALSE if cancelled or error */
/* if queue non-NULL, use as suggested queue */

BOOL 
get_queuename (HINSTANCE hInstance, HWND hwnd, char *portname, char *queue)
{
  char *buffer;
  char *p;
  int i, iport;
  buffer = get_queues ();
  if ((queue == (char *) NULL) || (strlen (queue) == 0))
    {
      /* select a queue */
      iport = DialogBoxParam (hInstance, "QueueDlgBox", hwnd, SpoolDlgProc, (LPARAM) buffer);
      if (!iport)
	{
	  FREE (buffer);
	  return FALSE;
	}
      p = buffer;
      for (i = 1; i < iport && strlen (p) != 0; i++)
	p += lstrlen (p) + 1;
      /* prepend \\spool\ which is used to distinguish */
      /* real files from queues */
      strcpy (portname, "\\\\spool\\");
      strcat (portname, p);
    }
  else
    {
      strcpy (portname, "\\\\spool\\");
      strcat (portname, queue);
    }
  FREE (buffer);
  return TRUE;
}

/******************************************************************
 * Print File to port or queue 
 * port==NULL means prompt for port or queue with dialog box 
 ******************************************************************/

#define PRINT_BUF_SIZE 16384u

int gp_printfile (HINSTANCE hInstance, HWND hwnd, char *filename, char *port)
{
  DWORD count;
  char *buffer;
  char portname[MAXSTR];
  FILE *f;
  HANDLE printer;
  DOC_INFO_1 di;
  DWORD written;

  if (!get_queuename (hInstance, hwnd, portname, port))
    return FALSE;
  port = portname + 8;		/* skip over \\spool\ */

  if ((buffer = MALLOC (PRINT_BUF_SIZE)) == (char *) NULL)
    return FALSE;
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
