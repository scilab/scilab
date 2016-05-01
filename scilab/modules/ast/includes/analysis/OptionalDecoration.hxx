/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __OPTIONAL_DECORATION_HXX__
#define __OPTIONAL_DECORATION_HXX__

#include <iostream>

#include "call/Call.hxx"
#include "data/Clone.hxx"
#include "data/LoopDecoration.hxx"
#include "DollarInfo.hxx"

namespace analysis
{

class OptionalDecoration
{
    enum Type { NONE, CALL, LOOP, DOLLAR };

    Type ty;
    void * ptr;

public:

    OptionalDecoration() : ty(NONE), ptr(nullptr) { }
    OptionalDecoration(Call * _ptr) : ty(CALL), ptr(_ptr) { }
    OptionalDecoration(LoopDecoration * _ptr) : ty(LOOP), ptr(_ptr) { }
    OptionalDecoration(DollarInfo * _ptr) : ty(DOLLAR), ptr(_ptr) { }
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

    inline void set(LoopDecoration * _ptr)
    {
        clean();
        ty = LOOP;
        ptr = _ptr;
    }

    inline void set(DollarInfo * _ptr)
    {
        clean();
        ty = DOLLAR;
        ptr = _ptr;
    }

    friend std::wostream & operator<<(std::wostream & out, const OptionalDecoration & od)
    {
        switch (od.ty)
        {
            case CALL:
                out << *od.get<Call>();
                break;
            case LOOP:
                out << *od.get<LoopDecoration>();
                break;
            case DOLLAR:
                out << *od.get<DollarInfo>();
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
            case LOOP:
                delete get<LoopDecoration>();
                break;
            case DOLLAR:
                delete get<DollarInfo>();
                break;
            default:
                break;
        }
    }

};

} // namespace analysis

#endif // __OPTIONAL_DECORATION_HXX__
