/* Copyright (C) 1998 Chancelier Jean-Philippe */
/* Scilab wgmenu.c 

 *  Menus for graphic window ( from wmenu.c ) 
 *  Jean-Philippe Chancelier 
 */

#include <stdio.h>
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <string.h>		/* only use  items */

#ifdef __CYGWIN32__
#include <unistd.h>		/* for getpid */
#endif
#ifdef __MSC__
#include <process.h>		/* for getpid */
#endif

#ifdef __ABSC__
#define getpid() getpid_()
#endif

#ifdef __STDC__
#include <stdlib.h>
#else
int system ();
#endif

#include "wgnuplib.h"
#include "wresource.h"
#include "wcommon.h"
#include "../graphics/Graphics.h"
#include "../sun/men_Sutils.h"
extern TW textwin;

/* limits */
#define MAXSTR 255
#define MACROLEN 5000
/* #define NUMMENU 256  defined in wresourc.h */
#define MENUDEPTH 3
/* menu tokens */

#define CMDMIN 129
#define ZOOM 129
#define UNZOOM 130
/** Warning must be like OPEN and SAVE in the wmenu.c file */
#define LOADSCG 131
#define SAVESCG 132
#define COPYCLIP 133
#define COPYCLIP1 134
#define REDRAW 135
#define ROT3D 136
#define PRINT 137
#define CLEARWG 138
#define EOS     139
#define SCIPS   140
#define SCIPR   141
#define SCIGSEL 142
#define UPDINI  143
#define CLOSE   144
#define CMDMAX 144

static char *keyword[] =
{
  "[ZOOM]", "[UNZOOM]", "[ROT3D]", "[PRINT]", "[COPYCLIP]", "[COPYCLIP1]",
  "[REDRAW]", "[LOADSCG]", "[SAVESCG]", "[CLEARWG]", "[SCIPS]", "[SCIPR]",
  "[SCIGSEL]", "[UPDINI]", "[EOS]", "[CLOSE]",
  "{ENTER}", "{ESC}", "{TAB}",
  "{^A}", "{^B}", "{^C}", "{^D}", "{^E}", "{^F}", "{^G}", "{^H}",
  "{^I}", "{^J}", "{^K}", "{^L}", "{^M}", "{^N}", "{^O}", "{^P}",
  "{^Q}", "{^R}", "{^S}", "{^T}", "{^U}", "{^V}", "{^W}", "{^X}",
  "{^Y}", "{^Z}", "{^[}", "{^\\}", "{^]}", "{^^}", "{^_}",
  NULL};

static BYTE keyeq[] =
{
  ZOOM, UNZOOM, ROT3D, PRINT, COPYCLIP, COPYCLIP1,
  REDRAW, LOADSCG, SAVESCG, CLEARWG, SCIPS, SCIPR, SCIGSEL, UPDINI, EOS,
  CLOSE,
  13, 27, 9,
  1, 2, 3, 4, 5, 6, 7, 8,
  9, 10, 11, 12, 13, 14, 15, 16,
  17, 18, 19, 20, 21, 22, 23, 24,
  25, 26, 28, 29, 30, 31,
  0 /* NULL */ };

static void dos2win32 (char *filename, char *filename1);
static void ExploreMenu (HMENU hmen, BYTE ** macro);
static void SciDelMenu (LPMW lpmw, char *name);
static void SciChMenu (LPMW lpmw, char *name, char *new_name);
static void SciSetMenu (HMENU hmen, char *name, int num, int flag);
static void SciChMenu (LPMW lpmw, char *name, char *new_name);
static void SciDelMenu (LPMW lpmw, char *name);
static void SavePs (struct BCG *ScilabGC);
static void PrintPs (struct BCG *ScilabGC);

/*********************************
 * Send a macro to the text window 
 *********************************/

