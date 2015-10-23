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

#ifndef __JIT_SCALARS_HXX__
#define __JIT_SCALARS_HXX__

#include <complex>

#include "internal.hxx"
#include "JITVisitor.hxx"
#include "JITScalar.hxx"

namespace jit
{

template<typename T>
class JITScalT : public JITScalar
{

public:

    JITScalT(JITVisitor & jit, const std::string & name) : JITScalar(jit.getAlloca<T>(), true, name) { }

    JITScalT(JITVisitor & jit, const T value, const bool alloc, const std::string & name) : JITScalar(jit.getValue(value, alloc), alloc, name) { }

    JITScalT(JITVisitor & jit, llvm::Value * const data, const bool alloc, const std::string & name) : JITScalar(jit.getValue(data, alloc, name), alloc, name) { }

    JITScalT(llvm::Value * const data, const bool alloc = false, const std::string & name = "") : JITScalar(data, alloc, name) { }

    virtual ~JITScalT() { }

    bool isSigned() const override
    {
        return std::is_integral<T>::value && std::is_signed<T>::value;
    }
};

typedef JITScalT<double> JITScalDouble;
typedef JITScalT<int8_t> JITScalInt8;
typedef JITScalT<int16_t> JITScalInt16;
typedef JITScalT<int32_t> JITScalInt32;
typedef JITScalT<int64_t> JITScalInt64;
typedef JITScalT<uint8_t> JITScalUInt8;
typedef JITScalT<uint16_t> JITScalUInt16;
typedef JITScalT<uint32_t> JITScalUInt32;
typedef JITScalT<uint64_t> JITScalUInt64;
typedef JITScalT<int32_t> JITScalBool;
typedef JITScalT<wchar_t> JITScalString;

class JITScalComplex : public JITScalar
{

    llvm::Value * imag;

public:

    JITScalComplex() : JITScalar() { }

    JITScalComplex(JITVisitor & jit, const std::string & name) : JITScalar(jit.getAlloca<double>(), true, name + "_re"), imag(jit.getAlloca<double>(name + "_im"))
    {
    }

    JITScalComplex(JITVisitor & jit, const std::complex<double> value, const bool alloc, const std::string & name) : JITScalar(jit.getValue(value.real(), alloc), alloc, name), imag(jit.getValue(value.imag(), alloc))
    {
        if (!name.empty())
        {
            imag->setName(name);
        }
    }

    JITScalComplex(JITVisitor & jit, llvm::Value * const _real, llvm::Value * const _imag, const bool alloc, const std::string & name) : JITScalar(jit.getValue(_real, alloc, name), alloc, name), imag(jit.getValue(_imag, alloc, name))
    {
        if (!name.empty())
        {
            imag->setName(name);
        }
    }

    JITScalComplex(llvm::Value * const _real, llvm::Value * const _imag, const bool alloc = false, const std::string & name = "") : JITScalar(_real, alloc, name), imag(_imag)
    {
        if (!name.empty())
        {
            imag->setName(name);
        }
    }

    virtual ~JITScalComplex() { }

    bool isSigned() const override
    {
        return false;
    }
    bool isComplex() const override
    {
        return true;
    }

    llvm::Value * loadReal(JITVisitor & jit) override;
    llvm::Value * loadImag(JITVisitor & jit) override;
    llvm::Value * getImag(JITVisitor & jit) const override;

    std::pair<llvm::Value *, llvm::Value *> loadReIm(JITVisitor & jit) override;
    void storeImag(JITVisitor & jit, llvm::Value * _imag) override;
    void storeReIm(JITVisitor & jit, std::pair<llvm::Value *, llvm::Value *> reim) override;
    void setImag(llvm::Value * imag) override;


    static llvm::Type * getCpx128Ty(JITVisitor & jit);
};
}

#endif // __JIT_SCALARS_HXX__
