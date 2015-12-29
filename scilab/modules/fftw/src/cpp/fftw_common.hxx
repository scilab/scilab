/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