void SendGraphMacro (struct BCG *ScilabGC, UINT m)
{
  BYTE *s;
  char *d;
  char *buf;
  BOOL flag = TRUE;
  wininfo (" ");
  if ((buf = LocalAlloc (LPTR, MAXSTR + 1)) == (char *) NULL)
    return;
  if (m >= NUMMENU || m < 0)
    return;
  s = ScilabGC->lpmw.macro[m];
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
	    case ZOOM:
	      scig_2dzoom (ScilabGC->CurWindow);
	      s++;
	      break;
	    case UNZOOM:
	      scig_unzoom (ScilabGC->CurWindow);
	      s++;
	      break;
	    case ROT3D:
	      scig_3drot (ScilabGC->CurWindow);
	      s++;
	      break;
	    case PRINT:
	      CopyPrint (ScilabGC);
	      s++;
	      break;
	    case COPYCLIP:
	      NewCopyClip (ScilabGC);
	      s++;
	      break;
	    case COPYCLIP1:
	      CopyClip (ScilabGC);
	      s++;
	      break;
	    case REDRAW:
	      scig_replay (ScilabGC->CurWindow);
	      s++;
	      break;
	    case CLEARWG:
	      scig_erase (ScilabGC->CurWindow);
	      s++;
	      break;
	    case SCIPS:
	      SavePs (ScilabGC);
	      s++;
	      break;
	    case SCIPR:
	      PrintPs (ScilabGC);
	      s++;
	      break;
	    case SCIGSEL:
	      scig_sel (ScilabGC->CurWindow);
	      s++;
	      break;
	    case LOADSCG:
	      flag = SciOpenSave (ScilabGC->hWndParent, &s, &d, &ierr);
	      if (flag == 0 || ierr == 1)
		{
		  LocalFree (buf);
		  return;
		}
	      *d = '\0';
	      scig_loadsg (ScilabGC->CurWindow, buf);
	      break;
	    case SAVESCG:
	      flag = SciOpenSave (ScilabGC->hWndParent, &s, &d, &ierr);
	      if (flag == 0 || ierr == 1)
		{
		  LocalFree (buf);
		  return;
		}
	      *d = '\0';
	      C2F (xsaveplots) (&(ScilabGC->CurWindow), buf, 0L);
	      break;
	    case UPDINI:
	      SendMessage (ScilabGC->CWindow, WM_COMMAND, M_WRITEINI, 0L);
	      s++;
	      break;
	    case CLOSE:
	      C2F (deletewin) (&(ScilabGC->CurWindow));
	      s++;
	      break;
	    default:
	      s++;
	      break;
	    }
	/** all the special commands are directly performed 
	  in the previous switch : we don't want to send any 
	  other string to scilab **/
	  d = buf;
	  s = (BYTE *) "";
	}
      else
	{
	  *d++ = *s++;
	}
    }
  *d = '\0';
  /** Now the result is a string in buf 
    we can send the result to the textwindow **/
  /** we check if the string is coming from a dynamically 
    added menu **/
  ScilabMenuAction (buf);
  LocalFree (buf);
}

/******************************
 * The following function 
 * for standard buf : stores expression in a Queue via the StoreCommand 
 * special buf expression ( i.e begining with '@' )
 *    are parsed 
 ******************************/

void ScilabMenuAction (char *buf)
{
  char *d;
  if (strlen (buf) > 2 && buf[0] == '@')
    {
/** special cases : buf was produced by a dynamic menu **/
      if (buf[1] == '0')
	{
	  /* Interpreted mode : we store the action on a queue */
	  StoreCommand1 (buf + 2, 1);
	}
      else
	{
	  /* hard coded mode */
	  int rep, win, entry;
	  if ((d = strchr (buf, '_')) != NULL)
	    {
	      sscanf (d + 1, "%d(%d)", &win, &entry);
	      entry--;
	      *d = '\0';
	    }
	  else
	    {
	      win = -1;
	      if ((d = strchr (buf, '(')) != NULL)
		{
		  sscanf (d, "(%d)", &entry);
		  entry--;
		  *d = '\0';
		}
	    }
/** removing leading @1execstr( **/
	  C2F (setfbutn) (buf + 2 + 8, &rep);
	  if (rep == 0)
	    F2C (fbutn) (buf + 2, &(win), &(entry));
	}
    }
  else if (buf[0] != '\0')
    {
/** standard string that we send to scilab **/
      if (strlen (buf) == 1 && buf[0] <= 31 && buf[0] >= 1)
	StoreCommand1 (buf, 0);
      else
	StoreCommand1 (buf, 1);
    }
}

/************************************
 * Send a string to scilab interaction window
 * as if it was typed by the user 
 ************************************/

void write_scilab (char *buf)
{
  char *d;
  if (buf[0] != '\0')
    {
      d = buf;
      while (*d)
	{
	  SendMessage (textwin.hWndText, WM_CHAR, *d, 1L);
	  d++;
	}
    }
}

