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

#ifndef __JIT_SCILAB_VAL_HXX__
#define __JIT_SCILAB_VAL_HXX__

#include "internal.hxx"
#include "JITVisitor.hxx"
//#include "JITVal.hxx"

namespace jit
{
class JITScilabVal
{

public:

    virtual ~JITScilabVal() { }

    virtual bool isValid() const = 0;
    virtual llvm::Value * getRows(JITVisitor & jit) const = 0;
    virtual llvm::Value * getCols(JITVisitor & jit) const = 0;
    virtual llvm::Value * loadRows(JITVisitor & jit) const = 0;
    virtual llvm::Value * loadCols(JITVisitor & jit) const = 0;
    virtual void setData(llvm::Value * data) = 0;
    virtual void setRows(llvm::Value * rows) = 0;
    virtual void setCols(llvm::Value * cols) = 0;
    virtual void setRefCount(llvm::Value * refCount) = 0;
    virtual void storeRows(JITVisitor & jit, llvm::Value * rows) = 0;
    virtual void storeCols(JITVisitor & jit, llvm::Value * cols) = 0;
    virtual bool isScalar() const = 0;
    virtual llvm::Value * getData(JITVisitor & jit) const = 0;
    virtual llvm::Value * getReal(JITVisitor & jit) const
    {
        return getData(jit);
    }
    virtual llvm::Value * getImag(JITVisitor & jit) const
    {
        return nullptr;
    }
    virtual llvm::Value * loadData(JITVisitor & jit) = 0;
    virtual llvm::Value * loadReal(JITVisitor & jit)
    {
        return loadData(jit);
    }
    virtual llvm::Value * loadImag(JITVisitor & jit)
    {
        return nullptr;
    }
    virtual std::pair<llvm::Value *, llvm::Value *> loadReIm(JITVisitor & jit)
    {
        return std::pair<llvm::Value *, llvm::Value *>(nullptr, nullptr);
    }
    virtual void storeData(JITVisitor & jit, llvm::Value * data) = 0;
    virtual void storeReal(JITVisitor & jit, llvm::Value * real)
    {
        storeData(jit, real);
    }
    virtual void storeImag(JITVisitor & jit, llvm::Value * imag) { }
    virtual void storeReIm(JITVisitor & jit, std::pair<llvm::Value *, llvm::Value *> imag) { }
    virtual void setReal(llvm::Value * real)
    {
        setData(real);
    }
    virtual void setImag(llvm::Value * imag) { }
    //virtual types::InternalType::ScilabId getScilabId() const = 0;
    //virtual void setScilabId(const types::InternalType::ScilabId id) = 0;
    virtual llvm::Value * getRefCount(JITVisitor & jit) const = 0;
    virtual llvm::Value * loadRefCount(JITVisitor & jit) const = 0;
    virtual void incRefCount(JITVisitor & jit) = 0;
    virtual void decRefCount(JITVisitor & jit) = 0;
    virtual bool isSigned() const = 0;
    virtual bool isComplex() const
    {
        return false;
    }
};

}

#endif // __JIT_SCILAB_VAL_HXX__
