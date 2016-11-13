/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
SCILAB_WINDOWS_IMPEXP int CommandByFileExtension(char *fichier, int OpenCode, char *Cmd);

#endif /*  __FILEASSOCIATION_H__ */
/*--------------------------------------------------------------------------*/