/************************************
 * Translate string to tokenized macro 
 ************************************/

static void TranslateMacro (char *string)
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
      wsprintf(buf,str,nLine,ScilabGC->lpgw->szMenuName); \
      MessageBox(ScilabGC->hWndParent,(LPSTR) buf,ScilabGC->lpgw->Title,MB_ICONEXCLAMATION);\
      goto errorcleanup;

void LoadGraphMacros (struct BCG *ScilabGC)
{
  GFILE *menufile;
  BYTE *macroptr;
  char *buf;
  HMENU hMenu[MENUDEPTH + 1];
  int nLine = 1, nInc, i, nMenuLevel, nCountMenu;
  HGLOBAL hmacro;
  /* mark all buffers and menu file as unused */
  buf = (char *) NULL;
  hmacro = 0;
  ScilabGC->lpmw.macro = (BYTE **) NULL;
  menufile = (GFILE *) NULL;

  /* open menu file */
  if ((menufile = Gfopen (ScilabGC->lpgw->szMenuName, OF_READ)) == (GFILE *) NULL)
    goto errorcleanup;

  /* allocate buffers */
  if ((buf = LocalAlloc (LPTR, MAXSTR)) == (char *) NULL)
    goto nomemory;
  hmacro = GlobalAlloc (GHND, (NUMMENU) * sizeof (BYTE *));
  if ((ScilabGC->lpmw.macro = (BYTE **) GlobalLock (hmacro)) == (BYTE **) NULL)
    goto nomemory;

  /* Initialize macro array * */
  for (i = 0; i < NUMMENU; i++)
    {
      ScilabGC->lpmw.macro[i] = (BYTE *) 0;
    }
  /** used to count items defined at start up in the menu bar :
    for preventing deletion **/
  ScilabGC->lpmw.nCountMenu = 0;
  ScilabGC->lpmw.nButton = 0;
  ScilabGC->lpmw.hMenu = hMenu[0] = CreateMenu ();
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
	    {
	      nMenuLevel++;
	    }
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
      else
	{
	  nCountMenu = WGFindMenuPos (ScilabGC->lpmw.macro);
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
	      ScilabGC->lpmw.nCountMenu++;
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
	      ScilabGC->lpmw.macro[nCountMenu] = macroptr;
	    }
	}
    }
  nCountMenu = WGFindMenuPos (ScilabGC->lpmw.macro);
  if ((nCountMenu - ScilabGC->lpmw.nButton) > 0)
    {
      /* we have a menu bar so put it on the window */
      SetMenu (ScilabGC->hWndParent, ScilabGC->lpmw.hMenu);
      DrawMenuBar (ScilabGC->hWndParent);
    }

  goto cleanup;

nomemory:
  MessageBox (ScilabGC->hWndParent, "Out of memory",
	      ScilabGC->lpgw->Title, MB_ICONEXCLAMATION);

errorcleanup:
  if (hmacro)
    {
      for (i = 0; i < NUMMENU; i++)
	{
	  if (ScilabGC->lpmw.macro[i] != (BYTE *) 0)
	    {
	      LocalFree (ScilabGC->lpmw.macro[i]);
	      ScilabGC->lpmw.macro[i] = (BYTE *) 0;
	    }
	}
      GlobalUnlock (hmacro);
      GlobalFree (hmacro);
      ScilabGC->lpmw.macro = (BYTE **) NULL;
    }

cleanup:
  if (buf != (char *) NULL)
    LocalFree (buf);
  if (menufile != (GFILE *) NULL)
    Gfclose (menufile);
  return;
}

/************************************
 * Find a free position for storing 
 * a new menu data
 ************************************/

int WGFindMenuPos (BYTE ** macros)
{
  int i;
  for (i = 0; i < NUMMENU; i++)
    {
      if (macros[i] == (BYTE *) 0)
	return (i);
    }
  return (NUMMENU);
}

/************************************
 * Cleaning everything : used 
 * A Rajouter quand on d'etruit une fenetre graphique XXXXX 
 ************************************/

