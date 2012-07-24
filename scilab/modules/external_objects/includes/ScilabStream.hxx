/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
