/* Allan CORNET INRIA 2005 */

#ifndef __FILEASSOCIATION_H__
#define __FILEASSOCIATION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <Winuser.h>
#include "..\version.h"

#include "wcommon.h"

/*-----------------------------------------------------------------------------------*/
/* Teste si la chaine de caractere correspond à un fichier ou repertoire existant*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsAFile(char *chainefichier);

/* Teste si le fichier a une extension .sav ou .bin*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsABinOrSavFile(char *chainefichier);

BOOL IsAGraphFile(char *chainefichier);
BOOL IsAGraphFilegraph(char *chainefichier);
BOOL IsAGraphFilegraphb(char *chainefichier);

BOOL IsAScicosFile(char *chainefichier);
BOOL IsAScicosFileCOS(char *chainefichier);
BOOL IsAScicosFileCOSF(char *chainefichier);

int CommandByFileExtension(char *fichier,int OpenCode,char *Cmd);
void ExtensionFileIntoLowerCase(char *fichier);
LPTSTR ScilabPathFindExtension(LPCTSTR pPath);

/*-----------------------------------------------------------------------------------*/
#endif /*  __FILEASSOCIATION_H__ */