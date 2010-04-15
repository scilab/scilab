/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MATIO_GW_HXX__
#define __MATIO_GW_HXX__

#include "funcmanager.hxx"
#include "context.hxx"

#ifdef _MSC_VER
#if MATIO_GW_EXPORTS
#define EXTERN_MATIO_GW __declspec (dllexport)
#else
#define EXTERN_MATIO_GW __declspec (dllimport)
#endif
#else
#define EXTERN_MATIO_GW 
#endif

class MatioModule
{
private :
  MatioModule(){};
  ~MatioModule(){};
public :
  EXTERN_MATIO_GW static bool Load();
};

Function::ReturnValue sci_matfile_open(types::typed_list &in, int* _piRetCount, types::typed_list &out);
Function::ReturnValue sci_matfile_close(types::typed_list &in, int* _piRetCount, types::typed_list &out);
Function::ReturnValue sci_matfile_listvar(types::typed_list &in, int* _piRetCount, types::typed_list &out);
Function::ReturnValue sci_matfile_varreadnext(types::typed_list &in, int* _piRetCount, types::typed_list &out);
Function::ReturnValue sci_matfile_varwrite(types::typed_list &in, int* _piRetCount, types::typed_list &out);

#endif /* __MATIO_GW_HXX__ */
