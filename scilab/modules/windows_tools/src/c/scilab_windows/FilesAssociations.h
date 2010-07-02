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

#include "dynlib_scilab_windows.h"
#include "BOOL.h" /* BOOL */
/*--------------------------------------------------------------------------*/
/**
* check if it is a .bin or .sav
* @param[in]
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL IsABinOrSavFile(char *chainefichier);

/**
* check if it is a scicos file
* @param[in]
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL IsAScicosFile(char *chainefichier);

/**
* check if it is a SciNotes file
* @param[in]
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL IsASciNotesFile(char *chainefichier);

/**
* get command to do by file extension
* @param[in]
* @param[in]
* @param[out]
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP int CommandByFileExtension(char *fichier,int OpenCode,char *Cmd);

#endif /*  __FILEASSOCIATION_H__ */
/*--------------------------------------------------------------------------*/
