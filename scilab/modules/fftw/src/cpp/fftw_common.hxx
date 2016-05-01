/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "fftw_utilities.h"
}

typedef int(*fftw_gen)(const char *fname, types::Double* A, types::Double** O, int isn, guru_dim_struct gdim, int iopt);

types::Function::ReturnValue fftw_common(std::wstring& name, types::typed_list &in, int _iRetCount, types::typed_list &out, fftw_gen func);
types::Function::ReturnValue common_2args(std::string& name, types::typed_list &in, int _iRetCount, types::typed_list &out, fftw_gen func, int way, int opt);
types::Function::ReturnValue common_3args(std::string& name, types::typed_list &in, int _iRetCount, types::typed_list &out, fftw_gen func, int way, int opt);
types::Function::ReturnValue common_4args(std::string& name, types::typed_list &in, int _iRetCount, types::typed_list &out, fftw_gen func, int way, int opt);

template<class T>
void getVarAsDims(T* t, int& dims, int*& pdims);
void getVarAsDims(types::InternalType* t, int& dims, int*& pdims);

