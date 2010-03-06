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

#ifndef __OUTPUTSTREAM_GW_HXX__
#define __OUTPUTSTREAM_GW_HXX__


#include "dynlib_output_stream_gw.h"

#include "funcmanager.hxx"
#include "context.hxx"


class OutputStreamModule
{
private :
  OutputStreamModule() {};
  ~OutputStreamModule() {};

public :
  OUTPUT_STREAM_IMPEXP_GW static bool Load();
};

Function::ReturnValue sci_disp(types::typed_list &in, int _iRetCount, types::typed_list &out);

#endif /* !__OUTPUTSTREAM_GW_HXX__ */
