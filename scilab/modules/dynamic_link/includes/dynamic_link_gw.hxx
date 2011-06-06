/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "dynlib_dynamic_link_gw.h"

#ifndef __DYNAMIC_LINK_GW_HXX__
#define __DYNAMIC_LINK_GW_HXX__

class DynamicLinkModule
{
private:
    DynamicLinkModule() {};
    ~DynamicLinkModule() {};

public:
    DYNAMIC_LINK_GW_IMPEXP static bool Load();
};

#endif /* !__DYNAMIC_LINK_GW_HXX__ */
