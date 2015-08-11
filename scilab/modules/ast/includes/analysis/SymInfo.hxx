/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SYMINFO_HXX__
#define __SYMINFO_HXX__

#include <iostream>

namespace analysis
{
struct SymInfo
{
    enum Kind { WRITE, READ, REPLACE, FOR_IT };

    bool write;
    bool read;
    bool replace;
    bool for_it;
    TIType current_type;

    SymInfo() : write(false), read(false), replace(false), for_it(false) { }
    SymInfo(Kind k) : write(k == WRITE), read(k == READ), replace(k == REPLACE), for_it(k == FOR_IT) { }
    SymInfo(Kind k1, Kind k2) : write(k1 == WRITE || k2 == WRITE),
        read(k1 == READ || k2 == READ),
        replace(k1 == REPLACE || k2 == REPLACE),
        for_it(k1 == FOR_IT || k2 == FOR_IT) { }

    inline void set(Kind k)
    {
        switch (k)
        {
            case WRITE:
                if (!write)
                {
                    write = true;
                }
                break;
            case READ:
                if (!read)
                {
                    read = true;
                }
                break;
            case REPLACE:
                if (!replace)
                {
                    replace = true;
                }
                break;
            case FOR_IT:
                if (!for_it)
                {
                    for_it = true;
                }
                break;
        }
    }

    inline void set(Kind k1, Kind k2)
    {
        set(k1);
        set(k2);
    }

    inline bool isJustRead() const
    {
        return !write && !replace && read;
    }

    friend std::wostream & operator<<(std::wostream & out, const SymInfo & si)
    {
        out << L"Symbol use { w: " << (si.write ? L'T' : L'F')
            << L", rd: " << (si.read ? L'T' : L'F')
            << L", rp: " << (si.replace ? L'T' : L'F')
            << L", fi: " << (si.for_it ? L'T' : L'F')
            << L"}, type " << si.current_type;
        return out;
    }
};

} // namespace analysis

#endif // __SYMINFO_HXX__
