/*******************************************
 * Original source : GNUPLOT - win/wmenu.c
 * modified for Scilab 
 *******************************************
 * Copyright (C) 1992   Maurice Castro, Russell Lang
 *
 * Permission to use, copy, and distribute this software and its
 * documentation for any purpose with or without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and 
 * that both that copyright notice and this permission notice appear 
 * in supporting documentation.
 *
 * Permission to modify the software is granted, but not the right to
 * distribute the modified code.  Modifications are to be distributed 
 * as patches to released version.
 *  
 * This software is provided "as is" without express or implied warranty.
 * AUTHORS
 *   Maurice Castro
 *   Russell Lang
 * Modified for Scilab/gc-win32 (1997)
 *   Jean-Philippe Chancelier 
 */

#include <stdio.h>
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <string.h>		/* only use far items */
#include "wgnuplib.h"
#include "wresource.h"
#include "wcommon.h"

EXPORT BOOL CALLBACK InputBoxDlgProc (HWND, UINT, WPARAM, LPARAM);
EXPORT LRESULT CALLBACK MenuButtonProc (HWND, UINT, WPARAM, LPARAM);

/* limits */
#define MAXSTR 255
#define MACROLEN 5000
/* #define NUMMENU 256  defined in wresourc.h */
#define MENUDEPTH 3

/* menu tokens */

#define CMDMIN 129
#define INPUT 129
#define EOS 130
/** if ope and save are changed : see wgmenu.c to change LOADSCG and SAVESCG*/
#define OPEN 131
#define SAVE 132
#define HELPDIALOG 133
#define PLUS 134
#define MINUS 135
#define RAISE 136
#define SET  137
#define DELETESCIW 138
#define MCOPY 139
#define PASTE 140
#define CHOOSETHEFONT 141
#define CMDMAX 141

char *keyword[] =
{
  "[INPUT]", "[EOS]", "[OPEN]", "[SAVE]", "[HELP]", "[PLUS]", "[MINUS]",
  "[RAISE]", "[SET]", "[DELETESCIW]","[MCOPY]", "[PASTE]", "[CHOOSETHEFONT]",
  "{ENTER}", "{ESC}", "{TAB}",
  "{^A}", "{^B}", "{^C}", "{^D}", "{^E}", "{^F}", "{^G}", "{^H}",
  "{^I}", "{^J}", "{^K}", "{^L}", "{^M}", "{^N}", "{^O}", "{^P}",
  "{^Q}", "{^R}", "{^S}", "{^T}", "{^U}", "{^V}", "{^W}", "{^X}",
  "{^Y}", "{^Z}", "{^[}", "{^\\}", "{^]}", "{^^}", "{^_}",
  NULL};

BYTE keyeq[] =
{
  INPUT, EOS, OPEN, SAVE, HELPDIALOG, PLUS, MINUS,
  RAISE, SET, DELETESCIW, MCOPY, PASTE, CHOOSETHEFONT,
  13, 27, 9,
  1, 2, 3, 4, 5, 6, 7, 8,
  9, 10, 11, 12, 13, 14, 15, 16,
  17, 18, 19, 20, 21, 22, 23, 24,
  25, 26, 28, 29, 30, 31,
  0 /* NULL */ };

static void Countp ();
static void Countm ();
static void SendCountSet ();
static void SendCountRaise ();
static void SendCountDelete ();

/*********************************
 * Send a macro to the text window 
 * lptw : text window 
 * m    : macro reference in the text 
 *        window menu ( lptw->lpmw )
 * s = lpmw->macro[m]; is the macro code 
 *   the macro code is parsed in this routine 
 *   to detect special values which implies special 
 *   routine calling to get the string ( ex OPEN ) 
 *********************************/

