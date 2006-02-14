#include "wgmenu.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
extern HINSTANCE hdllInstance;
/*-----------------------------------------------------------------------------------*/
extern BOOL IsWindowInterface();
extern void Write_Scilab_Console (char *buf);
extern void Write_Scilab_Window (char *buf);
extern void ResetMenu(void);
extern struct BCG *GetWindowXgcNumber (integer i);
extern void HideGraphToolBar(struct BCG * ScilabGC);
extern void ShowGraphToolBar(struct BCG * ScilabGC);
extern void SaveCurrentLine(BOOL RewriteLineAtPrompt);
extern void ExportBMP(struct BCG *ScilabGC,char *pszflname);
extern void ExportEMF(struct BCG *ScilabGC,char *pszflname);
extern HDC GetPrinterDC(void);
extern char GetPrinterOrientation(void);
extern void PrintPs (struct BCG *ScilabGC);
extern void SavePs (struct BCG *ScilabGC);
extern char *GetScilabDirectory(BOOL UnixStyle);
extern void Callback_PRINTSETUP(void);
extern LPTW GetTextWinScilab(void);
void ModifyEntityPickerToolbar(struct BCG * ScilabGC,BOOL Pressed);
/*-----------------------------------------------------------------------------------*/
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
  if (m >= NUMMENU || (int)m < 0)
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
	    case TOOLBARGRAPH:
	      if (ScilabGC->lpmw.LockToolBar == FALSE)
		{
		  if (ScilabGC->lpmw.ShowToolBar)	HideGraphToolBar(ScilabGC);
		  else ShowGraphToolBar(ScilabGC);
		}
	      s++;
	      break;
	    case NEWFIG:
	      SaveCurrentLine(TRUE);
	      NewFigure(ScilabGC);
	      s++;
	      break;
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
		case PRINTSETUP:
			Callback_PRINTSETUP();
			s++;
			break;
	    case PRINT:
	      CopyPrint (ScilabGC);
	      s++;
	      break;
	    case COPYCLIP:
	      {
		CopyToClipboardEMF (ScilabGC);
		s++;
	      }
	      break;
	    case COPYCLIP1:
	      CopyToClipboardBitmap (ScilabGC);
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
	      flag = SciOpenSave (ScilabGC->hWndParent, &s,FALSE,&d, &ierr);
	      if (flag == 0 || ierr == 1)
		{
		  LocalFree (buf);
		  return;
		}
	      *d = '\0';
	      scig_loadsg (ScilabGC->CurWindow, buf);
	      break;
	    case SAVESCG:
	      flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
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
/*-----------------------------------------------------------------------------------*/
/******************************
 * The following function 
 * for standard buf : stores expression in a Queue via the StoreCommand 
 * special buf expression ( i.e begining with '@' )
 *    are parsed 
 ******************************/
void ScilabMenuAction (char *buf)
{
	if (strlen (buf) > 2 && buf[0] == '@')
	{
	      /** special cases : LocalBuf was produced by a dynamic menu **/
	    if (buf[1] == '0')
		{
		  /* Interpreted mode : we store the action on a queue */
		  StoreCommand1 (buf + 2, 0);
		}
	    else
		{
			/* hard coded mode */
			int rep, win, entry;
			char *tmp=NULL;
			char *LocalBuf=(char*)MALLOC((strlen(buf)+1)*sizeof(char));
			char *FunctionName=(char*)MALLOC((strlen(buf)+1)*sizeof(char));
			wsprintf(LocalBuf,"%s",buf+strlen("@1")+strlen("execstr("));
			/*remove last char ')' */
			LocalBuf[strlen(LocalBuf)-1]='\0';

			if ( ( tmp = strchr (LocalBuf, '_') ) != NULL )
			{
				sscanf (tmp + 1, "%d(%d)", &win, &entry);
				entry--;
				strncpy(FunctionName,LocalBuf,strlen(LocalBuf)-strlen(tmp));
				*tmp = '\0';
			}
			else
			{
				win = -1;
				if ((tmp = strchr (LocalBuf, '(')) != NULL)
				{
					char *tmpbis=NULL;
					sscanf (tmp, "(%d)", &entry);
					
					if ( ( tmpbis = strchr (LocalBuf, '(') ) != NULL )
					{
						strncpy(FunctionName,LocalBuf,strlen(LocalBuf)-strlen(tmpbis));
					}

					entry--;
					*tmp = '\0';
				}
			}

			C2F (setfbutn) (FunctionName, &rep);
			if (rep == 0)
			{
				F2C (fbutn) (FunctionName, &(win), &(entry));
			}

			if (FunctionName) {FREE(FunctionName);FunctionName=NULL;}
			if (LocalBuf) {FREE(LocalBuf);LocalBuf=NULL;}
		}
	}
	else 
	{
		if (buf[0] != '\0')
		{
			/** standard string that we send to scilab **/
			if (strlen (buf) == 1 && buf[0] <= 31 && buf[0] >= 1) 
			{
				StoreCommand1 (buf, 0);
			}
			else
			{
				StoreCommand1 (buf, 1);
			}
		}
	}
}
/*-----------------------------------------------------------------------------------*/
/************************************
 * Send a string to scilab interaction window
 * as if it was typed by the user 
 ************************************/
