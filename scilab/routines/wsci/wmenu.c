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
 *   Modified for Scilab (1997) : Jean-Philippe Chancelier 
 *   Modified for Scilab (2003) : Allan CORNET
 */

#include "wmenu.h"


#ifndef STRICT
#define STRICT
#endif


/*-----------------------------------------------------------------------------------*/
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
void SendMacro (LPTW lptw, UINT m)
{
  BYTE *s;
  char *d;
  char *buf;
  BOOL flag = TRUE;
/*  int i;*/
 
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
	    	case NEWSCILAB:
	    	{
	    		
	    		STARTUPINFO start;
			SECURITY_ATTRIBUTES sec_attrs;
			PROCESS_INFORMATION child;
	    		char ScilexName[MAX_PATH];
	    		
	    		GetModuleFileName (NULL,ScilexName, MAX_PATH);

			memset (&start, 0, sizeof (start));
			start.cb = sizeof (start);
			start.dwFlags = STARTF_USESHOWWINDOW;

			start.wShowWindow = SW_SHOWMINIMIZED;
			sec_attrs.nLength = sizeof (sec_attrs);
			sec_attrs.lpSecurityDescriptor = NULL;
			sec_attrs.bInheritHandle = FALSE;
  			
  			if (CreateProcess (ScilexName,"", &sec_attrs, NULL, FALSE, CREATE_NEW_CONSOLE,  NULL, NULL, &start, &child))
  				{
  					CloseHandle (child.hThread);
					CloseHandle (child.hProcess);
  				}
  			else MessageBox(NULL,"Couldn't Execute a new Scilab","Warning",MB_ICONWARNING);

	    	}
	    		return;
	    	break;
	    	case EXEC:
	   		{ 			
	    			char Fichier[MAX_PATH];
	    			char command[MAX_PATH];
	    			
	    			if ( OpenSaveSCIFile(lptw->hWndParent,"Exec",TRUE,"Files *.sce;*.sci\0*.sci;*.sce\0Files *.sci\0*.sci\0Files *.sce\0*.sce\0All *.*\0*.*\0",Fichier) == TRUE)
	    			{
	    				SendCTRLandAKey(CTRLU);
	    				wsprintf(command,"exec('%s');disp('exec done');\n;",Fichier);
	    				write_scilab(command);
	    				
	    				
	    			}
	   		}
	    		return;
	    	break;
	    	case GETF:
	    		{ 			
	    			char Fichier[MAX_PATH];
	    			char command[MAX_PATH];
	    			
	    			if ( OpenSaveSCIFile(lptw->hWndParent,"Getf",TRUE,"Files *.sci\0*.sci\0All *.*\0*.*\0",Fichier) == TRUE)
	    			{
	    				SendCTRLandAKey(CTRLU);
	    				wsprintf(command,"getf('%s');disp('getf done');\n;",Fichier);
	    				write_scilab(command);
	    			}
	   		}
	    		return;
	    	break;
	    	case LOAD:
	    		{
	   		
	    			char Fichier[MAX_PATH];
	    			char command[MAX_PATH];
	    			
	    			if ( OpenSaveSCIFile(lptw->hWndParent,"Load",TRUE,"Files *.sav;*.bin\0*.sav;*.bin\0Files *.sav\0*.sav\0Files *.bin\0*.bin\0All *.*\0*.*\0",Fichier) == TRUE)
	    			{
	    				SendCTRLandAKey(CTRLU);
	    	   			wsprintf(command,"load('%s');disp('file loaded');\n;",Fichier);
	    	   			write_scilab(command);
	    				
	    			}
	    			
	    		}
	    		return;
	    	break;	
	    	case SAVE:
	    		{
	    			char Fichier[MAX_PATH];
	    			char command[MAX_PATH];
	    			
	    			if (OpenSaveSCIFile(lptw->hWndParent,"Save",FALSE,"Files *.sav\0*.sav\0Files *.bin\0*.bin\0All *.*\0*.*\0",Fichier) == TRUE)
	    			{
	    				SendCTRLandAKey(CTRLU);
	    				wsprintf(command," save('%s');disp('file saved');\n;",Fichier);
	    				write_scilab(command);
	    	   			
	    			}
	    		}
	    		return;
	    	break;
	    	case CHDIR:
	    	{
	    		
			BROWSEINFO InfoBrowserDirectory;
			char chemin[MAX_PATH];
			char command[MAX_PATH]; 
			LPITEMIDLIST pidl; 

			GetCurrentDirectory(MAX_PATH,chemin);

			InfoBrowserDirectory.hwndOwner = lptw->hWndParent; 
			InfoBrowserDirectory.pidlRoot = NULL; 
			InfoBrowserDirectory.lpszTitle="Choose a Directory";
			InfoBrowserDirectory.pszDisplayName = chemin; 
			InfoBrowserDirectory.ulFlags = BIF_STATUSTEXT|BIF_RETURNONLYFSDIRS; 
			InfoBrowserDirectory.lpfn =NULL;


			pidl=SHBrowseForFolder(&InfoBrowserDirectory);
			if (pidl!=NULL)
			{
			 	SHGetPathFromIDList(pidl, chemin); 
			 	SendCTRLandAKey(CTRLU);
	    			wsprintf(command,"chdir('%s');",chemin);
	    			StoreCommand1 (command, 2);
			 	
			}

	    	}
	    		return;
	    	break;
	    	case GETCWD:
	    		SendCTRLandAKey(CTRLU);
	    		StoreCommand1 ("printf('\n\n %s\n\n-->',getcwd())", 2);
	    		
	    		return;
	    	break;
	    	case EXIT:
	    		WriteTextIni (lptw);
	    		SendCTRLandAKey(CTRLU);
   			StoreCommand1 ("quit", 2);
	    		return;
	    	break;
	    	
	    	case MCOPY:
	    		 if ( HasAZoneTextSelected(lptw) == TRUE )TextCopyClip (lptw);
	    		 else MessageBox(lptw->hWndParent,"Please Select a zone of text","Info.",MB_ICONINFORMATION);
			s++;
	    	break;
	    	case PASTE:
	    		if ( !IsEmptyClipboard(lptw) ) PasteFunction(lptw);
	    		else MessageBox(lptw->hWndParent,"Clipboard is empty","Info.",MB_ICONINFORMATION);
	    	        return;
	    	break; 
	    	case TOOLBAR:
	    		ShowButtons=!ShowButtons;
	    		ToolBarOnOff(lptw,ShowButtons);
	    		return;
	    	break;
	    	case FRENCH:
	    		LanguageCode=1;
	    		SwitchLanguage(lptw,LanguageCode);
	    		
	    		return;
	    	break;
	    	case ENGLISH:
	    		LanguageCode=0;
	    		SwitchLanguage(lptw,LanguageCode);
	    		
	    		return;
	    	break;
	    	case CHOOSETHEFONT:
	    		TextSelectFont (lptw);
	      		s++;
	    	break;
	    	case RESTART:
	    		SendCTRLandAKey(CTRLU);
	    		ClearCommandWindow(lptw,TRUE);
	    		SendCTRLandAKey(CTRLU);
	    		write_scilab("abort;\n");
	    		SendCTRLandAKey(CTRLU);
	    		write_scilab("exec('SCI/scilab.star',-1);\n");
	    		SendCTRLandAKey(CTRLU);
	    		return;
	
	    	
	    	break;
	    	case ABORT:
	    		SendCTRLandAKey(CTRLU);
	    		write_scilab("abort;\n");
	    		return;
	    	break;
	    	case PAUSE:
	    		SendCTRLandAKey(CTRLU);
	    		write_scilab("pause;\n");
	    		return;
	    	break;
	    	case RESUME:
	    		SendCTRLandAKey(CTRLU);
	    		write_scilab("resume;\n");
	    		return;
	    	break;
	    	case CONSOLE:
	      		SwitchConsole();
	      		return;
	    	break;
	    	case SET:
	    		SendCountSet ();
	      		s++;
	    	break;
	    	case RAISE:
	    		SendCountRaise ();
	      		s++;
	    	break;
	    	case DELETESCIW:
	    		SendCountDelete ();
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
	    	case XBASC:
	    		return;
	    	break;
	    	case SCIPAD:
	    	
	    		StoreCommand1 ("scipad()", 2);
	    		switch (LanguageCode)
	    		{
				case 1:
					StoreCommand1 ("LANGUAGE=\"fr\";", 2);
				break;
		
				default: case 0:
					StoreCommand1 ("LANGUAGE=\"eng\";", 2);
				break;
			}
			StoreCommand1 ("%helps=initial_help_chapters(LANGUAGE);", 2);
	    		return;
	    	break;
	    	case HELP:
	    	
	    	StoreCommand1 ("help();", 2);
		switch (LanguageCode)
		{
			case 1:
				StoreCommand1 ("LANGUAGE=\"fr\";", 2);
			break;
		
			default: case 0:
				StoreCommand1 ("LANGUAGE=\"eng\";", 2);
			break;
		}
		
		StoreCommand1 ("%helps=initial_help_chapters(LANGUAGE);", 2);
		
	    		
	    		return;
	    	break;
	    	case DEMOS:
	    		SendCTRLandAKey(CTRLU);
	    		StoreCommand1 ("exec('SCI/demos/alldems.dem');", 1);
	    		return;
	    	break;
	    	case WEB:
	    	{
	    		int error =(int)ShellExecute(NULL, "open", URL, NULL, NULL, SW_SHOWNORMAL);
	    		if (error<= 32) MessageBox(NULL,"Couldn't Open Web Browser","Warning",MB_ICONWARNING);
	    	}
	    		return;
	    	break;
	    	case BUGZILLA:
	    	{
	    		int error =(int)ShellExecute(NULL, "open", URLBUGZILLA, NULL, NULL, SW_SHOWNORMAL);
	    		if (error<= 32) MessageBox(NULL,"Couldn't Open Web Browser","Warning",MB_ICONWARNING);
	    	}
	    		return;
	    	break;
	    	case EMAIL:
	    	{
	    		char Message[MAX_PATH];
	    		int error;
	    		
	    		wsprintf(Message,"mailto:%s?Subject=%s&CC=%s&body=%s",MAILTO, SUBJECT,CCMAILTO,"TEST Message");
	    		error =(int)ShellExecute(NULL, "open",Message,NULL,NULL,SW_SHOWNORMAL);
	    		if (error<= 32) MessageBox(NULL,"Couldn't Open Email Client","Warning",MB_ICONWARNING);
		}
 	    		return;
	    	break;
	    	case NEWSGROUP:
	    	{
	    		int error =(int)ShellExecute(NULL, "open", URLNEWSGROUP, NULL, NULL, SW_SHOWNORMAL);
	    		if (error<= 32) MessageBox(NULL,"Couldn't Open Web Browser","Warning",MB_ICONWARNING);
	    	}
	    		return;
	    	break;
	    	case ABOUT:
	    		{
	    			DLGPROC lpfnAboutDlgProc;
  				lpfnAboutDlgProc = (DLGPROC) MyGetProcAddress ("AboutDlgProc", AboutDlgProc);
	       			DialogBox (hdllInstance, "AboutDlgBox", NULL,lpfnAboutDlgProc);
	    		}
	    		return;
	    	break;
	    	case M_CTRL_C:
	    		{
	    			SignalCtrC ();
	    			write_scilab("\n");
	    		}
	    		return;
	    	break;
	    	
	    	case M_CTRL_U:
	    	{
	    		 SendCTRLandAKey(CTRLU);
	    	}
	    	return;
	    	break;
	    	
		case M_CTRL_P:
	    	{
	    		 SendCTRLandAKey(CTRLP);
	    	}
	    	return;
	    	break;
		case M_CTRL_B:
	    	{
	    		 SendCTRLandAKey(CTRLB);
	    	}
	    	return;
	    	break;	
		case M_CTRL_F:
	    	{
	    		 SendCTRLandAKey(CTRLF);
	    	}
	    	return;
	    	break;	
		case M_CTRL_A:
	    	{
	    		 SendCTRLandAKey(CTRLA);
	    	}
	    	return;
	    	break;	
		case M_CTRL_E:
	    	{
	    		 SendCTRLandAKey(CTRLE);
	    	}
	    	return;
	    	break;	
		case M_CTRL_H:
	    	{
	    		 SendCTRLandAKey(CTRLH);
	    	}
	    	return;
	    	break;	
		case M_CTRL_D:
	    	{
	    		 SendCTRLandAKey(CTRLD);
	    	}
	    	return;
	    	break;	
		case M_CTRL_W:
	    	{
	    		 SendCTRLandAKey(CTRLW);
	    	}
	    	return;
	    	break;	
		case M_CTRL_K:
	    	{
	    		 SendCTRLandAKey(CTRLK);
	    	}
	    	return;
	    	break;	
		case M_CTRL_L:
	    	{
	    		 SendCTRLandAKey(CTRLL);
	    	}
	    	return;
	    	break;	
	    	case CLEARCOMMANDWINDOW:
	    	{
	    		ClearCommandWindow(lptw,TRUE);
	    	}
	    	return;
	    	break;	
		case PRINTWINDOW:
		{
			PrintCommandWindow(lptw);
		}
		return;
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
/*-----------------------------------------------------------------------------------*/
/******************************
 * change the menu associated to the current 
 * graphic window 
 ******************************/
void MenuFixCurrentWin (int ivalue)
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
/*-----------------------------------------------------------------------------------*/
/****************************************
 * Actions for buttons of the graphic window
 * menu 
 ****************************************/
static void Countp ()
{
  MenuFixCurrentWin (lab_count + 1);
}
/*-----------------------------------------------------------------------------------*/
static void Countm ()
{
  MenuFixCurrentWin ((lab_count == 0) ? 0 : lab_count - 1);
}
/*-----------------------------------------------------------------------------------*/
static void SendCountSet ()
{
  char c;
  if ((c = GetDriver ()) == 'R' || c == 'X' || c == 'W')
    {
      C2F (dr) ("xset", "window", &lab_count, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
    };
}
/*-----------------------------------------------------------------------------------*/
static void SendCountRaise ()
{
  char c;
  if ((c = GetDriver ()) == 'R' || c == 'X' || c == 'W')
    {
      /* C2F(dr)("xsetdr","Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); */
      C2F (dr) ("xset", "window", &lab_count, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xselect", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
    };
}
/*-----------------------------------------------------------------------------------*/
static void SendCountDelete ()
{
  DeleteSGWin (lab_count);
}
/*-----------------------------------------------------------------------------------*/
/*********************************
 * File handling : for menu file 
 *********************************/
GFILE * Gfopen (LPSTR lpszFileName, int fnOpenMode)
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
/*-----------------------------------------------------------------------------------*/
void Gfclose (GFILE * gfile)
{
  _lclose (gfile->hfile);
  LocalFree (gfile);
  return;
}
/*-----------------------------------------------------------------------------------*/
/* returns number of characters read */
int Gfgets (LPSTR lp, int size, GFILE * gfile)
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
/*-----------------------------------------------------------------------------------*/
/* Get a line from the menu file */
/* Return number of lines read from file including comment lines */
int GetLine (char *buffer, int len, GFILE * gfile)
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
/*-----------------------------------------------------------------------------------*/
/* Left justify string */
void LeftJustify (char *d, char *s)
{
  while (*s && (*s == ' ' || *s == '\t'))
    s++;			/* skip over space */
  do
    {
      *d++ = *s;
    }
  while (*s++);
}
/*-----------------------------------------------------------------------------------*/
/* Translate string to tokenized macro */
void TranslateMacro (char *string)
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
void LoadMacros (LPTW lptw)
{
  GFILE *menufile;
  BYTE *macroptr;
  char *buf;
  int nMenuLevel;
  HMENU hMenu[MENUDEPTH + 1+1];
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
	  if (nMenuLevel < MENUDEPTH+1)
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

  if (!lpmw->nButton)  goto cleanup;		/* no buttons */
    


  /* calculate size of buttons */
  hdc = GetDC (lptw->hWndParent);
  
  SelectFont (hdc, GetStockFont (SYSTEM_FIXED_FONT));
  GetTextMetrics (hdc, &tm);
  ButtonX = 10 * tm.tmAveCharWidth;
  //ButtonY = 6 * (tm.tmHeight + tm.tmExternalLeading) / 4;
  ButtonY = 32;
  ReleaseDC (lptw->hWndParent, hdc);

  /* move top of client text window down to allow space for buttons */
  lptw->ButtonHeight = ButtonY + 1;
  GetClientRect (lptw->hWndParent, &rect);
  SetWindowPos (lptw->hWndText, (HWND) NULL, 0, lptw->ButtonHeight,
		rect.right, rect.bottom - lptw->ButtonHeight,
		SWP_NOZORDER | SWP_NOACTIVATE);

  /* create the buttons */

  lpmw->lpfnMenuButtonProc = (WNDPROC) MyGetProcAddress ("MenuButtonProc", MenuButtonProc);
  for (i = 0; i < lpmw->nButton; i++)
    {
    	CreateButton(lptw,ButtonText, i,ButtonX, ButtonY);
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
/*-----------------------------------------------------------------------------------*/
void CreateButton(LPTW lptw, char *ButtonText[BUTTONMAX], int index,int ButtonSizeX, int ButtonSizeY)
{
	LPMW lpmw;
	lpmw = lptw->lpmw;
		lpmw->hButton[index] = CreateWindow ("button", ButtonText[index],
				       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON|BS_TEXT |BS_VCENTER|BS_CENTER ,
				       index * ButtonSizeX, 0,
				       ButtonSizeX, ButtonSizeY,
				       lptw->hWndParent, (HMENU) index,
				       lptw->hInstance, lptw);
		/* Associe une infobulle */
		CreateMyTooltip (lpmw->hButton[index], ButtonText[index]);		       
		
	
	
	
				       

      lpmw->lpfnButtonProc[index] = (WNDPROC) GetWindowLong (lpmw->hButton[index], GWL_WNDPROC);
      SetWindowLong (lpmw->hButton[index], GWL_WNDPROC, (LONG) lpmw->lpfnMenuButtonProc);
}  	
/*	
	#define ID_STATIC1 100
        LPMW lpmw;
        HBITMAP hBitmap;
        HDC hdc,hdcMemory;
        HICON hIcon;	
	lpmw = lptw->lpmw;
	
	

 
//lpmw->hButton[index] = CreateWindow("button",IDI_GRAPH,WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,index * ButtonSizeX, 0,  ButtonSizeX, ButtonSizeY,lptw->hWndParent, (HMENU) index,lptw->hInstance,  NULL);

lpmw->hButton[index]=CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|BS_ICON,index * ButtonSizeX, 0,  ButtonSizeX, ButtonSizeY,lptw->hWndParent, (HMENU) ID_STATIC1,lptw->hInstance,  lptw);
hIcon=LoadIcon(NULL,IDI_QUESTION);
if (hIcon==NULL) MessageBox(NULL,"","",MB_OK);
SendMessage(lpmw->hButton[index],(UINT)BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)hIcon);

ShowWindow(lpmw->hButton[index],1); 


				       
	lpmw->lpfnButtonProc[index] = (WNDPROC) GetWindowLong (lpmw->hButton[index], GWL_WNDPROC);
      	SetWindowLong (lpmw->hButton[index], GWL_WNDPROC, (LONG) lpmw->lpfnMenuButtonProc);			       
}	
	/*
	
	#define ID_STATIC1 100
	LPMW lpmw;
	int l=0;
	char *BufferButtonText=NULL;
	lpmw = lptw->lpmw;
	
	l=strlen(ButtonText[index])+1;
	BufferButtonText=(char*)malloc(l*sizeof(char));
	strcpy(BufferButtonText,ButtonText[index]);
	
	CharUpperBuff( BufferButtonText, lstrlen( BufferButtonText ) );    
	
	
	
	
	if  ( ( strcmp(BufferButtonText,"|SCICOS|")==0) ||
	      ( strcmp(BufferButtonText,"|METANET|")==0) )
	{
		HWND hWnd;
		
		ButtonSizeX=32;
		
		lpmw->hButton[index] =	 CreateWindow ("button", NULL,
				       WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
				       index * ButtonSizeX, 0,
				       ButtonSizeX, ButtonSizeY,
				       lptw->hWndParent, (HMENU) index,
				       lptw->hInstance, lptw);
		
		if ( strcmp(BufferButtonText,"|SCICOS|")==0 )
		{
			hWnd =  CreateWindowEx( 0, "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON,
               					        0, 0,
				       			100,100,
				       			lpmw->hButton[index],ID_STATIC1,lptw->hInstance, NULL);           
     
		SendMessage(hWnd, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(lptw->hInstance, IDI_COS));
		CreateMyTooltip (lpmw->hButton[index], "Scicos");		       
		}
		
		if ( strcmp(BufferButtonText,"|METANET|")==0 )
		{
			hWnd =  CreateWindowEx( 0, "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON,
               					        0, 0,
				       			100,100,
				       			lpmw->hButton[index],ID_STATIC1,lptw->hInstance, NULL);           
     		
		SendMessage(hWnd, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(lptw->hInstance, IDI_GRAPH));
		CreateMyTooltip (lpmw->hButton[index], "Metanet");		       
		}
		
		UpdateWindow(hWnd);
	
		
	}
	else
	{
		lpmw->hButton[index] = CreateWindow ("button", ButtonText[index],
				       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON|BS_TEXT |BS_VCENTER|BS_CENTER ,
				       index * ButtonSizeX, 0,
				       ButtonSizeX, ButtonSizeY,
				       lptw->hWndParent, (HMENU) index,
				       lptw->hInstance, lptw);
		/* Associe une infobulle */
	/*	CreateMyTooltip (lpmw->hButton[index], ButtonText[index]);		       
		
	}
	
	/*	
				       

      lpmw->lpfnButtonProc[index] = (WNDPROC) GetWindowLong (lpmw->hButton[index], GWL_WNDPROC);
      SetWindowLong (lpmw->hButton[index], GWL_WNDPROC, (LONG) lpmw->lpfnMenuButtonProc);
      
      
      
      free(BufferButtonText);
      
      */
      

/*-----------------------------------------------------------------------------------*/
/***********************************************************************
 * MenuButtonProc() -  Message handling routine for Menu Buttons
 ***********************************************************************/
 /* Modifications Allan CORNET ToolBar */
EXPORT LRESULT CALLBACK MenuButtonProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	if (PtInRect (&rect, pt)) SendMessage (lptw->hWndText, WM_COMMAND, lpmw->hButtonID[n], 0L);
	SetFocus (lptw->hWndText);
      }
      break;
     
     break;
      
    }
  return CallWindowProc ((lpmw->lpfnButtonProc[n]), hwnd, message, wParam, lParam);
}


/*-----------------------------------------------------------------------------------*/
/************************************
 * Cleaning everything : used 
 * when exiting Scilab  XXXXXX
 ************************************/
void CloseMacros (LPTW lptw)
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
/*-----------------------------------------------------------------------------------*/
/***********************************************************************
 * InputBoxDlgProc() -  Message handling routine for Input dialog box   
 ***********************************************************************/
EXPORT BOOL CALLBACK InputBoxDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
/*-----------------------------------------------------------------------------------*/

BOOL OpenSaveSCIFile(HWND hWndParent,char *titre,BOOL read,char *FileExt,char *file)
{
	BOOL Retour=FALSE;
	char szBuffer[MAX_PATH] = "";
	OPENFILENAME OFN;
		
        memset( &OFN, 0, sizeof(OFN) );
        OFN.lStructSize = sizeof(OFN);
  
        OFN.hwndOwner = hWndParent;
        OFN.hInstance = NULL;
        OFN.lpstrFilter = FileExt;
        OFN.nFilterIndex=1;
        OFN.lpstrFile = NULL;/*FILE NAME*/
        OFN.nMaxFile = 0;/*SIZE OF FILE NAME*/
        OFN.lpstrFile = szBuffer;
        OFN.nMaxFile = MAX_PATH;
        OFN.lpstrTitle = titre;
        
 
        if (read == TRUE)
	 {
	 	/* Lecture */
	 	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	  if( GetOpenFileName( &OFN ) )
       		{
       			lstrcpy(file,OFN.lpstrFile);
       			Retour=TRUE;
		}
		else Retour=FALSE;
	 }
	else
	 {
	 	/* Ecriture */
	 	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR|OFN_HIDEREADONLY;
	  if(GetSaveFileName ( &OFN ) )
       		{
       			lstrcpy(file,OFN.lpstrFile);
       			Retour=TRUE;
		}
		else Retour=FALSE;
	 }
	
	return Retour;
}
/*-----------------------------------------------------------------------------------*/
/* Double le caractere \ dans un chemin */
void DoubleDoubleSlash(char *pathout,char *pathin)
{
	int l=0;
	int i=0;
	for (i=0;i < strlen(pathin);i++)
		{

			if (pathin[i]=='\\')
			{
				pathout[l]=pathin[i];
				pathout[l+1]='\\';
				l=l+2;

			}
			else
			{
				pathout[l]=pathin[i];
				l++;
			}

		}
		pathout[l]='\0';
}
/*-----------------------------------------------------------------------------------*/
/****************************************
 * Get a filename from system menu 
 * menu title and suffixes are read from s
 * the filename is stored in d 
 * d and s are both incremented 
 * flag == FALSE : in case of cancel 
 * XXXXX : reste un petit BUG : on ne teste pas le d'epassement ds 
 * d 
 ****************************************/
BOOL SciOpenSave (HWND hWndParent, BYTE ** s, char **d, int *ierr)
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
/*-----------------------------------------------------------------------------------*/
/* Envoye le signal CTRL + le code d'une touche */
void SendCTRLandAKey(int code)
{
	char command[MAX_PATH];
	wsprintf(command,"%c",(char)code);
	write_scilab(command);
}
/*-----------------------------------------------------------------------------------*/

void PrintCommandWindow(LPTW lptw)
{
	int NombredeCaracteresAImprimer=0;
	char *CopyOfScreenBuffer=NULL;
	HDC hDC;     /* HDC imprimante */     	
	HDC hWTDC;   /* HDC Fenetre Windows */
	DOCINFO dc;  
	TEXTMETRIC tm;
	int NombredeCaracteresparLignes=0;
	POINT SizeChar;
	HFONT hfont;
	int fontSizeX;
	int fontSizeY;
	char szFaceName[40];
	LOGFONT lf;
	
	int yChar=0;
	int iCharsPerLine=0;
	int iLinesPerPage=0;
	
	TCHAR szBuffer [100] ;
	int y=0;
	
	NombredeCaracteresAImprimer=lptw->CursorPos.y * lptw->ScreenSize.x + lptw->CursorPos.x;
	CopyOfScreenBuffer=(char*)malloc( (NombredeCaracteresAImprimer+1)* sizeof(char));
	strncpy(CopyOfScreenBuffer,(LPSTR)lptw->ScreenBuffer,NombredeCaracteresAImprimer);
	
		
        hWTDC = GetDC( lptw->hWndText );
        /* recupere le nom de la fonte utilisée dans la fenetre de commande */
	GetTextFace(hWTDC, 39, szFaceName ); 
	
	hDC = GetPrinterDC();
	if( !hDC )
       	{
           MessageBox(NULL, "Error creating DC", "Error", MB_APPLMODAL | MB_OK );
           return;
       	}
	
	
	
	
       	ZeroMemory(&dc, sizeof(DOCINFO));
	dc.cbSize = sizeof(DOCINFO);
	dc.lpszDocName = "Scilab Command Window"; /* Nom du document affiché dans la queue d'impression */
	
	
	SetMapMode (hDC, MM_ANISOTROPIC) ;
	SetWindowExtEx (hDC, 1440, 1440, NULL) ;
	SetViewportExtEx (hDC, GetDeviceCaps (hDC, LOGPIXELSX),	GetDeviceCaps (hDC, LOGPIXELSY), NULL) ;
	
	hfont = EzCreateFont (hDC, TEXT ("Courier"),120, 0, 0, TRUE) ;
		
	GetObject (hfont, sizeof (LOGFONT), &lf) ;
	SelectObject (hDC, hfont) ;
	GetTextMetrics (hDC, &tm) ;
	
	
	NombredeCaracteresparLignes=tm.tmMaxCharWidth+23; /* Fonte fixe */
	yChar = tm.tmHeight + tm.tmExternalLeading ;
	
	MessageBox(NULL,"En cours de Dév.","Impression",MB_OK);
	/*
	StartDoc(hDC, &dc);
	{
		char *Ligne=NULL;
		
		int NumeroDeLigne=0;
		int numerodepage=1;
		int NombredeLignesEntetes=0;
	
		int y=0;
		
		int k=0;
		int reste=0;
		
		Ligne=(char*)malloc( (NombredeCaracteresparLignes + 1)* sizeof(char));

		
		StartPage(hDC);
		NombredeLignesEntetes=EntetePage(hDC,"Scilab Command Window",numerodepage);
		NumeroDeLigne=NombredeLignesEntetes;
		y = yChar *NumeroDeLigne;
	/*	
		for(k=0;k<(NombredeCaracteresAImprimer/NombredeCaracteresparLignes);k++)
		{
			strncpy(Ligne,&CopyOfScreenBuffer[k*(NombredeCaracteresparLignes+1)],NombredeCaracteresparLignes+1);
			TextOut(hDC,0,y,Ligne,NombredeCaracteresparLignes+1);
			NumeroDeLigne++;
			y = yChar *NumeroDeLigne;
		}
		
		reste=NombredeCaracteresAImprimer%NombredeCaracteresparLignes;
		strncpy(Ligne,&CopyOfScreenBuffer[k*(NombredeCaracteresparLignes+1)],reste);
		TextOut(hDC,0,y,Ligne,strlen(Ligne));
		NumeroDeLigne++;
		y = yChar *NumeroDeLigne;
	*/
   	/*	EndPage(hDC);
   		free(Ligne);     
   		
		numerodepage++;

	}
	EndDoc(hDC);
	*/
	free(CopyOfScreenBuffer);	
	DeleteObject (SelectObject (hDC, GetStockObject (SYSTEM_FONT))) ;
		

	
	
}

/*-----------------------------------------------------------------------------------*/
int EntetePage(HDC hdc,LPSTR Entete,int numero)
{
		
	int NombredeLignesOccupeesparEntete=0;
	int NombredeCaracteresparLignes=0;
	TEXTMETRIC tm;
	int yChar=0;
	
	char dbuffer [9];
	char tbuffer [9];
	char *ptrLine=NULL;
	char StrNumeropage[128];
	
	int j=0;
	int l=0;
	int w=0;
	
	_strdate( dbuffer );
	_strtime( tbuffer );

	GetTextMetrics (hdc, (TEXTMETRIC *) & tm);
	NombredeCaracteresparLignes=tm.tmMaxCharWidth+10; /* Fonte fixe */
	yChar = tm.tmHeight + tm.tmExternalLeading ;
	
	
	TextOut(hdc,0,NombredeLignesOccupeesparEntete*yChar,Entete,strlen(Entete));
	NombredeLignesOccupeesparEntete++;
	
	
	ptrLine=(char*)malloc( (NombredeCaracteresparLignes + 1)* sizeof(char));
	
	wsprintf(ptrLine,"%s %s",dbuffer,tbuffer);
	wsprintf(StrNumeropage,"Page %d",numero);
	l=NombredeCaracteresparLignes-strlen(ptrLine)-strlen(StrNumeropage);
	for(j=0;j<l;j++) strcat(ptrLine," ");
	strcat(ptrLine,StrNumeropage);
	
	TextOut(hdc,0,NombredeLignesOccupeesparEntete*yChar,ptrLine,NombredeCaracteresparLignes);		
	NombredeLignesOccupeesparEntete++;
	
	MoveToEx(hdc,0,NombredeLignesOccupeesparEntete*yChar,NULL);
  	LineTo(hdc,NombredeCaracteresparLignes*tm.tmWeight,NombredeLignesOccupeesparEntete*yChar);
	NombredeLignesOccupeesparEntete++;
	
	free(ptrLine);     
	
	return (NombredeLignesOccupeesparEntete);

}
/*-----------------------------------------------------------------------------------*/
HFONT EzCreateFont (HDC hdc, TCHAR * szFaceName, int iDeciPtHeight,int iDeciPtWidth, int iAttributes, BOOL fLogRes)
{
	FLOAT cxDpi, cyDpi ;
	HFONT hFont ;
	LOGFONT lf ;
	POINT pt ;
	TEXTMETRIC tm ;
	SaveDC (hdc) ;
	SetGraphicsMode (hdc, GM_ADVANCED) ;
	ModifyWorldTransform (hdc, NULL, MWT_IDENTITY) ;
	SetViewportOrgEx (hdc, 0, 0, NULL) ;
	SetWindowOrgEx (hdc, 0, 0, NULL) ;
	if (fLogRes)
	{
		cxDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSX) ;
		cyDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSY) ;
	}
	else
	{
		cxDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, HORZRES) /
		GetDeviceCaps (hdc, HORZSIZE)) ;
		cyDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, VERTRES) /
		GetDeviceCaps (hdc, VERTSIZE)) ;
	}
	pt.x = (int) (iDeciPtWidth * cxDpi / 72) ;
	pt.y = (int) (iDeciPtHeight * cyDpi / 72) ;
	DPtoLP (hdc, &pt, 1) ;

	lf.lfHeight = - (int) (fabs (pt.y) / 10.0 + 0.5) ;
	lf.lfWidth = 0 ;
	lf.lfEscapement = 0 ;
	lf.lfOrientation = 0 ;
	lf.lfWeight = iAttributes & EZ_ATTR_BOLD ? 700 : 0 ;
	lf.lfItalic = iAttributes & EZ_ATTR_ITALIC ? 1 : 0 ;
	lf.lfUnderline = iAttributes & EZ_ATTR_UNDERLINE ? 1 : 0 ;
	lf.lfStrikeOut = iAttributes & EZ_ATTR_STRIKEOUT ? 1 : 0 ;
	lf.lfCharSet = DEFAULT_CHARSET ;
	lf.lfOutPrecision = 0 ;
	lf.lfClipPrecision = 0 ;
	lf.lfQuality = 0 ;
	lf.lfPitchAndFamily = 0 ;
	lstrcpy (lf.lfFaceName, szFaceName) ;
	hFont = CreateFontIndirect (&lf) ;
	if (iDeciPtWidth != 0)
	{
		hFont = (HFONT) SelectObject (hdc, hFont) ;
		GetTextMetrics (hdc, &tm) ;
		DeleteObject (SelectObject (hdc, hFont)) ;
		lf.lfWidth = (int) (tm.tmAveCharWidth *
		fabs (pt.x) / fabs (pt.y) + 0.5) ;
		hFont = CreateFontIndirect (&lf) ;
	}
	RestoreDC (hdc, -1) ;
	return hFont ;
}
/*-----------------------------------------------------------------------------------*/
HDC GetPrinterDC(void)
   {
       PRINTDLG pdlg;

       // Initialize the PRINTDLG structure.
       memset( &pdlg, 0, sizeof( PRINTDLG ) );
       pdlg.lStructSize = sizeof( PRINTDLG );
       // Set the flag to return printer DC.
       	pdlg.hwndOwner   = NULL;
	pdlg.hDevMode    = NULL;     // Don't forget to free or store hDevMode
	pdlg.hDevNames   = NULL;     // Don't forget to free or store hDevNames
	pdlg.Flags       = PD_NOPAGENUMS|PD_NOSELECTION|PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC; 
	pdlg.nCopies     = 1;
	pdlg.nFromPage   = 0xFFFF; 
	pdlg.nToPage     = 0xFFFF; 
	pdlg.nMinPage    = 1; 
	pdlg.nMaxPage    = 0xFFFF; 

       // Invoke the printer dialog box.
       PrintDlg( &pdlg );
       // hDC member of the PRINTDLG structure contains
       // the printer DC.
       return pdlg.hDC;
   }
