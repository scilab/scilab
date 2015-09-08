/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OPTIONAL_DECORATION_HXX__
#define __OPTIONAL_DECORATION_HXX__

#include <iostream>

#include "call/Call.hxx"
#include "data/Clone.hxx"

namespace analysis
{

class OptionalDecoration
{
    enum Type { NONE, CALL, CLONE };

    Type ty;
    void * ptr;

public:

    OptionalDecoration() : ty(NONE), ptr(nullptr) { }
    OptionalDecoration(Call * _ptr) : ty(CALL), ptr(_ptr) { }
    OptionalDecoration(Clone * _ptr) : ty(CLONE), ptr(_ptr) { }
    OptionalDecoration(OptionalDecoration && od) : ty(od.ty), ptr(od.ptr)
    {
        od.ty = NONE;
        od.ptr = nullptr;
    }

    OptionalDecoration(const OptionalDecoration & od) = delete;

    ~OptionalDecoration()
    {
        clean();
    }

    inline bool empty() const
    {
        return ty == NONE;
    }

    template<typename T>
    inline T * get() const
    {
        return static_cast<T *>(ptr);
    }

    inline void set(Call * _ptr)
    {
        clean();
        ty = CALL;
        ptr = _ptr;
    }

    inline void set(Clone * _ptr)
    {
        clean();
        ty = CLONE;
        ptr = _ptr;
    }

    friend std::wostream & operator<<(std::wostream & out, const OptionalDecoration & od)
    {
        switch (od.ty)
        {
            case CALL:
                out << *od.get<Call>();
                break;
            case CLONE:
                out << *od.get<Clone>();
                break;
            default:
                break;
        }

        return out;
    }

private:

    inline void clean()
    {
        switch (ty)
        {
            case CALL:
                delete get<Call>();
                break;
            case CLONE:
                delete get<Clone>();
                break;
            default:
                break;
        }
    }

};

} // namespace analysis


#endif // __OPTIONAL_DECORATION_HXX__
