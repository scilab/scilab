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

#ifndef __JIT_INFO_HXX__
#define __JIT_INFO_HXX__

#include "data/TITypeSignatureTuple.hxx"

namespace jit
{

class JITInfo
{
    const std::string name;
    analysis::TITypeSignatureTuple inSignature;
    analysis::TITypeSignatureTuple outSignature;
    void * wrapper;

public:

    JITInfo(const std::string & _name) : name(_name), wrapper(nullptr) { }

    inline const std::string & getName() const
    {
        return name;
    }

    inline void setWrapper(void * _wrapper)
    {
        wrapper = _wrapper;
    }

    inline void * getWrapper() const
    {
        return wrapper;
    }

    inline void addToOutSignature(const bool scalar, const analysis::TIType::Type type)
    {
        outSignature.emplace(scalar, type);
    }

    inline const analysis::TITypeSignatureTuple & getOutSignature() const
    {
        return outSignature;
    }

    inline void addToInSignature(const bool scalar, const analysis::TIType::Type type)
    {
        inSignature.emplace(scalar, type);
    }

    inline const analysis::TITypeSignatureTuple & getInSignature() const
    {
        return inSignature;
    }
};

} // namespace jit

#endif // __JIT_INFO_HXX__
