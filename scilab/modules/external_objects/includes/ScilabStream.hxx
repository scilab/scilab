/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __SCILABSTREAM_HXX__
#define __SCILABSTREAM_HXX__

extern "C" {
#include "sciprint.h"
}

#include <sstream>

namespace org_modules_external_objects
{

class ScilabStream : virtual public std::ostream
{

    class MyStringBuf : public std::stringbuf
    {

    public:

        MyStringBuf() { }
        virtual ~MyStringBuf() { }

        virtual int sync()
        {
            int ret = std::stringbuf::sync();
            if (!ret && str().size() != 0)
            {
                sciprint("%s", str().c_str());
                str("");
            }

            return ret;
        }
    };

public:
    ScilabStream() : std::ostream(new MyStringBuf()) { }
    virtual ~ScilabStream()
    {
        flush();
    }
};
}

#endif //__SCILABSTREAM_HXX__