void CloseGraphMacros (struct BCG *ScilabGC)
{
  int i;
  HGLOBAL hglobal;
  if (ScilabGC->lpmw.macro != (BYTE **) NULL)
    {
      hglobal = GlobalHandle (ScilabGC->lpmw.macro);
      if (hglobal)
	{
	  for (i = 0; i < NUMMENU; i++)
	    if (ScilabGC->lpmw.macro[i] != (BYTE *) 0)
	      {
		LocalFree (ScilabGC->lpmw.macro[i]);
		ScilabGC->lpmw.macro[i] = (BYTE *) 0;
	      }
	  GlobalUnlock (hglobal);
	  GlobalFree (hglobal);
	  ScilabGC->lpmw.macro = (BYTE **) NULL;
	}
    }
}

/***********************************
 * functions to change the menu state 
 ***********************************/

static void ExploreMenu (HMENU hmen, BYTE ** hmacro)
{
  int i, is, id, Nums = GetMenuItemCount (hmen);
  sciprint ("Nombre d'Items %d\r\n", Nums);
  for (i = 0; i < Nums; i++)
    {
      char buf[256];
      HMENU hSubMenu;
      hSubMenu = GetSubMenu (hmen, i);
      is = GetMenuString (hmen, i, buf, 256, MF_BYPOSITION);
      buf[is] = '\0';
      sciprint (" submenu %d, [%s]", i, buf);
      id = GetMenuItemID (hmen, i);
      sciprint (" ID %d\r\n", is);
      if (id >= 0 && is != 0)
	sciprint ("String %d %s\r\n", id, (char *) hmacro[id]);
      if (hSubMenu != NULL)
	{
	  ExploreMenu (hSubMenu, hmacro);
	}
    }
}

extern struct BCG *GetWindowXgcNumber (integer i);

/***********************************
 * activate or deactivate a menu (scilab interface)
 ***********************************/

int C2F (setmen) (integer * win_num, char *button_name,
		  integer * entries, integer * ptrentries,
		  integer * ne, integer * ierr)
{
  struct BCG *ScilabGC;
  if (*win_num == -1)
    {
      SciSetMenu (textwin.lpmw->hMenu, button_name, *ne, MF_ENABLED);
      DrawMenuBar (textwin.hWndParent);
    }
  else
    {
      ScilabGC = GetWindowXgcNumber (*win_num);
      if (ScilabGC != (struct BCG *) 0)
	{
	  SciSetMenu (ScilabGC->lpmw.hMenu, button_name, *ne, MF_ENABLED);
	  DrawMenuBar (ScilabGC->hWndParent);
	}
    }
  return (0);
}

int C2F (unsmen) (integer * win_num, char *button_name, integer * entries,
		  integer * ptrentries, integer * ne, integer * ierr)
{
  struct BCG *ScilabGC;
  if (*win_num == -1)
    {
      SciSetMenu (textwin.lpmw->hMenu, button_name, *ne, MF_GRAYED);
      DrawMenuBar (textwin.hWndParent);
    }
  else
    {
      ScilabGC = GetWindowXgcNumber (*win_num);
      if (ScilabGC != (struct BCG *) 0)
	{
	  SciSetMenu (ScilabGC->lpmw.hMenu, button_name, *ne, MF_GRAYED);
	  DrawMenuBar (ScilabGC->hWndParent);
	}
    }
  return (0);
}

static void SciSetMenu (HMENU hmen, char *name, int num, int flag)
{
  int i, is, Nums = GetMenuItemCount (hmen);
  for (i = 0; i < Nums; i++)
    {
      char buf[256], *ptr;
      is = GetMenuString (hmen, i, buf, 256, MF_BYPOSITION);
      buf[is] = '\0';
/** we strip the & sign which can be present in the menu name **/
      if ((ptr = strstr (buf, "&")) != NULL)
	lstrcpy (ptr, ptr + 1);
      if (strcmp (buf, name) == 0)
	{
	  if (num == 0)
	    EnableMenuItem (hmen, i, flag | MF_BYPOSITION);
	  else
	    {
	      HMENU hSubMenu;
	      hSubMenu = GetSubMenu (hmen, i);
	      if (hSubMenu != NULL)
		{
		  EnableMenuItem (hSubMenu, num - 1, flag | MF_BYPOSITION);
		}
	    }
	}
    }
}

/***********************************
 * change a name in a top level menu 
 * used for keeping track of the graphic window 
 * number 
 ***********************************/

