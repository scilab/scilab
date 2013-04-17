/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __STRING_GW_HXX__
#define __STRING_GW_HXX__

#include "dynlib_string_gw.h"

#include "cpp_gateway_prototype.hxx"

class StringModule
{
private :
    StringModule() {};
    ~StringModule() {};
public :
    EXTERN_STRING_GW static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_grep);
CPP_GATEWAY_PROTOTYPE(sci_stripblanks);
CPP_GATEWAY_PROTOTYPE(sci_regexp);
CPP_GATEWAY_PROTOTYPE(sci_part);
CPP_GATEWAY_PROTOTYPE(sci_length);
CPP_GATEWAY_PROTOTYPE(sci_strindex);
CPP_GATEWAY_PROTOTYPE(sci_strsubst);
CPP_GATEWAY_PROTOTYPE(sci_ascii);
CPP_GATEWAY_PROTOTYPE(sci_strcat);
CPP_GATEWAY_PROTOTYPE(sci_string);
CPP_GATEWAY_PROTOTYPE(sci_convstr);
CPP_GATEWAY_PROTOTYPE(sci_strncpy);
CPP_GATEWAY_PROTOTYPE(sci_strchr);
CPP_GATEWAY_PROTOTYPE(sci_strrchr);
CPP_GATEWAY_PROTOTYPE(sci_strstr);
CPP_GATEWAY_PROTOTYPE(sci_strrev);
CPP_GATEWAY_PROTOTYPE(sci_strtod);
CPP_GATEWAY_PROTOTYPE(sci_tokens);
CPP_GATEWAY_PROTOTYPE(sci_strcmp);
CPP_GATEWAY_PROTOTYPE(sci_isletter);
CPP_GATEWAY_PROTOTYPE(sci_strcspn);
CPP_GATEWAY_PROTOTYPE(sci_strtok);
CPP_GATEWAY_PROTOTYPE(sci_strspn);
CPP_GATEWAY_PROTOTYPE(sci_isalphanum);
CPP_GATEWAY_PROTOTYPE(sci_isdigit);
CPP_GATEWAY_PROTOTYPE(sci_isascii);
CPP_GATEWAY_PROTOTYPE(sci_strsplit);
#endif /* !__STRING_GW_HXX__ */
