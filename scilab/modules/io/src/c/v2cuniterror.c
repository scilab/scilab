/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "localization.h"
#include "v2cuniterror.h"
#include "Scierror.h"
#include "FileExist.h"
#include "isdir.h"
/*--------------------------------------------------------------------------*/
int C2F(v2cuniterror)(int *_errorcode, char *_filename,
                      unsigned long _length_filename)
{
    switch (*_errorcode)
    {
        case 66:
        {
            Scierror(*_errorcode, _("Too many files opened!\n"));
        }
        break;

        case 240:
        {
            if (FileExist(_filename))
            {
                Scierror(*_errorcode, _("File \"%s\" already exists.\n"), _filename);
            }
            else
            {
                Scierror(*_errorcode,  _("\"%s\" directory write access denied.\n"), _filename);
            }
        }
        break;

        case 241:
        {
            if (!FileExist(_filename))
            {
                Scierror(*_errorcode, _("File \"%s\" does not exist.\n"), _filename);
            }
            else
            {
                Scierror(*_errorcode, _("File \"%s\" read access denied.\n"), _filename);
            }
        }
        break;

        default:
        {
            Scierror(*_errorcode,
                     _("unmanaged error by v2cunit.\n"));
        }
        break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
