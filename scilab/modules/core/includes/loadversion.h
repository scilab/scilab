/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) ENPC
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
BOOL getversionmodule(wchar_t* modulename,
                      int *sci_version_major,
                      int *sci_version_minor,
                      int *sci_version_maintenance,
                      wchar_t *sci_version_string,
                      int *sci_version_revision);
/*--------------------------------------------------------------------------*/
