
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
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
/**
 * Generates an scierror with the appropriate message
 *
 * @param fname name of the scilab function
 * @param errorCode the code of error
 * @param SharedLibraryName name of the library
 */
DYNAMIC_LINK_IMPEXP void dl_genErrorMessage(const wchar_t* _pwstCallerName, int _iErr, const wchar_t* _pwstLibraryName);
/*--------------------------------------------------------------------------*/
