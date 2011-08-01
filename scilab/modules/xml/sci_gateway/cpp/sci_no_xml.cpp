/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
extern "C" {
#include "stack-c.h"
#include "gw_xml.h"
#include "Scierror.h"
#include "localization.h"
}

int gw_xml()
{
    Scierror(999, _("XML is not installed"));
    return 0;
}

int sci_xmlRead(char *fname, unsigned long fname_len)
{
    Scierror(999, _("XML is not installed"));
    PutLhsVar();
    return 0;
}

int sci_xmlXPath(char *fname, unsigned long fname_len)
{
    Scierror(999, _("XML is not installed"));
    PutLhsVar();
    return 0;
}