void write_scilab (char *buf)
{
  if ( IsWindowInterface() ) Write_Scilab_Window(buf);
  else Write_Scilab_Console(buf);
  
}
/*-----------------------------------------------------------------------------------*/
void CallTranslateMacroGraphicWindow(char *string)
{
	TranslateMacro(string);
}
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
/**************************************************
 * Load Macros, and create Menu from Menu file
 **************************************************/
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
	      BUGGOTOCLEAN (MSG_WARNING5);
	    }
	  LeftJustify (buf, buf);
	  if (nMenuLevel < MENUDEPTH)
	    {
	      nMenuLevel++;
	    }
	  else
	    {
	      BUGGOTOCLEAN (MSG_WARNING6);
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
	      BUGGOTOCLEAN ( MSG_WARNING7 );
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
		  BUGGOTOCLEAN (MSG_WARNING8);
		}
	      LeftJustify (buf, buf);
	      TranslateMacro (buf);
	      if ((macroptr = LocalAlloc (LPTR, lstrlen (buf) + 1)) != (BYTE *) NULL)
		{
		  lstrcpy ((char *) macroptr, buf);
		}
	      else
		{
		  BUGGOTOCLEAN (MSG_WARNING9);
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
  MessageBox (ScilabGC->hWndParent, MSG_ERROR77,
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
/*-----------------------------------------------------------------------------------*/
/************************************
 * Find a FREE position for storing 
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/


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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
				sciprint (MSG_WARNING10);
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
	      sciprint (MSG_WARNING11,*win_num);
	      return;
	    }
		}
		else 	return;
  }

  if (*ne == 0)
  {
    AppendMenu (lpmw->hMenu, MF_STRING, nCountMenu, button_name);
    if ((macroptr = LocalAlloc (LPTR, lstrlen ("@%dexecstr(%s_%d(%d))")+ lstrlen (fname) + 1)) != (BYTE *) NULL)
		{
			if (*win_num < 0)
				if (*typ==0) 
					sprintf ((char *) macroptr, "@%dexecstr(%s(%d))", *typ, fname, 1);
				else 
					if (*typ==2) sprintf ((char *) macroptr, "@0%s(%d,%d)",fname, 1,*win_num);
					else sprintf ((char *) macroptr, "@%dexecstr(%s(%d))", *typ,fname, 1);
			else 
				if (*typ==0) sprintf ((char *) macroptr, "@0execstr(%s_%d(%d))",fname, *win_num, 1);
				else
					if (*typ==2) sprintf ((char *) macroptr, "@0%s(%d,%d)",fname, 1,*win_num);
					else sprintf ((char *) macroptr, "@%dexecstr(%s_%d(%d))", *typ,fname, *win_num, 1);
		}
    else
		{
			sciprint (MSG_WARNING12);
			return;
		}

    lpmw->macro[nCountMenu] = macroptr;
  }
  else
  {
		int i=0;
		int j=0;
    HMENU hMenu;
    hMenu = CreateMenu ();
		

    AppendMenu (lpmw->hMenu, MF_STRING | MF_POPUP, (UINT) hMenu, button_name);
    for (i = 0; i < *ne; i++)
		{
			
			nCountMenu = WGFindMenuPos (lpmw->macro);
			if (nCountMenu >= NUMMENU)
	    {
	      sciprint (MSG_WARNING13);
	      return;
	    }
			if ( strcmp("[--]",entries[i])==0 )
			{
				AppendMenu (hMenu, MF_SEPARATOR, 0, (LPSTR) NULL);
			}
			else
			{
				AppendMenu (hMenu, MF_STRING, nCountMenu, entries[i]);
				if ((macroptr = LocalAlloc (LPTR, lstrlen ("@%dexecstr(%s_%d(%d))")+ lstrlen (fname) + 1)) != (BYTE *) NULL)
				{
					if (*win_num < 0)
						if (*typ==0) 
							sprintf ((char *) macroptr, "@%dexecstr(%s(%d))", *typ, fname, j + 1);
						else
							if (*typ==2) sprintf ((char *) macroptr, "@0%s(%d)",fname, j + 1);
							else sprintf ((char *) macroptr, "@%dexecstr(%s(%d))", *typ,fname, j + 1);
					else
						if (*typ==0) sprintf ((char *) macroptr, "@0execstr(%s_%d(%d))",fname, *win_num, j + 1);
						else
							if (*typ==2) sprintf ((char *) macroptr, "@0%s(%d,%d)",fname, j + 1,*win_num);
							else sprintf ((char *) macroptr, "@%dexecstr(%s_%d(%d))", *typ,fname, *win_num, j + 1);
					j++;
				}
				else
				{
					sciprint (MSG_WARNING14);
					return;
				}
				lpmw->macro[nCountMenu] = macroptr;
			}
		}
  }
  DrawMenuBar (hwnd);
}
/*-----------------------------------------------------------------------------------*/
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
  menu_entries=NULL;
  *ierr = 0;
  if (*ne != 0)
    {
      ScilabMStr2CM (entries, ne, ptrentries, &menu_entries, ierr);
      if (*ierr == 1)
	return (0);
    }
  /* Modification Allan CORNET */
  /* Réinitialisation Menu Console Principale */
  if ( (strcmp(button_name,"RESETMENU")==0) && (*win_num==-1) )
    {
      ResetMenu();	
    }
  else AddMenu (win_num, button_name, menu_entries, ne, typ, fname, ierr);
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/* used by command_handler in metanet */
static void scig_command_scilabgc (int number, void f (struct BCG *))
{
  struct BCG *ScilabGC = GetWindowXgcNumber (number);
  if (ScilabGC != NULL && ScilabGC->CWindow && IsWindow (ScilabGC->CWindow))
    f (ScilabGC);
}
/*-----------------------------------------------------------------------------------*/
void scig_h_copyclip (integer number)
{
  scig_command_scilabgc (number, CopyToClipboardEMF);
}
/*-----------------------------------------------------------------------------------*/
void scig_h_copyclip1 (integer number)
{
  scig_command_scilabgc (number, CopyToClipboardBitmap);
}
/*-----------------------------------------------------------------------------------*/
void scig_print (integer number)
{
  scig_command_scilabgc (number, PrintPs);
}
/*-----------------------------------------------------------------------------------*/
void scig_export (integer number)
{
  scig_command_scilabgc (number, SavePs);
}
/*-----------------------------------------------------------------------------------*/
void UpdateFileGraphNameMenu(struct BCG *ScilabGC)
{
#define FILEGRAPHMENUFRENCH "wgscilabF.mnu"
#define FILEGRAPHMENUENGLISH "wgscilabE.mnu"
  extern char ScilexWindowName[MAX_PATH];
  char *ScilabDirectory=NULL;

  HWND hWndTmpScilex=FindWindow(NULL,ScilexWindowName);
  int LangCode=0; /*English*/

  if (IsWindowInterface())
    {
      if (hWndTmpScilex)
	{
	  LPTW lptw=GetTextWinScilab();
	  LangCode=lptw->lpmw->CodeLanguage;
	}
      else LangCode=0;
    }
  else LangCode=0;
  ScilabGC->lpmw.CodeLanguage=LangCode;

  ScilabDirectory=GetScilabDirectory(FALSE);
	
  if (ScilabGC->lpgw->szMenuName!=NULL) FREE(ScilabGC->lpgw->szMenuName);
  ScilabGC->lpgw->szMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) +strlen("\\bin\\")+ strlen (FILEGRAPHMENUENGLISH) + 1);
	
  switch (LangCode)
  {
    case 1:
	  wsprintf(ScilabGC->lpgw->szMenuName,"%s\\bin\\%s",ScilabDirectory, FILEGRAPHMENUFRENCH);
      break;
    default : case 0:
	  wsprintf(ScilabGC->lpgw->szMenuName,"%s\\bin\\%s",ScilabDirectory, FILEGRAPHMENUENGLISH);
      break;
  }

  if (ScilabDirectory){FREE(ScilabDirectory);ScilabDirectory=NULL;}	
}
/*-----------------------------------------------------------------------------------*/
void NewFigure(struct BCG * ScilabGC)
{
	
  char Command[MAX_PATH];
  int FreeWindow=0;

  FreeWindow=FindFreeGraphicWindow(ScilabGC);
  wsprintf(Command,"xset(\"window\",%d);",FreeWindow);
  StoreCommand1(Command,0);
	
}
	