int C2F (chmenu) (integer * win_num, char *old_name, char *new_name)
{
  struct BCG *ScilabGC;
  if (*win_num == -1)
    {
      SciChMenu (textwin.lpmw, old_name, new_name);
      DrawMenuBar (textwin.hWndParent);
    }
  else
    {
      ScilabGC = GetWindowXgcNumber (*win_num);
      if (ScilabGC != (struct BCG *) 0)
	{
	  SciChMenu (&(ScilabGC->lpmw), old_name, new_name);
	  DrawMenuBar (ScilabGC->hWndParent);
	}
    }
  return (0);
}

static void SciChMenu (LPMW lpmw, char *name, char *new_name)
{
  int i, is, id, Nums = GetMenuItemCount (lpmw->hMenu);
  for (i = 0; i < Nums; i++)
    {
      char buf[256];
      is = GetMenuString (lpmw->hMenu, i, buf, 256, MF_BYPOSITION);
      buf[is] = '\0';
      if (strcmp (buf, name) == 0)
	{
	  id = GetMenuItemID (lpmw->hMenu, i);
	  ModifyMenu (lpmw->hMenu, i, MF_BYPOSITION | MF_STRING, id, new_name);
	}
    }
}

/***********************************
 * delete a menu (scilab interface)
 ***********************************/

int C2F (delbtn) (integer * win_num, char *button_name)
{
  struct BCG *ScilabGC;
  if (*win_num == -1)
    {
      SciDelMenu (textwin.lpmw, button_name);
      DrawMenuBar (textwin.hWndParent);
    }
  else
    {
      ScilabGC = GetWindowXgcNumber (*win_num);
      if (ScilabGC != (struct BCG *) 0)
	{
	  SciDelMenu (&(ScilabGC->lpmw), button_name);
	  DrawMenuBar (ScilabGC->hWndParent);
	}
    }
  return (0);
}

static void SciDelMenu (LPMW lpmw, char *name)
{
  int i, is, id, Nums = GetMenuItemCount (lpmw->hMenu);
  for (i = 0; i < Nums; i++)
    {
      char buf[256];
      is = GetMenuString (lpmw->hMenu, i, buf, 256, MF_BYPOSITION);
      buf[is] = '\0';
      if (strcmp (buf, name) == 0)
	{
	  int j, Nums1;
	  HMENU hSubMenu;
	  hSubMenu = GetSubMenu (lpmw->hMenu, i);
	  if (hSubMenu != NULL)
	    {
	      Nums1 = GetMenuItemCount (hSubMenu);
	      for (j = 0; j < Nums1; j++)
		{
		  id = GetMenuItemID (hSubMenu, j);
/** menu item without a macro : id < 0 **/
		  if (id < 0 || is == 0)
		    continue;
		  /** XXXX : we accept to delete predefined menus  
		  if ( id < lpmw->nCountMenu ) 
		    {
		      sciprint("Warning : Can't delete predefined menus\r\n");
		      continue;
		    }
		  **/
		  if (lpmw->macro[id] != (BYTE *) 0)
		    {
		      LocalFree (lpmw->macro[id]);
		      lpmw->macro[id] = (BYTE *) 0;
		    }
		}
	    }
	  else
	    {
	      id = GetMenuItemID (lpmw->hMenu, i);
/** menu item without a macro : id < 0 **/
	      if (id < 0 || is == 0)
		continue;
	      /** XXXX : we accept to delete predefined menus  
	      if ( id < lpmw->nCountMenu ) 
		{
		  sciprint("Warning : Can't delete predefined menus\r\n");
		  continue;
		}
	      **/
	      if (lpmw->macro[id] != (BYTE *) 0)
		{
		  LocalFree (lpmw->macro[id]);
		  lpmw->macro[id] = (BYTE *) 0;
		}
	    }
	  DeleteMenu (lpmw->hMenu, i, MF_BYPOSITION);
	  return;
	}
    }
}

/*********************************************************
   Add a menu in  window  number wun_num or in Main window
   win_num     : graphic window number or -1 for main scilab window
   button_name : label of button
   entries     : labels of submenus if any
   ne          : number of submenus
   typ         : Action mode
                 typ==0 : interpreted (execution of scilab instruction
                 typ!=0 : hard coded a routine is called
   fname;      : name of the action function  
   *****************************************************/

