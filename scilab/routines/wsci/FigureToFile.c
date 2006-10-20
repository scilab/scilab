/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "FigureToFile.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
extern void ExportBMP(struct BCG *ScilabGC,char *pszflname);
extern void ExportEMF(struct BCG *ScilabGC,char *pszflname);
extern BOOL ExportStyle (struct BCG * ScilabGC);
extern void CallTranslateMacroGraphicWindow(char *string);
/*-----------------------------------------------------------------------------------*/
extern LS ls;
/*-----------------------------------------------------------------------------------*/
static void ModifyFilenameExtension(char *Filename,char *ext);
/*-----------------------------------------------------------------------------------*/
void SavePs (struct BCG *ScilabGC)
{
  char *d, ori;
  char filename[MAXSTR],filename1[MAXSTR];
  BYTE *s;
  char str[1024];
  char formatmacro[]= "[SAVESCG]Export to %s[EOS]%s[EOS]";
  int flag, ierr = 0;
  /** getting ls flags **/
  ls.use_printer = 0;
  if (ExportStyle (ScilabGC) == FALSE) return;

  /** getting filename **/
  d = filename;

  switch (ls.ps_type)
  {
    case 0:
		{
			wsprintf(str,formatmacro,"Postscript","*.eps");
			CallTranslateMacroGraphicWindow(str);
			s = str;
			flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
			if (flag == 0 || ierr == 1) return;
			*d = '\0';

			ModifyFilenameExtension(filename,".pos");

			/** postscript Epsf file **/
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			wininfo (MSG_SCIMSG96);
			dos2win32 (filename, filename1);
			scig_tops (ScilabGC->CurWindow, ls.colored, filename1, "Pos");
			ori = (ls.land == 1) ? 'l' : 'p';
			ScilabPsToEps (ori, filename1, filename);
			wininfo (MSG_SCIMSG97);
			SetCursor (LoadCursor (NULL, IDC_CROSS));
		}
		break;
    case 1:
		{
			wsprintf(str,formatmacro,"Postscript No Preamble","*.ps");
			CallTranslateMacroGraphicWindow(str);
			s = str;
			flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
			if (flag == 0 || ierr == 1) return;
			*d = '\0';

			ModifyFilenameExtension(filename,".ps");

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			scig_tops (ScilabGC->CurWindow, ls.colored, filename, "Pos");
			wininfo (MSG_SCIMSG98);
			SetCursor (LoadCursor (NULL, IDC_CROSS));
		}
		break;

    case 2:
		{
			wsprintf(str,formatmacro,"Postscript-Latex","*.tex");

			CallTranslateMacroGraphicWindow(str);
			s = str;
			flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
			if (flag == 0 || ierr == 1) return;
			*d = '\0';

			ModifyFilenameExtension(filename,".tex");

			/** Epsf + Tex file **/
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			wininfo (MSG_SCIMSG99);
			dos2win32 (filename, filename1);
			scig_tops (ScilabGC->CurWindow, ls.colored, filename1, "Pos");
			ori = (ls.land == 1) ? 'l' : 'p';
			ScilabPsToTeX (ori, filename1, filename, 1.0, 1.0);
			wininfo (MSG_SCIMSG100);
			SetCursor (LoadCursor (NULL, IDC_CROSS));
		}
    break;
    case 3:
		{
			wsprintf(str,formatmacro,"Xfig","*.fig");
			CallTranslateMacroGraphicWindow(str);
			s = str;
			flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
			if (flag == 0 || ierr == 1) return;
			*d = '\0';

			ModifyFilenameExtension(filename,".fig");

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			scig_tops (ScilabGC->CurWindow, ls.colored, filename, "Fig");
			wininfo (MSG_SCIMSG101);
			SetCursor (LoadCursor (NULL, IDC_CROSS));
		}
		break;
    case 4:
		{
			wsprintf(str,formatmacro,"gif","*.gif");
			CallTranslateMacroGraphicWindow(str);
			s = str;
			flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
			if (flag == 0 || ierr == 1) return;
			*d = '\0';

			ModifyFilenameExtension(filename,".gif");

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			scig_tops (ScilabGC->CurWindow, ls.colored, filename, "GIF");
			wininfo (MSG_SCIMSG102);
			SetCursor (LoadCursor (NULL, IDC_CROSS));
		}
		break;
    case 5:
		{
			wsprintf(str,formatmacro,"PPM","*.ppm");
			CallTranslateMacroGraphicWindow(str);
			s = str;
			flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
			if (flag == 0 || ierr == 1) return;
			*d = '\0';

			ModifyFilenameExtension(filename,".ppm");

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			scig_tops (ScilabGC->CurWindow, ls.colored, filename, "PPM");
			wininfo (MSG_SCIMSG103);
			SetCursor (LoadCursor (NULL, IDC_CROSS));
		}
    break;
    case 6:
		{
			wsprintf(str,formatmacro,"Bmp","*.bmp");
			CallTranslateMacroGraphicWindow(str);
			s = str;
			flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
			if (flag == 0 || ierr == 1) return;
			*d = '\0';

			ModifyFilenameExtension(filename,".bmp");

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			ExportBMP(ScilabGC,filename);
			wininfo (MSG_SCIMSG104);
			SetCursor (LoadCursor (NULL, IDC_CROSS));
		}
    break;
    case 7:
		{
			wsprintf(str,formatmacro,"Enhanced Metafile","*.emf");
			CallTranslateMacroGraphicWindow(str);
			s = str;
			flag = SciOpenSave (ScilabGC->hWndParent, &s,TRUE,&d, &ierr);
			if (flag == 0 || ierr == 1) return;
			*d = '\0';

			ModifyFilenameExtension(filename,".emf");

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			ExportEMF(ScilabGC,filename);
			wininfo (MSG_SCIMSG104);
			SetCursor (LoadCursor (NULL, IDC_CROSS));
		}
    break;
  }
	
}
/*-----------------------------------------------------------------------------------*/
void dos2win32 (char *filename, char *filename1)
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
/*-----------------------------------------------------------------------------------*/
char * GetFileExtension(char* filename)
{
	char *StrReturn=NULL;
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	
	strcpy(ChaineTemp,filename);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot)
	{
		StrReturn = (char *) MALLOC ((strlen(lastdot)+2)*sizeof(char));
		strcpy(StrReturn,".");
		strcat(StrReturn,lastdot);
	}
	else
	{
		StrReturn = NULL;
	}
	return StrReturn;
}
/*-----------------------------------------------------------------------------------*/
static void ModifyFilenameExtension(char *Filename,char *ext)
{
	char *Ext=NULL;
	Ext=GetFileExtension(Filename);
	if (Ext)
	{
		if (stricmp(Ext,ext)!=0)
		{
			strcat(Filename,ext);
		}
		if (Ext) {FREE(Ext);Ext=NULL;}
	}
	else strcat(Filename,ext);
}
/*-----------------------------------------------------------------------------------*/

