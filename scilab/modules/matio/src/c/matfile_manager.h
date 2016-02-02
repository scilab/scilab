/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

#include "matio.h" /* mat_t */
#include "dynlib_matio.h"

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
MATIO_IMPEXP void matfile_manager(int action, int *fileIndex, mat_t **matfile);

