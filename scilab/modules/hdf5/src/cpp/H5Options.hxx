/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __H5OPTIONS_HXX__
#define __H5OPTIONS_HXX__

#include <algorithm>
#include <string>

#include "H5Exception.hxx"

extern "C"
{
#include "localization.h"
}

namespace org_modules_hdf5
{

class H5Options
{
    enum RowOrder
    {
        C,
        FORTRAN
    };

    static RowOrder writeStyle;
    static RowOrder readStyle;

public:

    static void setOption(const std::string op, const std::string & style)
    {
        std::string upperStyle(style);
        std::transform(style.begin(), style.end(), upperStyle.begin(), toupper);
        std::string upperOp(op);
        std::transform(op.begin(), op.end(), upperOp.begin(), toupper);
        RowOrder order;

        if (upperStyle == "C")
        {
            order = C;
        }
        else if (upperStyle == "FORTRAN")
        {
            order = FORTRAN;
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid option: must be C or FORTRAN."));
        }

        if (upperOp == "READ")
        {
            setWriteStyle(order);
        }
        else if (upperOp == "WRITE")
        {
            setReadStyle(order);
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid option: must be C or FORTRAN."));
        }
    }

    static bool isWriteFlip()
    {
        return writeStyle == FORTRAN;
    }

    static bool isReadFlip()
    {
        return readStyle == FORTRAN;
    }

private:

    static void setWriteStyle(const RowOrder order)
    {
        writeStyle = order;
    }

    static void setReadStyle(const RowOrder order)
    {
        readStyle = order;
    }
};
}

#endif // __H5OPTIONS_HXX__
