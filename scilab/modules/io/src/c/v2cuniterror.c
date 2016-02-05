/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
