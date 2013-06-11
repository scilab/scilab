
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
DYNAMIC_LINK_IMPEXP void dl_genErrorMessage(wchar_t* _pwstCallerName, int _iErr, wchar_t* _pwstLibraryName);
/*--------------------------------------------------------------------------*/