/*-----------------------------------------------------------------------------------*/
/* Retourne un numéro valide de fenetre graphique libre */
int FindFreeGraphicWindow(struct BCG * ScilabGC)
{
  int FreeNumber=-1;
  integer iflag =0,ids,num,un=1;

  if (ScilabGC->graphicsversion == 1) /* old mode */
    {
      HWND hWndGraph=NULL;
      int Num=0;
      char NameWindow[MAX_PATH];

      wsprintf(NameWindow,"%s%d","ScilabGraphic",Num);
      while ( FindWindow(NULL,NameWindow) )
	{
	  Num++;
	  wsprintf(NameWindow,"%s%d","ScilabGraphic",Num);
	}
      FreeNumber=Num;
    }
  else
    {
      integer *tab=NULL;
      int sizetab=0;
      int i=0;

      iflag = 0; 
      C2F(getwins)(&num,&ids ,&iflag);
      sizetab=num;

      tab=(integer*)MALLOC(sizeof(integer)*sizetab);
      for(i=0;i<sizetab;i++) tab[i]=0;

      iflag = 1; 
      C2F(getwins)(&num,tab,&iflag);

      for(i=0;i<sizetab;i++)
	{
	  if(FreeNumber<tab[i]) FreeNumber=tab[i];	
	}
      FreeNumber=FreeNumber+1;
      FREE(tab);
    } 
  return FreeNumber;
}
/*-----------------------------------------------------------------------------------*/
void RefreshMenus(struct BCG * ScilabGC)
{
	//if (ScilabGC->graphicsversion!=0) 
	//{
	//	BOOL LockToolBar=ScilabGC->lpmw.LockToolBar;
	//	int nButton=ScilabGC->lpmw.nButton;
	//	BOOL StateToolBar=ScilabGC->lpmw.ShowToolBar;

	//	DestroyMenu(ScilabGC->hMenuRoot);
	//	ScilabGC->hMenuRoot=NULL;
	//	ScilabGC->IDM_Count=1;
	//	SetMenu(ScilabGC->hWndParent,NULL);

	//	UpdateFileGraphNameMenu( ScilabGC);
	//	LoadGraphMacros( ScilabGC);

	//	ScilabGC->lpmw.nButton=nButton;
	//	ScilabGC->lpmw.ShowToolBar=StateToolBar;
	//	ScilabGC->lpmw.LockToolBar=LockToolBar;

	//}
	//else
	//{
	//	BOOL LockToolBar=ScilabGC->lpmw.LockToolBar;
	//	int nButton=ScilabGC->lpmw.nButton;
	//	BOOL StateToolBar=ScilabGC->lpmw.ShowToolBar;


	//	DestroyMenu(GetMenu(ScilabGC->hWndParent));
	//	SetMenu(ScilabGC->hWndParent,NULL);
	//	CloseGraphMacros ( ScilabGC);
	//	ScilabGC->lpmw.nButton=nButton;
	//	ScilabGC->lpmw.ShowToolBar=StateToolBar;
	//	ScilabGC->lpmw.LockToolBar=LockToolBar;

	//	ScilabGC->hMenuRoot=CreateMenu();
	//	ScilabGC->IDM_Count=1;

	//	SetMenu(ScilabGC->hWndParent,ScilabGC->hMenuRoot);

	//}
	int WinNum=ScilabGC->CurWindow;
	BOOL LockToolBar=ScilabGC->lpmw.LockToolBar;
	int nButton=ScilabGC->lpmw.nButton;
	BOOL StateToolBar=ScilabGC->lpmw.ShowToolBar;

	DestroyMenu(GetMenu(ScilabGC->hWndParent));
	ScilabGC->hMenuRoot=NULL;
	ScilabGC->IDM_Count=1;
	SetMenu(ScilabGC->hWndParent,NULL);

	CloseGraphMacros (ScilabGC);
	CreateGedMenus(ScilabGC);

	ScilabGC->lpmw.nButton=nButton;
	ScilabGC->lpmw.ShowToolBar=StateToolBar;
	ScilabGC->lpmw.LockToolBar=LockToolBar;

}
/*-----------------------------------------------------------------------------------*/
void CreateGedMenus(struct BCG * ScilabGC)
{
	int WinNum=ScilabGC->CurWindow;
	if (ScilabGC->graphicsversion!=0)
	{
		integer ne=3, menutyp=2, ierr;
		char *EditMenusE[]={"&Select figure","&Redraw figure","&Erase figure"};
		char *EditMenusF[]={"&Selectionner figure","&Redessiner figure","&Effacer figure"};

		UpdateFileGraphNameMenu(ScilabGC);
		LoadGraphMacros(ScilabGC);

		switch( ScilabGC->lpmw.CodeLanguage)
		{
			case 1:
				AddMenu(&WinNum,"&Editer", EditMenusF, &ne, &menutyp, "ged", &ierr);
			break;
			default:
				AddMenu(&WinNum,"&Edit", EditMenusE, &ne, &menutyp, "ged", &ierr);
			break;
		}
	}
	else
	{
		/*
		for new menus
		ScilabXgc->hMenuRoot=CreateMenu();
		ScilabXgc->IDM_Count=1;

		SetMenu(ScilabXgc->hWndParent,ScilabXgc->hMenuRoot); 
		*/
	#ifdef WITH_TK
		integer ne=9, menutyp=2, ierr;
		char *EditMenusE[]={"&Select figure as current","&Redraw figure","&Erase figure","[--]","Figure properties","Current &axes properties","[--]",MSG_SCIMSG116,MSG_SCIMSG117};
		char *EditMenusF[]={"&Selectionner figure comme courante","&Redessiner figure","&Effacer figure","[--]","Propriétés de la &figure","Propriétés des &axes courants","[--]",MSG_SCIMSG118,MSG_SCIMSG119};
	#else
		integer ne=3, menutyp=2, ierr;
		char *EditMenusE[]={"&Select figure","&Redraw figure","&Erase figure"};
		char *EditMenusF[]={"&Selectionner figure","&Redessiner figure","&Effacer figure"};
	#endif

		UpdateFileGraphNameMenu(ScilabGC);
		LoadGraphMacros(ScilabGC);

		switch( ScilabGC->lpmw.CodeLanguage)
		{
			case 1:
				AddMenu(&WinNum,"&Editer", EditMenusF, &ne, &menutyp, "ged", &ierr);
			break;

			default:
				AddMenu(&WinNum,"&Edit", EditMenusE, &ne, &menutyp, "ged", &ierr);
			break;
		}

	}
}
/*-----------------------------------------------------------------------------------*/
BOOL SendMacroEntityPicker(struct BCG * ScilabGC,int id)
{
	BOOL bOK=FALSE;
	char command[1024];
	
	if (IsEntityPickerMenu(ScilabGC,id))
	{
		if (id == 19)
		{
			wsprintf(command,"ged(6,%d);",ScilabGC->CurWindow); /* Start */
			ModifyEntityPickerToolbar(ScilabGC,TRUE);
			StoreCommand(command);
			bOK=TRUE;
		}

		if (id == 20)
		{
			wsprintf(command,"ged(7,%d);",ScilabGC->CurWindow); /* Stop */
			ModifyEntityPickerToolbar(ScilabGC,FALSE);
			StoreCommand(command);
			bOK=TRUE;
		}
	}
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsEntityPickerMenu(struct BCG * ScilabGC,int id)
{
	BOOL bOK=FALSE;
	if (id == 19)
	{
		#define lenStringMenu 64
		char CurrentStringMenu[lenStringMenu];
		GetMenuString(ScilabGC->lpmw.hMenu,id,CurrentStringMenu,lenStringMenu-1,MF_BYCOMMAND);
		switch (ScilabGC->lpmw.CodeLanguage)
		{
			case 1:
				if (strcmp(CurrentStringMenu,MSG_SCIMSG118)==0) bOK=TRUE;
			break;

			case 0: default:
				if (strcmp(CurrentStringMenu,MSG_SCIMSG116)==0) bOK=TRUE;
			break;
		}
	}

	if (id == 20)
	{
		#define lenStringMenu 64
		char CurrentStringMenu[lenStringMenu];
		GetMenuString(ScilabGC->lpmw.hMenu,id,CurrentStringMenu,lenStringMenu-1,MF_BYCOMMAND);
		switch (ScilabGC->lpmw.CodeLanguage)
		{
			case 1:
				if (strcmp(CurrentStringMenu,MSG_SCIMSG119)==0) bOK=TRUE;
			break;

			case 0: default:
				if (strcmp(CurrentStringMenu,MSG_SCIMSG117)==0) bOK=TRUE;
			break;
		}
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
