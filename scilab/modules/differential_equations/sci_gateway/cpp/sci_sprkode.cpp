//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
//
//--------------------------------------------------------------------------

#include "differential_equations_gw.hxx"
#include "sci_sundialsode.hpp"
#include "SPRKODEManager.hxx"

types::Function::ReturnValue sci_sprkode(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    return sci_sundialsode<SPRKODEManager>(in, opt, _iRetCount, out);
}