void AddMenu (integer * win_num, char *button_name, char **entries,
	      integer * ne, integer * typ, char *fname, integer * ierr)
{
  BYTE *macroptr;
  struct BCG *ScilabGC;
  HWND hwnd;
  LPMW lpmw;
  int number, nCountMenu;
  number = *win_num;
  if (*win_num == -1)
    {
      lpmw = textwin.lpmw;
      hwnd = textwin.hWndParent;
      nCountMenu = WGFindMenuPos (lpmw->macro);
      if (nCountMenu >= NUMMENU)
	{
	  sciprint ("Can't add a menu item in scilab main menu \r\n");
	  return;
	}
    }
  else
    {
      ScilabGC = GetWindowXgcNumber (*win_num);
      if (ScilabGC != (struct BCG *) 0)
	{
	  lpmw = &(ScilabGC->lpmw);
	  hwnd = ScilabGC->hWndParent;
	  nCountMenu = WGFindMenuPos (lpmw->macro);
	  if (nCountMenu >= NUMMENU)
	    {
	      sciprint ("Can't add a menu item in scilab graphic %d menu \r\n"
			,*win_num);
	      return;
	    }
	}
      else
	return;
    }
  if (*ne == 0)
    {
      AppendMenu (lpmw->hMenu, MF_STRING, nCountMenu, button_name);
      if ((macroptr = LocalAlloc (LPTR, lstrlen ("@%dexecstr(%s_%d(%d))")
				  + lstrlen (fname) + 1)) != (BYTE *) NULL)
	{
	  if (*win_num < 0)
	    sprintf ((char *) macroptr, "@%dexecstr(%s(%d))", *typ, fname, 1);
	  else
	    sprintf ((char *) macroptr, "@%dexecstr(%s_%d(%d))", *typ,
		     fname, *win_num, 1);
	}
      else
	{
	  sciprint ("Out of space for storing menu macros\n");
	  return;
	}
      lpmw->macro[nCountMenu] = macroptr;
    }
  else
    {
      int i;
      HMENU hMenu;
      hMenu = CreateMenu ();
      AppendMenu (lpmw->hMenu, MF_STRING | MF_POPUP, (UINT) hMenu, button_name);
      for (i = 0; i < *ne; i++)
	{
	  nCountMenu = WGFindMenuPos (lpmw->macro);
	  if (nCountMenu >= NUMMENU)
	    {
	      sciprint ("Can't add a menu item \r\n");
	      return;
	    }
	  AppendMenu (hMenu, MF_STRING, nCountMenu, entries[i]);
	  if ((macroptr = LocalAlloc (LPTR, lstrlen ("@%dexecstr(%s_%d(%d))")
				   + lstrlen (fname) + 1)) != (BYTE *) NULL)
	    {
	      if (*win_num < 0)
		sprintf ((char *) macroptr, "@%dexecstr(%s(%d))", *typ, fname, i + 1);
	      else
		sprintf ((char *) macroptr, "@%dexecstr(%s_%d(%d))", *typ,
			 fname, *win_num, i + 1);
	    }
	  else
	    {
	      sciprint ("Out of space for storing menu macros\n");
	      return;
	    }
	  lpmw->macro[nCountMenu] = macroptr;
	}
    }
  DrawMenuBar (hwnd);
}

/* Scilab interface for the AddMenu function 
   Add a menu in  window  number win_num or in Main window

   win_num     : graphic window number or -1 for main scilab window
   button_name : label of button
   entries     : labels of submenus if any (in scilab code)
   ptrentries  : table of pointers on each entries
   ne          : number of submenus
   typ         : Action mode
   typ==0 : interpreted (execution of scilab instruction
   typ!=0 : hard coded a routine is called
   fname;      : name of the action function  
 */

int C2F (addmen) (integer * win_num, char *button_name, integer * entries,
	     integer * ptrentries, integer * ne, integer * typ, char *fname,
		  integer * ierr)
{
  char **menu_entries;
  *ierr = 0;
  if (*ne != 0)
    {
      ScilabMStr2CM (entries, ne, ptrentries, &menu_entries, ierr);
      if (*ierr == 1)
	return (0);
    }
  AddMenu (win_num, button_name, menu_entries, ne, typ, fname, ierr);
  return (0);
}

/********************************************
 *  save to Postscript or Send to A printer 
 *  for Xwindow the associated code is in jpc_SGraph + menusX 
 ****************************************/

/*****************************
  menu for exporting to ps or xfig 
  ***************************/

typedef struct tagLS
{
  int colored;
  int land;
  int use_printer;
  int ps_type;
}
LS;

