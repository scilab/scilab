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

#ifndef __JIT_ARRAYOFS_HXX__
#define __JIT_ARRAYOFS_HXX__

#include <complex>

#include "internal.hxx"
#include "JITVisitor.hxx"
#include "JITArrayof.hxx"

namespace jit
{
template<typename T>
class JITArrayofT : public JITArrayof
{

public:

    JITArrayofT(llvm::Value * const data, llvm::Value * const rows, llvm::Value * const cols, llvm::Value * const refCount, const std::string & name) : JITArrayof(data, rows, cols, refCount, name) { }

    JITArrayofT(JITVisitor & jit, const std::string & name) : JITArrayof(jit.getAlloca<T *>(), jit.getAlloca<int64_t>(), jit.getAlloca<int64_t>(), jit.getAlloca<int64_t>(), name) { }

    JITArrayofT(JITVisitor & jit, const std::string & name, const bool init) : JITArrayof(init ? jit.getValue((T *)nullptr, true) : jit.getAlloca<T *>(), init ? jit.getValue(int64_t(0), true) : jit.getAlloca<int64_t>(), init ? jit.getValue(int64_t(0), true) : jit.getAlloca<int64_t>(), init ? jit.getValue(int64_t(0), true) : jit.getAlloca<int64_t>(), name) { }

    JITArrayofT(JITVisitor & jit, const T * data, const int64_t rows, const int64_t cols, const std::string & name) : JITArrayof(jit.getValue(data, true), jit.getValue(rows, true), jit.getValue(cols, true), jit.getValue<int64_t>(0), name) { }

    JITArrayofT(JITVisitor & jit, llvm::Value * const data, llvm::Value * const rows, llvm::Value * const cols, llvm::Value * const refCount, const std::string & name) : JITArrayof(jit, data, rows, cols, refCount, name) { }

    JITArrayofT(JITVisitor & jit, llvm::Value * const data, llvm::Value * const rows, llvm::Value * const cols, llvm::Value * const refCount, const bool alloc, const std::string & name) : JITArrayof(jit, data, rows, cols, refCount, alloc, name) { }

    virtual ~JITArrayofT() { }

    bool isSigned() const override
    {
        return std::is_integral<T>::value && std::is_signed<T>::value;
    }

};

typedef JITArrayofT<double> JITArrayofDouble;
typedef JITArrayofT<int8_t> JITArrayofInt8;
typedef JITArrayofT<int16_t> JITArrayofInt16;
typedef JITArrayofT<int32_t> JITArrayofInt32;
typedef JITArrayofT<int64_t> JITArrayofInt64;
typedef JITArrayofT<uint8_t> JITArrayofUInt8;
typedef JITArrayofT<uint16_t> JITArrayofUInt16;
typedef JITArrayofT<uint32_t> JITArrayofUInt32;
typedef JITArrayofT<uint64_t> JITArrayofUInt64;
typedef JITArrayofT<int32_t> JITArrayofBool;
typedef JITArrayofT<wchar_t> JITArrayofString;

class JITArrayofComplex : public JITArrayof
{

    llvm::Value * imag;

public:

    JITArrayofComplex(llvm::Value * const _real, llvm::Value * const _imag, llvm::Value * const _rows, llvm::Value * const _cols, llvm::Value * const _refCount, const std::string & _name) : JITArrayof(_real, _rows, _cols, _refCount, _name), imag(_imag)
    {
        if (!_name.empty())
        {
            imag->setName(_name);
        }
    }

    JITArrayofComplex(JITVisitor & jit, const std::string & name) : JITArrayof(jit.getAlloca<double *>(), jit.getAlloca<int64_t>(), jit.getAlloca<int64_t>(), jit.getAlloca<int64_t>(), name), imag(jit.getAlloca<double *>())
    {
        if (!name.empty())
        {
            imag->setName(name);
        }
    }

    JITArrayofComplex(JITVisitor & jit, const std::string & name, const bool init) : JITArrayof(init ? jit.getValue((double *)nullptr, true) : jit.getAlloca<double *>(), init ? jit.getValue(int64_t(0), true) : jit.getAlloca<int64_t>(), init ? jit.getValue(int64_t(0), true) : jit.getAlloca<int64_t>(), init ? jit.getValue(int64_t(0), true) : jit.getAlloca<int64_t>(), name), imag(init ? jit.getValue((double *)nullptr, true) : jit.getAlloca<double *>())
    {
        if (!name.empty())
        {
            imag->setName(name);
        }
    }

    JITArrayofComplex(JITVisitor & jit, const double * _real, const double * _imag, const int64_t _rows, const int64_t _cols, const std::string & _name) : JITArrayof(jit.getValue(_real, true), jit.getValue(_rows, true), jit.getValue(_cols, true), jit.getValue<int64_t>(0), _name), imag(jit.getValue(_imag, true))
    {
        if (!_name.empty())
        {
            imag->setName(_name);
        }
    }

    JITArrayofComplex(JITVisitor & jit, llvm::Value * const _real, llvm::Value * const _imag, llvm::Value * const _rows, llvm::Value * const _cols, llvm::Value * const _refCount, const std::string & _name) : JITArrayof(jit, _real, _rows, _cols, _refCount, _name), imag(_imag)
    {
        if (!_name.empty())
        {
            imag->setName(_name);
        }
    }

    JITArrayofComplex(JITVisitor & jit, llvm::Value * const _real, llvm::Value * _imag, llvm::Value * const _rows, llvm::Value * const _cols, llvm::Value * const _refCount, const bool _alloc, const std::string & _name) : JITArrayof(jit, _real, _rows, _cols, _refCount, _alloc, _name), imag(jit.getValue(_imag, _alloc))
    {
        if (!_name.empty())
        {
            imag->setName(_name);
        }
    }

    virtual ~JITArrayofComplex() { }

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

};

}

#endif // __JIT_ARRAYOFS_HXX__
