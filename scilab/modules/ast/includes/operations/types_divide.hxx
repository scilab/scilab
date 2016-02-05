/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __TYPES_DIVIDE_HXX__
#define __TYPES_DIVIDE_HXX__

#include "generic_operations.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

int RDivideDoubleByDouble(types::Double *_pDouble1, types::Double *_pDouble2, types::Double **_pDoubleOut);
int RDividePolyByDouble(types::Polynom* _pPoly, types::Double* _pDouble, types::Polynom** _pPolyOut);
int RDivideDoubleByPoly(types::Double* _pDouble, types::Polynom* _pPoly, types::Polynom** _pPolyOut);
int RDivideSparseByDouble(types::Sparse* _pSp, types::Double* _pDouble, types::InternalType** _pSpOut);

#endif /* !__TYPES_DIVIDE_HXX__ */