void 
SendMacro (LPTW lptw, UINT m)
{
  BYTE *s;
  char *d;
  char *buf;
  BOOL flag = TRUE;
  int i;
  HDC hdc;
  LPMW lpmw = lptw->lpmw;
  if ((buf = LocalAlloc (LPTR, MAXSTR + 1)) == (char *) NULL)
    return;
  if (m >= NUMMENU || m < 0)
    return;
  s = lpmw->macro[m];
  d = buf;
  *d = '\0';
  while (s && *s && (d - buf < MAXSTR))
    {
      if (*s >= CMDMIN && *s <= CMDMAX)
	{
	  int ierr = 0;
/** Special cases **/
	  switch (*s)
	    {
	    case SAVE:		/* [SAVE] - get a save filename from a file list box */
	    case OPEN:		/* [OPEN] - get a filename from a file list box */
	      flag = SciOpenSave (lptw->hWndParent, &s, &d, &ierr);
	      if (flag == 0 || ierr == 1)
		{
		  LocalFree (buf);
		  return;
		}
	      break;
	    case INPUT:	/* [INPUT] - input a string of characters */
	      s++;
	      for (i = 0; (*s >= 32 && *s <= 126); i++)
		lpmw->szPrompt[i] = *s++;
	      lpmw->szPrompt[i] = '\0';
	      lpmw->lpProcInput = (DLGPROC) MyGetProcAddress ("InputBoxDlgProc",
							   InputBoxDlgProc);
	      flag = DialogBox (hdllInstance, "InputDlgBox", lptw->hWndParent, lpmw->lpProcInput);
	      if (flag)
		{
		  for (i = 0; i < lpmw->nChar; i++)
		    *d++ = lpmw->szAnswer[i];
		}
	      break;
	    case HELPDIALOG:
	      DoHelpDialog ();
	      s++;
	      break;
	    case PLUS:
	      Countp ();
	      s++;
	      break;
	    case MINUS:
	      Countm ();
	      s++;
	      break;
	    case RAISE:
	      SendCountRaise ();
	      s++;
	      break;
	    case SET:
	      SendCountSet ();
	      s++;
	      break;
	    case DELETESCIW:
	      SendCountDelete ();
	      s++;
	      break;
	    case MCOPY:
			TextCopyClip (lptw);
			s++;
	      break;
	    case PASTE:
			{
				HGLOBAL hGMem;
				BYTE *cbuf;
				TEXTMETRIC tm;
				UINT type;
				/* find out what type to get from clipboard */
				hdc = GetDC (lptw->hWndText);
				SelectFont (hdc, lptw->hfont);
				GetTextMetrics (hdc, (TEXTMETRIC *) & tm);
				if (tm.tmCharSet == OEM_CHARSET)
					type = CF_OEMTEXT;
				else
					type = CF_TEXT;
				ReleaseDC (lptw->hWndText, hdc);
				/* now get it from clipboard */
				OpenClipboard (lptw->hWndText);
				hGMem = GetClipboardData (type);
				if (hGMem)
				{
					cbuf = (BYTE *) GlobalLock (hGMem);
					while (*cbuf)
					{
						if (*cbuf != '\n')
							SendMessage (lptw->hWndText, WM_CHAR, *cbuf, 1L);
						cbuf++;
					}
					GlobalUnlock (hGMem);
				}
				CloseClipboard ();
			}
			s++;
			break;
	    case CHOOSETHEFONT:
		  TextSelectFont (lptw);
	      s++;
	      break;
	    case EOS:		/* [EOS] - End Of String - do nothing */
	    default:
	      s++;
	      break;
	    }
	  if (!flag)
	    {			/* abort */
	      d = buf;
	      s = (BYTE *) "";
	    }
	}
      else
	{
	  *d++ = *s++;
	}
    }
  *d = '\0';
/** Now we can send the result to the textwindow **/
  ScilabMenuAction (buf);
  LocalFree (buf);
}


/****************************************
 * Get a filename from system menu 
 * menu title and suffixes are read from s
 * the filename is stored in d 
 * d and s are both incremented 
 * flag == FALSE : in case of cancel 
 * XXXXX : reste un petit BUG : on ne teste pas le d'epassement ds 
 * d 
 ****************************************/

#define NEWSTRING(x) if ((x=LocalAlloc(LPTR, MAXSTR+1)) == (char *)NULL) \
{ *ierr=1;return(FALSE);}

