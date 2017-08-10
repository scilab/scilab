/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *  Copyright (C) 2017 - ESI-Group - Cedric DELAMARRE
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __WEBTOOLS_GW_HXX__
#define __WEBTOOLS_GW_HXX__

#include "cpp_gateway_prototype.hxx"
#include "function.hxx"

extern "C"
{
#include "dynlib_webtools.h"
}

CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(sci_http_get, WEBTOOLS_IMPEXP);
CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(sci_http_post, WEBTOOLS_IMPEXP);
CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(sci_http_put, WEBTOOLS_IMPEXP);
CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(sci_http_patch, WEBTOOLS_IMPEXP);
CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(sci_http_delete, WEBTOOLS_IMPEXP);
CPP_OPT_GATEWAY_PROTOTYPE_EXPORT(sci_http_upload, WEBTOOLS_IMPEXP);

types::Function::ReturnValue sci_http_put_post(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out, const char* fname);
int checkCommonOpt(void* curl, types::optional_list &opt, const char* fname);

#endif /* !__WEBTOOLS_GW_HXX__ */
