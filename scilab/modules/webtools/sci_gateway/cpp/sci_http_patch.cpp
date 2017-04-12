/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2017 - ESI-Group - Cedric DELAMARRE
*
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

#include "webtools_gw.hxx"
#include "function.hxx"

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_http_patch(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    return sci_http_put_post(in, opt, _iRetCount, out, "http_patch");
}