BOOL 
SciOpenSave (HWND hWndParent, BYTE ** s, char **d, int *ierr)
{
  int i, nChar;
  OPENFILENAME ofn;
  char *szTitle;
  char *szFile;
  char *szFileTitle;
  char *szFilter;
  BOOL save, flag;
  NEWSTRING (szTitle);
  NEWSTRING (szFile);
  NEWSTRING (szFileTitle);
  NEWSTRING (szFilter);
  save = (**s == SAVE);
  (*s)++;
  for (i = 0; (**s >= 32 && **s <= 126); i++)
    {
      szTitle[i] = **s;
      (*s)++;
    }				/* get dialog box title */
  szTitle[i] = '\0';
  (*s)++;
  for (i = 0; (**s >= 32 && **s <= 126); i++)
    {
      szFile[i] = **s;
      (*s)++;
    }				/* temporary copy of filter */
  szFile[i++] = '\0';
  lstrcpy (szFilter, "Default (");
  lstrcat (szFilter, szFile);
  lstrcat (szFilter, ")");
  i = lstrlen (szFilter);
  i++;				/* move past NULL */
  lstrcpy (szFilter + i, szFile);
  i += lstrlen (szFilter + i);
  i++;				/* move past NULL */
  lstrcpy (szFilter + i, "All Files (*.*)");
  i += lstrlen (szFilter + i);
  i++;				/* move past NULL */
  lstrcpy (szFilter + i, "*.*");
  i += lstrlen (szFilter + i);
  i++;				/* move past NULL */
  szFilter[i++] = '\0';		/* add a second NULL */
  /* the Windows 3.1 implentation - MC */
  szFile[0] = '\0';
  /* clear the structrure */
  memset (&ofn, 0, sizeof (OPENFILENAME));
  ofn.lStructSize = sizeof (OPENFILENAME);
  ofn.hwndOwner = hWndParent;
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = MAXSTR;
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = MAXSTR;
  ofn.lpstrTitle = szTitle;
  ofn.lpstrInitialDir = (LPSTR) NULL;
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST
    | OFN_NOCHANGEDIR;
  flag = (save ? GetSaveFileName (&ofn) : GetOpenFileName (&ofn));
  if (flag)
    {
      nChar = lstrlen (ofn.lpstrFile);
      for (i = 0; i < nChar; i++)
	{
	  **d = ofn.lpstrFile[i];
	  (*d)++;
	}
    }
  LocalFree (szTitle);
  LocalFree (szFilter);
  LocalFree (szFile);
  LocalFree (szFileTitle);
  return (flag);
}



/******************************
 * change the menu associated to the current 
 * graphic window 
 ******************************/

static integer lab_count = 0;
static char gwin_name[100], gwin_name1[100];

void 
MenuFixCurrentWin (int ivalue)
{
  static int firstentry = 0, top = -1;
  int i;
  if (firstentry == 0)
    {
      strcpy (gwin_name, "&Graphic Window 0");
      firstentry++;
    }
  if (lab_count != ivalue)
    {
      C2F (xscion) (&i);
      if (i == 1)
	{
	  lab_count = ivalue;
	  strcpy (gwin_name1, gwin_name);
	  sprintf (gwin_name, "&Graphic Window %d", (int) ivalue);
	  C2F (chmenu) (&top, gwin_name1, gwin_name);
	}
    }
}

/****************************************
 * Actions for buttons of the graphic window
 * menu 
 ****************************************/

static void 
Countp ()
{
  MenuFixCurrentWin (lab_count + 1);
}

static void 
Countm ()
{
  MenuFixCurrentWin ((lab_count == 0) ? 0 : lab_count - 1);
}


