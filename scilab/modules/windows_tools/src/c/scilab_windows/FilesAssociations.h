/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#ifndef __FILEASSOCIATION_H__
#define __FILEASSOCIATION_H__

#include "machine.h" /* BOOL */
/*--------------------------------------------------------------------------*/
/**
* check if filename exists (windows only)
* @param[in] a filename
* @return TRUE or FALSE
*/
BOOL IsAFile(char *chainefichier);

/**
* check if it is a .bin or .sav
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsABinOrSavFile(char *chainefichier);

/**
* check if it is a .graph or .graphb
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAGraphFile(char *chainefichier);

/**
* check if it is a .graph
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAGraphFilegraph(char *chainefichier);

/**
* check if it is a .graphb
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAGraphFilegraphb(char *chainefichier);

/**
* check if it is a scicos file
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAScicosFile(char *chainefichier);

/**
* check if it is a .cos
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAScicosFileCOS(char *chainefichier);

/**
* check if it is a .cosf
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAScicosFileCOSF(char *chainefichier);

/**
* get command to do by file extension
* @param[in]
* @param[in]
* @param[out]
* @return TRUE or FALSE
*/
int CommandByFileExtension(char *fichier,int OpenCode,char *Cmd);

/**
* convert (lower cases) extension
* @param[in]
* @return TRUE or FALSE
*/
void ExtensionFileIntoLowerCase(char *fichier);

#endif /*  __FILEASSOCIATION_H__ */
/*--------------------------------------------------------------------------*/
