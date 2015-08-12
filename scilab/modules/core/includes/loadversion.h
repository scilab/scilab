/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) ENPC
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "BOOL.h"
/*--------------------------------------------------------------------------*/
/**
 * Retrieve the version of the module from the XML description file
 *
 * @param[in] modulename name of the module
 * @param[out] sci_version_major
 * @param[out] sci_version_minor
 * @param[out] sci_version_maintenance
 * @param[out] sci_version_string
 * @param[out] sci_version_revision
 * @return if the function has been able to load the information
 */
BOOL getversionmodule(char *modulename,
                      int *sci_version_major,
                      int *sci_version_minor,
                      int *sci_version_maintenance,
                      char *sci_version_string,
                      int *sci_version_revision);
/*--------------------------------------------------------------------------*/