/*-----------------------------------------------------------------------------------*/   
void ShowToolBar(LPTW lptw)
{
	int i=0;	      		
	LPMW lpmw;      		
	RECT rect;
	lpmw = lptw->lpmw;
   	
   	GetClientRect (lptw->hWndParent, &rect);
   	SetWindowPos (lptw->hWndText, (HWND) NULL, 0, lptw->ButtonHeight,
   		      rect.right, rect.bottom - lptw->ButtonHeight,SWP_NOZORDER | SWP_NOACTIVATE);	
	for (i = 0; i < lpmw->nButton; i++)
	{
		ShowWindow( lpmw->hButton[i] , SW_SHOWNORMAL );
    	}
    	InvalidateRect(lptw->hWndParent, (LPRECT) NULL, TRUE);
    	InvalidateRect(lptw->hWndText, (LPRECT) NULL, TRUE);
    	UpdateWindow (lptw->hWndText);
}
/*-----------------------------------------------------------------------------------*/   
void HideToolBar(LPTW lptw)
{
	int i=0;	      		
	LPMW lpmw;      		
	RECT rect;
	lpmw = lptw->lpmw;
   	
   	GetClientRect (lptw->hWndParent, &rect);
   	SetWindowPos (lptw->hWndText, (HWND) NULL,0,0,
		      rect.right, rect.bottom,SWP_NOZORDER | SWP_NOACTIVATE);
	for (i = 0; i < lpmw->nButton; i++)
	{
		ShowWindow( lpmw->hButton[i] , SW_HIDE );
    	}
    	InvalidateRect(lptw->hWndParent, (LPRECT) NULL, TRUE);
    	InvalidateRect(lptw->hWndText, (LPRECT) NULL, TRUE);
    	UpdateWindow (lptw->hWndText);
    	
}
/*-----------------------------------------------------------------------------------*/   
void ToolBarOnOff(LPTW lptw,BOOL ON)
{
	if  (ON)
	{
		ShowToolBar(lptw);
	}
	else
	{
		HideToolBar(lptw);
	}
}
/*-----------------------------------------------------------------------------------*/   
void ReLoadMenus(LPTW lptw,BOOL ToolbarON)
{
	int i=0;	      		
	LPMW lpmw;      		
	RECT rect;
	lpmw = lptw->lpmw;
	
	/* Destruction des menus */
	CloseMacros (lptw);
	/* Destruction des boutons ToolBar*/
	for (i = 0; i < lpmw->nButton; i++)
	{
		DestroyWindow(lpmw->hButton[i]);
    	}
	/* Contruction Menu et Toolbar */
	UpdateFileNameMenu(lptw,LanguageCode);
	LoadMacros (lptw);
	/* Affichage Toolbar */
	ToolBarOnOff(lptw,ToolbarON);
}
/*-----------------------------------------------------------------------------------*/   
void CreateMyTooltip (HWND hwnd,char ToolTipString[30])
{
    INITCOMMONCONTROLSEX iccex; 
    HWND hwndTT;                 // handle to the ToolTip control
          
    TOOLINFO ti; // struct specifying info about tool in ToolTip control
    unsigned int uid = 0;       // for ti initialization
    
    LPTSTR lptstr = ToolTipString;
    RECT rect;                  // for client area coordinates

    /* INITIALIZE COMMON CONTROLS */
    iccex.dwICC = ICC_WIN95_CLASSES;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&iccex);

    /* CREATE A TOOLTIP WINDOW */
    hwndTT = CreateWindowEx(WS_EX_TOPMOST,
        TOOLTIPS_CLASS,
        NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        hwnd,
        NULL,
        NULL,
        NULL
        );

    SetWindowPos(hwndTT,
        HWND_TOPMOST,
        0,
        0,
        0,
        0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    /* GET COORDINATES OF THE MAIN CLIENT AREA */
    GetClientRect (hwnd, &rect);

    /* INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE */
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = hwnd;
    ti.hinst = NULL;
    ti.uId = uid;
    ti.lpszText = lptstr;
        // ToolTip control will cover the whole window
    ti.rect.left = rect.left;    
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;

    /* SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW */
    SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);	

} 
/*-----------------------------------------------------------------------------------*/   
void UpdateFileNameMenu(LPTW lptw,int LangCode)
{
	#define FILEMENUFRENCH "wscilabF.mnu"
	#define FILEMENUENGLISH "wscilabE.mnu"
	
	LPMW lpmw= lptw->lpmw;
	LPSTR szModuleName;
	LPSTR tail;
	
	HINSTANCE hInstance=(HINSTANCE) GetModuleHandle(NULL);   		
	szModuleName = (LPSTR) malloc (MAXSTR + 1);
	
	GetModuleFileName (hInstance, (LPSTR) szModuleName, MAXSTR);
	
	if ((tail = strrchr (szModuleName, '\\')) != (LPSTR) NULL)
	{
		tail++;
		*tail = '\0';
	}
	
	strcpy (lpmw->szMenuName, szModuleName);
	free(szModuleName);
	
	switch (LangCode)
	{
		case 1:
			strcat (lpmw->szMenuName, FILEMENUFRENCH);
		break;
		default : case 0:
			strcat (lpmw->szMenuName, FILEMENUENGLISH);
		break;
	}
	
}
/*-----------------------------------------------------------------------------------*/   
void SwitchLanguage(LPTW lptw,int LangCode)
{
	StoreCommand1 ("%helps=initial_help_chapters(LANGUAGE);", 2);
	switch (LangCode)
	{
		case 1:
			StoreCommand1 ("LANGUAGE=\"fr\";", 2);
		break;
		
		default: case 0:
			StoreCommand1 ("LANGUAGE=\"eng\";", 2);
		break;
	}


	ReLoadMenus(lptw,ShowButtons);
	ToolBarOnOff(lptw,ShowButtons);
	OnRightClickMenu(lptw);

}
/*-----------------------------------------------------------------------------------*/
void ResetMenu(void)
{
	extern char ScilexWindowName[MAX_PATH];
	LPTW lptw;
	lptw = (LPTW) GetWindowLong (FindWindow(NULL,ScilexWindowName), 0);
	ReLoadMenus(lptw,ShowButtons);
}   
/*-----------------------------------------------------------------------------------*/