static void 
SendCountSet ()
{
  char c;
  if ((c = GetDriver ()) == 'R' || c == 'X' || c == 'W')
    {
      C2F (dr) ("xset", "window", &lab_count, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
    };
}


static void 
SendCountRaise ()
{
  char c;
  if ((c = GetDriver ()) == 'R' || c == 'X' || c == 'W')
    {
      /* C2F(dr)("xsetdr","Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); */
      C2F (dr) ("xset", "window", &lab_count, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xselect", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
    };
}

static void 
SendCountDelete ()
{
  DeleteSGWin (lab_count);
}



/*********************************
 * File handling : for menu file 
 *********************************/

GFILE *
Gfopen (LPSTR lpszFileName, int fnOpenMode)
{
  GFILE *gfile;
  gfile = (GFILE *) LocalAlloc (LPTR, sizeof (GFILE));
  if (!gfile)
    return NULL;
  gfile->hfile = _lopen (lpszFileName, fnOpenMode);
  if (gfile->hfile == HFILE_ERROR)
    {
      LocalFree (gfile);
      return NULL;
    }
  gfile->getleft = 0;
  gfile->getnext = 0;
  return gfile;
}

void 
Gfclose (GFILE * gfile)
{
  _lclose (gfile->hfile);
  LocalFree (gfile);
  return;
}

/* returns number of characters read */

int 
Gfgets (LPSTR lp, int size, GFILE * gfile)
{
  int i;
  int ch;
  for (i = 0; i < size; i++)
    {
      if (gfile->getleft <= 0)
	{
	  if ((gfile->getleft
	       = _lread (gfile->hfile, gfile->getbuf, GBUFSIZE)) == 0)
	    break;
	  gfile->getnext = 0;
	}
      ch = *lp++ = gfile->getbuf[gfile->getnext++];
      gfile->getleft--;
      if (ch == '\r')
	{
	  i--;
	  lp--;
	}
      if (ch == '\n')
	{
	  i++;
	  break;
	}
    }
  if (i < size)
    *lp++ = '\0';
  return i;
}

/* Get a line from the menu file */
/* Return number of lines read from file including comment lines */

int 
GetLine (char *buffer, int len, GFILE * gfile)
{
  BOOL status;
  int nLine = 0;

  status = (Gfgets (buffer, len, gfile) != 0);
  nLine++;
  while (status && (buffer[0] == '\0' || buffer[0] == '\n' || buffer[0] == ';'))
    {
      /* blank line or comment - ignore */
      status = (Gfgets (buffer, len, gfile) != 0);
      nLine++;
    }
  if (lstrlen (buffer) > 0)
    buffer[lstrlen (buffer) - 1] = '\0';	/* remove trailing \n */
  if (!status)
    nLine = 0;			/* zero lines if file error */
  return nLine;
}

/* Left justify string */

void 
LeftJustify (char *d, char *s)
{
  while (*s && (*s == ' ' || *s == '\t'))
    s++;			/* skip over space */
  do
    {
      *d++ = *s;
    }
  while (*s++);
}

/* Translate string to tokenized macro */

void 
TranslateMacro (char *string)
{
  int i, len;
  LPSTR ptr;
  for (i = 0; keyword[i] != (char *) NULL; i++)
    {
      if ((ptr = strstr (string, keyword[i])) != NULL)
	{
	  len = lstrlen (keyword[i]);
	  *ptr = keyeq[i];
	  lstrcpy (ptr + 1, ptr + len);
	  i--;			/* allows for more than one occurrence of keyword */
	}
    }
}

/**************************************************
 * Load Macros, and create Menu from Menu file
 **************************************************/

#define BUGGOTOCLEAN(str) \
      wsprintf(buf,str,nLine,lpmw->szMenuName); \
      MessageBox(lptw->hWndParent,(LPSTR) buf,lptw->Title,MB_ICONEXCLAMATION);\
      goto errorcleanup;

void 
LoadMacros (LPTW lptw)
{
  GFILE *menufile;
  BYTE *macroptr;
  char *buf;
  int nMenuLevel;
  HMENU hMenu[MENUDEPTH + 1];
  LPMW lpmw;
  int nLine = 1;
  int nInc;
  int nCountMenu;
  HGLOBAL hmacro;
  int i;
  HDC hdc;
  TEXTMETRIC tm;
  RECT rect;
  int ButtonX, ButtonY;
  char *ButtonText[BUTTONMAX];

  lpmw = lptw->lpmw;

  /* mark all buffers and menu file as unused */
  buf = (char *) NULL;
  hmacro = 0;
  lpmw->macro = (BYTE **) NULL;
  lpmw->szPrompt = (char *) NULL;
  lpmw->szAnswer = (char *) NULL;
  menufile = (GFILE *) NULL;

  /* open menu file */
  if ((menufile = Gfopen (lpmw->szMenuName, OF_READ)) == (GFILE *) NULL)
    goto errorcleanup;

  /* allocate buffers */
  if ((buf = LocalAlloc (LPTR, MAXSTR)) == (char *) NULL)
    goto nomemory;
  hmacro = GlobalAlloc (GHND, (NUMMENU) * sizeof (BYTE *));
  if ((lpmw->macro = (BYTE **) GlobalLock (hmacro)) == (BYTE **) NULL)
    goto nomemory;
  if ((lpmw->szPrompt = LocalAlloc (LPTR, MAXSTR)) == (char *) NULL)
    goto nomemory;
  if ((lpmw->szAnswer = LocalAlloc (LPTR, MAXSTR)) == (char *) NULL)
    goto nomemory;

  /* Initialize macro array * */
  for (i = 0; i < NUMMENU; i++)
    {
      lpmw->macro[i] = (BYTE *) 0;
    }
  lpmw->nCountMenu = 0;		/* counts predefined menus * */
  lpmw->nButton = 0;
  lpmw->hMenu = hMenu[0] = CreateMenu ();
  nMenuLevel = 0;

  while ((nInc = GetLine (buf, MAXSTR, menufile)) != 0)
    {
      nLine += nInc;
      LeftJustify (buf, buf);
      if (buf[0] == '\0')
	{
	  /* ignore blank lines */
	}
      else if (!lstrcmpi (buf, "[Menu]"))
	{
	  /* new menu */
	  if (!(nInc = GetLine (buf, MAXSTR, menufile)))
	    {
	      nLine += nInc;
	      BUGGOTOCLEAN ("Problem on line %d of %s\n");
	    }
	  LeftJustify (buf, buf);
	  if (nMenuLevel < MENUDEPTH)
	    nMenuLevel++;
	  else
	    {
	      BUGGOTOCLEAN ("Menu is too deep at line %d of %s\n");
	    }
	  hMenu[nMenuLevel] = CreateMenu ();
	  AppendMenu (hMenu[nMenuLevel > 0 ? nMenuLevel - 1 : 0],
		      MF_STRING | MF_POPUP, (UINT) hMenu[nMenuLevel],
		      (LPCSTR) buf);
	}
      else if (!lstrcmpi (buf, "[EndMenu]"))
	{
	  if (nMenuLevel > 0)
	    nMenuLevel--;	/* back up one menu */
	}
      else if (!lstrcmpi (buf, "[Button]"))
	{
	  /* button macro */
	  if (lpmw->nButton >= BUTTONMAX)
	    {
	      BUGGOTOCLEAN ("Too many buttons at line %d of %s\n");
	    }
	  if (!(nInc = GetLine (buf, MAXSTR, menufile)))
	    {
	      nLine += nInc;
	      BUGGOTOCLEAN ("Problem on line %d of %s\n");
	    }
	  LeftJustify (buf, buf);
	  if ((macroptr = LocalAlloc (LPTR, lstrlen (buf) + 1)) != (BYTE *) NULL)
	    {
	      lstrcpy ((char *) macroptr, buf);
	    }
	  else
	    {
	      BUGGOTOCLEAN ("Out of space for menu macros at line %d of %s\n");
	    }
	  ButtonText[lpmw->nButton] = (char *) macroptr;
	  if (!(nInc = GetLine (buf, MAXSTR, menufile)))
	    {
	      nLine += nInc;
	      BUGGOTOCLEAN ("Problem on line %d of %s\n");
	    }
	  LeftJustify (buf, buf);
	  TranslateMacro (buf);
	  if ((macroptr = LocalAlloc (LPTR, lstrlen (buf) + 1)) != (BYTE *) NULL)
	    lstrcpy ((char *) macroptr, buf);
	  else
	    {
	      BUGGOTOCLEAN ("Out of space for menu macros at line %d of %s\n");
	    }
	  nCountMenu = WGFindMenuPos (lpmw->macro);
	  lpmw->nCountMenu++;
	  lpmw->hButtonID[lpmw->nButton] = nCountMenu;
	  lpmw->macro[nCountMenu] = macroptr;
	  lpmw->nButton++;
	}
      else
	{
	  nCountMenu = WGFindMenuPos (lpmw->macro);
	  /* menu item */
	  if (nCountMenu >= NUMMENU)
	    {
	      BUGGOTOCLEAN ("Too many menu items at line %d of %s\n");
	    }
	  LeftJustify (buf, buf);
	  if (buf[0] == '-')
	    {
	      if (nMenuLevel == 0)
		AppendMenu (hMenu[0], MF_MENUBREAK, 0, (LPSTR) NULL);
	      else
		AppendMenu (hMenu[nMenuLevel], MF_SEPARATOR, 0, (LPSTR) NULL);
	    }
	  else if (buf[0] == '|')
	    {
	      AppendMenu (hMenu[nMenuLevel], MF_MENUBARBREAK, 0, (LPSTR) NULL);
	    }
	  else
	    {
	      AppendMenu (hMenu[nMenuLevel], MF_STRING, nCountMenu, (LPSTR) buf);
	      if (!(nInc = GetLine (buf, MAXSTR, menufile)))
		{
		  nLine += nInc;
		  BUGGOTOCLEAN ("Problem on line %d of %s\n");
		}
	      LeftJustify (buf, buf);
	      TranslateMacro (buf);
	      if ((macroptr = LocalAlloc (LPTR, lstrlen (buf) + 1)) != (BYTE *) NULL)
		{
		  lstrcpy ((char *) macroptr, buf);
		}
	      else
		{
		  BUGGOTOCLEAN ("Out of space for storing menu macros\n at line %d of %s\n");
		}
	      lpmw->macro[nCountMenu] = macroptr;
	      lpmw->nCountMenu++;
	    }
	}
    }
  nCountMenu = WGFindMenuPos (lpmw->macro);
  if ((nCountMenu - lpmw->nButton) > 0)
    {
      /* we have a menu bar so put it on the window */
      SetMenu (lptw->hWndParent, lpmw->hMenu);
      DrawMenuBar (lptw->hWndParent);
    }

  if (!lpmw->nButton)
    goto cleanup;		/* no buttons */

  /* calculate size of buttons */
  hdc = GetDC (lptw->hWndParent);
  if (lptw->hfont)
    SelectFont (hdc, lptw->hfont);
  else
    SelectFont (hdc, GetStockFont (SYSTEM_FIXED_FONT));
  GetTextMetrics (hdc, &tm);
  ButtonX = 8 * tm.tmAveCharWidth;
  ButtonY = 6 * (tm.tmHeight + tm.tmExternalLeading) / 4;
  ReleaseDC (lptw->hWndParent, hdc);

  /* move top of client text window down to allow space for buttons */
  lptw->ButtonHeight = ButtonY + 1;
  GetClientRect (lptw->hWndParent, &rect);
  SetWindowPos (lptw->hWndText, (HWND) NULL, 0, lptw->ButtonHeight,
		rect.right, rect.bottom - lptw->ButtonHeight,
		SWP_NOZORDER | SWP_NOACTIVATE);

  /* create the buttons */

  lpmw->lpfnMenuButtonProc = (WNDPROC) MyGetProcAddress ("MenuButtonProc",
							 MenuButtonProc);
  for (i = 0; i < lpmw->nButton; i++)
    {
      lpmw->hButton[i] = CreateWindow ("button", ButtonText[i],
				       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				       i * ButtonX, 0,
				       ButtonX, ButtonY,
				       lptw->hWndParent, (HMENU) i,
				       lptw->hInstance, lptw);
      lpmw->lpfnButtonProc[i] = (WNDPROC) GetWindowLong (lpmw->hButton[i], GWL_WNDPROC);
      SetWindowLong (lpmw->hButton[i], GWL_WNDPROC, (LONG) lpmw->lpfnMenuButtonProc);
    }

  goto cleanup;

nomemory:
  MessageBox (lptw->hWndParent, "Out of memory", lptw->Title, MB_ICONEXCLAMATION);

errorcleanup:
  if (hmacro)
    {
      for (i = 0; i < NUMMENU; i++)
	{
	  if (lpmw->macro[i] != (BYTE *) 0)
	    {
	      LocalFree (lpmw->macro[i]);
	      lpmw->macro[i] = (BYTE *) 0;
	    }
	}
      GlobalUnlock (hmacro);
      GlobalFree (hmacro);
      lpmw->macro = (BYTE **) NULL;
    }
  if (lpmw->szPrompt != (char *) NULL)
    {
      LocalFree (lpmw->szPrompt);
      lpmw->szPrompt = (char *) NULL;
    }
  if (lpmw->szAnswer != (char *) NULL)
    {
      LocalFree (lpmw->szAnswer);
      lpmw->szAnswer = (char *) NULL;
    }

cleanup:
  if (buf != (char *) NULL)
    LocalFree (buf);
  if (menufile != (GFILE *) NULL)
    Gfclose (menufile);
  return;
}

/************************************
 * Cleaning everything : used 
 * when exiting Scilab  XXXXXX
 ************************************/

void 
CloseMacros (LPTW lptw)
{
  int i;
  HGLOBAL hglobal;
  LPMW lpmw;
  lpmw = lptw->lpmw;
  if (lpmw->macro != (BYTE **) NULL)
    {
      hglobal = GlobalHandle (lpmw->macro);
      if (hglobal)
	{
	  for (i = 0; i < NUMMENU; i++)
	    if (lpmw->macro[i] != (BYTE *) 0)
	      {
		LocalFree (lpmw->macro[i]);
		lpmw->macro[i] = (BYTE *) 0;
	      }
	  GlobalUnlock (hglobal);
	  GlobalFree (hglobal);
	  lpmw->macro = (BYTE **) NULL;
	}
    }
  if (lpmw->szPrompt != (char *) NULL)
    {
      LocalFree (lpmw->szPrompt);
      lpmw->szPrompt = (char *) NULL;
    }
  if (lpmw->szAnswer != (char *) NULL)
    {
      LocalFree (lpmw->szAnswer);
      lpmw->szAnswer = (char *) NULL;
    }
}


/***********************************************************************
 * InputBoxDlgProc() -  Message handling routine for Input dialog box   
 ***********************************************************************/

EXPORT BOOL CALLBACK
InputBoxDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  LPTW lptw;
  LPMW lpmw;
  lptw = (LPTW) GetWindowLong (GetParent (hDlg), 0);
  lpmw = lptw->lpmw;

  switch (message)
    {
    case WM_INITDIALOG:
      SetDlgItemText (hDlg, ID_PROMPT, lpmw->szPrompt);
      return (TRUE);

    case WM_COMMAND:
      switch (LOWORD (wParam))
	{
	case ID_ANSWER:
	  return (TRUE);

	case IDOK:
	  lpmw->nChar = GetDlgItemText (hDlg, ID_ANSWER, lpmw->szAnswer, MAXSTR);
	  EndDialog (hDlg, TRUE);
	  return (TRUE);

	case IDCANCEL:
	  lpmw->szAnswer[0] = '\0';
	  EndDialog (hDlg, FALSE);
	  return (TRUE);

	default:
	  return (FALSE);
	}
    default:
      return (FALSE);
    }
}

/***********************************************************************
 * MenuButtonProc() -  Message handling routine for Menu Buttons
 ***********************************************************************/

EXPORT LRESULT CALLBACK
MenuButtonProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  LPTW lptw;
  LPMW lpmw;
  LONG n = GetWindowLong (hwnd, GWL_ID);
  lptw = (LPTW) GetWindowLong (GetParent (hwnd), 0);
  lpmw = lptw->lpmw;

  switch (message)
    {
    case WM_LBUTTONUP:
      {
	RECT rect;
	POINT pt;
	GetWindowRect (hwnd, &rect);
	GetCursorPos (&pt);
	if (PtInRect (&rect, pt))
	  SendMessage (lptw->hWndText, WM_COMMAND, lpmw->hButtonID[n], 0L);
	SetFocus (lptw->hWndText);
      }
      break;
    }
  return CallWindowProc ((lpmw->lpfnButtonProc[n]), hwnd, message, wParam, lParam);
}
