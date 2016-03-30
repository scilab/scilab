/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#ifndef __SCILAB_SCANF_HXX__
#define __SCILAB_SCANF_HXX__

#include "dynlib_fileio.h"
#include "internal.hxx"

int scilab_sscanf(wchar_t* _wcsFormat, wchar_t* _wcsData, int _iIterrator, int iNiter, std::vector<types::InternalType*>* _pITOut);

#endif /* !__SCILAB_SCANF_HXX__ */