static LS ls =
{1, 0, 0, 0};

static char *Print_Formats[] =
{
  "Postscript",
  "Postscript No Preamble",
  "Postscript-Latex",
  "Xfig",
  "GIF",
};

/****************************************************
 * Event handler function for the line style window 
 * uses GetWindowLong(hwnd, 4) and SetWindowLong
 ****************************************************/

EXPORT BOOL CALLBACK
  ExportStyleDlgProc (HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
  int i;
  switch (wmsg)
    {
    case WM_INITDIALOG:
      SendDlgItemMessage (hdlg, PS_COLOR, CB_ADDSTRING, 0,
			  (LPARAM) ((LPSTR) "Black and white"));
      SendDlgItemMessage (hdlg, PS_COLOR, CB_ADDSTRING, 0,
			  (LPARAM) ((LPSTR) "Color"));
      SendDlgItemMessage (hdlg, PS_COLOR, CB_SETCURSEL,
			  ls.colored, 0L);
      SendDlgItemMessage (hdlg, PS_LAND, CB_ADDSTRING, 0,
			  (LPARAM) ((LPSTR) "Portrait"));
      SendDlgItemMessage (hdlg, PS_LAND, CB_ADDSTRING, 0,
			  (LPARAM) ((LPSTR) "Landscape"));
      SendDlgItemMessage (hdlg, PS_LAND, CB_SETCURSEL,
			  ls.land, 0L);
      if (ls.use_printer == 0)
	{
	  for (i = 0; i < 5; i++)
	    SendDlgItemMessage (hdlg, PS_TYPE, CB_ADDSTRING, 0,
				(LPARAM) ((LPSTR) Print_Formats[i]));
	  SendDlgItemMessage (hdlg, PS_TYPE, CB_SETCURSEL,
			      ls.ps_type, 0L);
	}
      return TRUE;
    case WM_COMMAND:
      switch (LOWORD (wparam))
	{
	case PS_COLOR:
	  ls.colored =
	    (UINT) SendDlgItemMessage (hdlg, PS_COLOR, CB_GETCURSEL, 0, 0L);
	  return FALSE;
	case PS_TYPE:
	  ls.ps_type =
	    (UINT) SendDlgItemMessage (hdlg, PS_TYPE, CB_GETCURSEL, 0, 0L);
	  return FALSE;
	case PS_LAND:
	  ls.land =
	    (UINT) SendDlgItemMessage (hdlg, PS_LAND, CB_GETCURSEL, 0, 0L);
	  return FALSE;
	case IDOK:
	  EndDialog (hdlg, IDOK);
	  return TRUE;
	case IDCANCEL:
	  EndDialog (hdlg, IDCANCEL);
	  return TRUE;
	}
      break;
      return FALSE;
    }
  return FALSE;
}

/****************************************************
 * Export style dialog box 
 ****************************************************/

BOOL ExportStyle (struct BCG * ScilabGC)
{
  DLGPROC lpfnExportStyleDlgProc;
  BOOL status = FALSE;
  lpfnExportStyleDlgProc = (DLGPROC) MyGetProcAddress ("ExportStyleDlgProc",
						       ExportStyleDlgProc);
  if (DialogBox (hdllInstance,
		 (ls.use_printer == 0) ? "ExportStyleDlgBox"
		 : "ExportPrintDlgBox",
		 ScilabGC->hWndParent, lpfnExportStyleDlgProc) == IDOK)
    {
      status = TRUE;
    }
  return status;
}

static char filename[MAXSTR], filename1[MAXSTR];

