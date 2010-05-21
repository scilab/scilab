/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __TIME_GW_HXX__
#define __TIME_GW_HXX__

#include "funcmanager.hxx"
#include "context.hxx"

#ifdef _MSC_VER
   #if TIME_GW_EXPORTS
      #define EXTERN_TIME_GW __declspec (dllexport)
   #else
      #define EXTERN_TIME_GW __declspec (dllimport)
   #endif
#else
   #define EXTERN_TIME_GW 
#endif

class TimeModule
{
private :
   TimeModule() {};
   ~TimeModule() {};
public :
   EXTERN_TIME_GW static bool Load();
};

types::Function::ReturnValue sci_tic(types::typed_list &in, int _iRetCount, types::typed_list &out);
types::Function::ReturnValue sci_toc(types::typed_list &in, int _iRetCount, types::typed_list &out);


#endif /* !__TIME_GW_HXX__ */
