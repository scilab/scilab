/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
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
    EXTERN_STRING_GW static int Unload()
    {
        return 1;
    }
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
CPP_GATEWAY_PROTOTYPE(sci_isnum);
#endif /* !__STRING_GW_HXX__ */