static void SavePs (struct BCG *ScilabGC)
{
  char *d, ori;
  BYTE *s;
  char str[] = "[SAVESCG]XScilab Postscript[EOS]*[EOS]";
  int flag, ierr = 0;
/** getting ls flags **/
  ls.use_printer = 0;
  if (ExportStyle (ScilabGC) == FALSE)
    return;
/** getting filename **/
  d = filename;
  TranslateMacro (str);
  s = str;
  flag = SciOpenSave (ScilabGC->hWndParent, &s, &d, &ierr);
  if (flag == 0 || ierr == 1)
    {
      return;
    }
  *d = '\0';
/** sciprint(" file name [%s] color=%d\r\n",filename,ls.colored); **/
  switch (ls.ps_type)
    {
    case 0:
/** postscript Epsf file **/
      SetCursor (LoadCursor (NULL, IDC_WAIT));
      wininfo ("Epsf file generation");
      dos2win32 (filename, filename1);
      scig_tops (ScilabGC->CurWindow, ls.colored, filename1, "Pos");
      ori = (ls.land == 1) ? 'l' : 'p';
      ScilabPsToEps (ori, filename1, filename);
      wininfo ("end of Epsf generation");
      SetCursor (LoadCursor (NULL, IDC_CROSS));
      break;
    case 1:
      SetCursor (LoadCursor (NULL, IDC_WAIT));
      scig_tops (ScilabGC->CurWindow, ls.colored, filename, "Pos");
      wininfo ("end of Ps file generation");
      SetCursor (LoadCursor (NULL, IDC_CROSS));
      break;
    case 2:
/** Epsf + Tex file **/
      SetCursor (LoadCursor (NULL, IDC_WAIT));
      wininfo ("Epsf and LaTeX files generation");
      dos2win32 (filename, filename1);
      scig_tops (ScilabGC->CurWindow, ls.colored, filename1, "Pos");
      ori = (ls.land == 1) ? 'l' : 'p';
      ScilabPsToTeX (ori, filename1, filename, 1.0, 1.0);
      wininfo ("end of Epsf file and LaTeX file generation");
      SetCursor (LoadCursor (NULL, IDC_CROSS));
      break;
    case 3:
      SetCursor (LoadCursor (NULL, IDC_WAIT));
      scig_tops (ScilabGC->CurWindow, ls.colored, filename, "Fig");
      wininfo ("end of Xfig file generation");
      SetCursor (LoadCursor (NULL, IDC_CROSS));
      break;
    case 4:
      SetCursor (LoadCursor (NULL, IDC_WAIT));
      scig_tops (ScilabGC->CurWindow, ls.colored, filename, "GIF");
      wininfo ("end of GIF file generation");
      SetCursor (LoadCursor (NULL, IDC_CROSS));
      break;
    }
}

static void dos2win32 (char *filename, char *filename1)
{
#ifdef CVT_PATH_BEG
  if (filename[1] == ':')
    {
      *filename1++ = '/';
      *filename1++ = '/';
      *filename1++ = *filename++;
      filename++;
    }
#endif
  while (*filename != '\0')
    {
      *filename1++ = *filename++;
      if (*(filename1 - 1) == '\\')
	*(filename1 - 1) = '/';
    }
  *filename1 = '\0';
}

static void PrintPs (struct BCG *ScilabGC)
{
  char *p1;
  char ori;
/** getting ls flags **/
  ls.use_printer = 1;
  if (ExportStyle (ScilabGC) == FALSE)
    return;
/** getting filename **/
  if ((p1 = getenv ("TMPDIR")) == (char *) 0)
    {
      sciprint ("Cannot find environment variable TMPDIR\r\n");
      return;
    }
  sprintf (filename, "%s/scilab-%d", p1, (int) ScilabGC->CurWindow);
/** sciprint(" file name [%s] color=%d\r\n",filename,ls.colored); **/
  dos2win32 (filename, filename1);
  scig_tops (ScilabGC->CurWindow, ls.colored, filename1, "Pos");
  ori = (ls.land == 1) ? 'l' : 'p';
  if (ScilabPsToEps (ori, filename1, filename) == 0)
    {
      if (gp_printfile (hdllInstance, ScilabGC->hWndParent, filename,
			(char *) 0) == FALSE)
	sciprint ("Error while printing\r\n");
    }
/** filename is destroyed when we quit scilab **/
}

/* used by command_handler in metanet */

static void scig_command_scilabgc (int number, void f (struct BCG *))
{
  struct BCG *ScilabGC = GetWindowXgcNumber (number);
  if (ScilabGC != NULL && ScilabGC->CWindow && IsWindow (ScilabGC->CWindow))
    f (ScilabGC);
}

void scig_h_winmeth_print (integer number)
{
  scig_command_scilabgc (number, CopyPrint);
}

void scig_h_copyclip (integer number)
{
  scig_command_scilabgc (number, NewCopyClip);
}

void scig_h_copyclip1 (integer number)
{
  scig_command_scilabgc (number, CopyClip);
}

void scig_print (integer number)
{
  scig_command_scilabgc (number, PrintPs);
}
void scig_export (integer number)
{
  scig_command_scilabgc (number, SavePs);
}
