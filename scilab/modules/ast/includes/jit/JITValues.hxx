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

#ifndef __JIT_VALUES_HXX__
#define __JIT_VALUES_HXX__

#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

#include "internal.hxx"
#include "JITVisitor.hxx"
#include "TIType.hxx"

namespace jit
{
class JITVal
{

public:

    JITVal(llvm::Value * _val, llvm::Type * _type) : val(_val), type(_type) { }

    virtual bool is_scalar() const = 0;
    virtual int get_type_size() const = 0;
    virtual types::InternalType::ScilabId get_scilab_id() const = 0;
    virtual void store(llvm::Value * x, JITVisitor & visitor) const = 0;
    virtual void store(llvm::Value * x, llvm::Value * free, JITVisitor & visitor) const = 0;
    virtual void storeR(llvm::Value * _r, JITVisitor & visitor) const = 0;
    virtual void storeC(llvm::Value * _c, JITVisitor & visitor) const = 0;
    virtual llvm::Value * load(JITVisitor & visitor) const = 0;
    virtual llvm::Value * loadR(JITVisitor & visitor) const = 0;
    virtual llvm::Value * loadC(JITVisitor & visitor) const = 0;
    virtual llvm::Value * getR() const = 0;
    virtual llvm::Value * getC() const = 0;
    virtual llvm::Value * get_must_free() const = 0;
    virtual void free() = 0;

    inline llvm::Value * get_value() const
    {
        return val;
    }

    inline llvm::Type * get_type() const
    {
        return type;
    }

    inline bool is_null() const
    {
        return !val;
    }

    void store(JITVal & x, JITVisitor & visitor) const
    {
        if (is_scalar())
        {
            store(x.val, visitor);
        }
        else
        {
            store(x.val, x.get_must_free(), visitor);
            storeR(x.getR(), visitor);
            storeC(x.getC(), visitor);
        }
    }

    static JITVal * get(JITVisitor & visitor, types::InternalType * const pIT, const bool alloc = false, const std::string & name = "");
    static JITVal * get(JITVisitor & visitor, const analysis::TIType & t, const bool alloc = false, const std::string & name = "");

protected:

    llvm::Value * val;
    llvm::Type * type;

    template<typename T>
    inline static llvm::Value * load(llvm::Value * v, bool _scalar, llvm::IRBuilder<> & builder)
    {
        if (llvm::isa<llvm::AllocaInst>(v))
        {
            return builder.CreateAlignedLoad(llvm::cast<llvm::AllocaInst>(v), _scalar ? sizeof(T) : sizeof(T *));
        }

        return v;
    }

    template<typename T>
    inline static void store(llvm::Value * v, llvm::Value * x, bool _scalar, llvm::IRBuilder<> & builder)
    {
        if (llvm::isa<llvm::AllocaInst>(v))
        {
            llvm::Value * _x = llvm::isa<llvm::AllocaInst>(x) ? builder.CreateAlignedLoad(llvm::cast<llvm::AllocaInst>(x), sizeof(T)) : x;

            builder.CreateAlignedStore(_x, llvm::cast<llvm::AllocaInst>(v), _scalar ? sizeof(T) : sizeof(T *));
        }
        else
        {
            throw ast::InternalError("Scilab: Cannot store a value !");
        }
    }
};

template<typename T>
class JITScalarVal : public JITVal
{
public:

    JITScalarVal(JITVisitor & visitor, T _val, bool alloc = false, const std::string & name = "") : JITVal(alloc ? visitor.getAlloca(_val, name) : visitor.getConstant(_val), getLLVMTy<T>(visitor.getContext())) { }

    JITScalarVal(JITVisitor & visitor, JITVal * _val, const std::string & name = "") : JITVal(visitor.getAlloca(_val->get_value(), _val->get_type(), _val->get_type_size(), name), _val->get_type()) { }

    JITScalarVal(JITVisitor & visitor, llvm::Value * _val) : JITVal(_val, getLLVMTy<T>(visitor.getContext())) { }

    inline bool is_scalar() const
    {
        return true;
    }

    inline int get_type_size() const
    {
        return sizeof(T);
    }

    inline types::InternalType::ScilabId get_scilab_id() const
    {
        return types::InternalType::IdScalarDouble;
    }

    inline void store(llvm::Value * x, JITVisitor & visitor) const
    {
        JITVal::store<T>(this->val, x, true, visitor.getBuilder());
    }

    inline void store(llvm::Value * x, llvm::Value * free, JITVisitor & visitor) const { }

    inline void storeR(llvm::Value * _r, JITVisitor & visitor) const { }
    inline void storeC(llvm::Value * _c, JITVisitor & visitor) const { }

    llvm::Value * load(JITVisitor & visitor) const
    {
        return JITVal::load<T>(val, true, visitor.getBuilder());
    }

