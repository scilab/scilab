
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
 */

#include "gvwin.h"
#include "wgnuplib.h"
#include "wresource.h"
#include "wcommon.h"

/** 
  Warning : Remove AbortPrinter from 
  b18/H-i386-cygwin32/i386-cygwin32/include/Windows32/Functions.h
  for gc-win32 
  **/

#ifdef GVW_ALONE
/* standalone executable for print spool */
/* get a filename and spool it for printing */
/* XXXX : a finir en rajoutant des arguments * */

int 
main (int argc, char *argv[])
{
  HWND parent;
  HANDLE x = GetModuleHandleA (0);
  parent = GetActiveWindow ();
  if (argc == 2)
    gp_printfile (x, parent, argv[1], (char *) 0);
  else
    gp_printfile (x, parent, (char *) 0, (char *) 0);
  return (0);
}

void 
sciprint (char *fmt,...)
{
  int count;
  va_list args;
  va_start (args, fmt);
  count = vfprintf (stdout, fmt, args);
}

#endif /* GW_ALONE */

/* documented in Device Driver Adaptation Guide */

#define PORT_BUF_SIZE 4096

/* Dialog box to select printer port */

char printer_port[32];

EXPORT BOOL CALLBACK
SpoolDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
  enumbuffer = malloc (needed);
  if (enumbuffer == (char *) NULL)
    return FALSE;
  if (!EnumPrinters (PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL, NULL, 1, (LPBYTE) enumbuffer, needed, &needed, &count))
    {
      free (enumbuffer);
      sciprint ("EnumPrinters() failed, error code = %d", GetLastError ());
      return NULL;
    }
  prinfo = (PRINTER_INFO_1 *) enumbuffer;
  if ((buffer = malloc (PORT_BUF_SIZE)) == (char *) NULL)
    {
      free (enumbuffer);
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
  free (enumbuffer);
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
	  free (buffer);
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
  free (buffer);
  return TRUE;
}

/******************************************************************
 * Print File to port or queue 
 * port==NULL means prompt for port or queue with dialog box 
 ******************************************************************/

#define PRINT_BUF_SIZE 16384u

int 
gp_printfile (HINSTANCE hInstance, HWND hwnd, char *filename, char *port)
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

  if ((buffer = malloc (PRINT_BUF_SIZE)) == (char *) NULL)
    return FALSE;
  if (filename != (char *) 0)
    {
      if ((f = fopen (filename, "rb")) == (FILE *) NULL)
	{
	  free (buffer);
	  return FALSE;
	}
    }
  else
    f = stdin;

  /* open a printer */
  if (!OpenPrinter (port, &printer, NULL))
    {
      sciprint ("OpenPrinter() failed for \042%s\042, error code = %d", port, GetLastError ());
      free (buffer);
      return FALSE;
    }
  /* from here until ClosePrinter, should AbortPrinter on error */

  di.pDocName = filename;
  di.pOutputFile = NULL;
  di.pDatatype = "RAW";		/* for available types see EnumPrintProcessorDatatypes */
  if (!StartDocPrinter (printer, 1, (LPBYTE) & di))
    {
      sciprint ("StartDocPrinter() failed, error code = %d", GetLastError ());
      AbortPrinter (printer);
      free (buffer);
      return FALSE;
    }

  while ((count = fread (buffer, 1, PRINT_BUF_SIZE, f)) != 0)
    {
      if (!WritePrinter (printer, (LPVOID) buffer, count, &written))
	{
	  free (buffer);
	  fclose (f);
	  AbortPrinter (printer);
	  return FALSE;
	}
    }
  fclose (f);
  free (buffer);

  if (!EndDocPrinter (printer))
    {
      sciprint ("EndDocPrinter() failed, error code = %d", GetLastError ());
      AbortPrinter (printer);
      return FALSE;
    }

  if (!ClosePrinter (printer))
    {
      sciprint ("ClosePrinter() failed, error code = %d", GetLastError ());
      return FALSE;
    }
  return TRUE;
}
