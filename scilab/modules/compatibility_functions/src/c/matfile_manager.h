/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "matio.h" /* mat_t */

#define MATFILEMANAGER_ADDFILE  0
#define MATFILEMANAGER_DELFILE  1
#define MATFILEMANAGER_VIEWFILE 2
#define MATFILEMANAGER_GETFILE  3

/**
 * Manage the set of files opened using matio
 *
 * @param action one of the upper define value
 * @param fileIndex the file index in the table of files openend
 * @param matfile a pointer to the matfile entity to close
 */
void matfile_manager(int action, int *fileIndex, mat_t **matfile);