    inline llvm::Value * loadR(JITVisitor & visitor) const
    {
        return JITVisitor::ONE;
    }
    inline llvm::Value * loadC(JITVisitor & visitor) const
    {
        return JITVisitor::ONE;
    }

    inline llvm::Value * getR() const
    {
        return nullptr;
    }
    inline llvm::Value * getC() const
    {
        return nullptr;
    }


    inline llvm::Value * get_must_free() const
    {
        return nullptr;
    }

    inline void free() { }
};

template<typename T>
class JITMatrixVal : public JITVal
{
    llvm::Value * r;
    llvm::Value * c;
    llvm::Value * must_be_freed;

public:

    JITMatrixVal(JITVisitor & visitor, const int _r, const int _c, T * _val, const bool alloc = false, const std::string & name = "") : JITVal(alloc ? visitor.getAllocaPtr(_val, name) : visitor.getConstantPtr(_val), getLLVMTy<T * >(visitor.getContext())),
        r(alloc ? visitor.getAlloca(_r, name + "_row") : visitor.getConstant(_r)),
        c(alloc ? visitor.getAlloca(_c, name + "_col") : visitor.getConstant(_c)),
        must_be_freed(alloc ? visitor.getAlloca<bool>(false, name + "_MBF") : visitor.getConstant(false))
    {
    }

    JITMatrixVal(JITVisitor & visitor, llvm::Value * _r, llvm::Value * _c, llvm::Value * _val) : JITVal(/*visitor.getAllocaPtr<T>(_val)*/_val, getLLVMTy<T * >(visitor.getContext())), r(_r), c(_c), must_be_freed(visitor.getConstant(true/*visitor.getAlloca<bool>(true, "must_be_freed_t")*/))
    {
    }

    inline llvm::Value * get_must_free() const
    {
        return must_be_freed;
    }

    inline void free() { }

    inline bool is_scalar() const
    {
        return false;
    }

    inline int get_type_size() const
    {
        return sizeof(T*);
    }

    inline types::InternalType::ScilabId get_scilab_id() const
    {
        return types::InternalType::IdDouble;
    }

    static void debug(double * p)
    {
        std::cout << "free=" << (void*)p << std::endl;
    }

    inline void store(llvm::Value * x, llvm::Value * free, JITVisitor & visitor) const
    {
        llvm::Function * func = &visitor.getFunction();
        llvm::LLVMContext & context = visitor.getContext();
        llvm::IRBuilder<> & builder = visitor.getBuilder();
        llvm::BasicBlock * cur_block = builder.GetInsertBlock();

        llvm::BasicBlock * BBThen = llvm::BasicBlock::Create(context, "", func);
        llvm::BasicBlock * BBElse = llvm::BasicBlock::Create(context, "", func);

        builder.CreateCondBr(JITVal::load<bool>(must_be_freed, false, visitor.getBuilder()), BBThen, BBElse);

        builder.SetInsertPoint(BBThen);

        //llvm::Value * toCall = visitor.getPointer(reinterpret_cast<void *>(&debug), getLLVMPtrFuncTy<void, double *>(context));
        //builder.CreateCall(toCall, load(visitor));
        llvm::Value * _free = llvm::CallInst::CreateFree(load(visitor), BBThen);
        BBThen->getInstList().push_back(llvm::cast<llvm::Instruction>(_free));

        builder.CreateBr(BBElse);
        builder.SetInsertPoint(BBElse);
        store(x, visitor);
        JITVal::store<bool>(must_be_freed, free, false, visitor.getBuilder());
    }

    inline void store(llvm::Value * x, JITVisitor & visitor) const
    {
        JITVal::store<T>(this->val, x, false, visitor.getBuilder());
    }

    inline void storeR(llvm::Value * _r, JITVisitor & visitor) const
    {
        JITVal::store<int>(this->r, _r, true, visitor.getBuilder());
    }

    inline void storeC(llvm::Value * _c, JITVisitor & visitor) const
    {
        JITVal::store<int>(this->c, _c, true, visitor.getBuilder());
    }

    inline llvm::Value * load(JITVisitor & visitor) const
    {
        return JITVal::load<T>(val, false, visitor.getBuilder());
    }

    inline llvm::Value * loadR(JITVisitor & visitor) const
    {
        return JITVal::load<int>(r, true, visitor.getBuilder());
    }

    inline llvm::Value * loadC(JITVisitor & visitor) const
    {
        return JITVal::load<int>(c, true, visitor.getBuilder());
    }

    inline llvm::Value * getR() const
    {
        return r;
    }
    inline llvm::Value * getC() const
    {
        return c;
    }
};


} // namespace jit

#endif // __JIT_VALUES_HXX__
