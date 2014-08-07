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

#ifndef __JIT_OPERATIONS_HXX__
#define __JIT_OPERATIONS_HXX__

#include "JITValues.hxx"
#include "types_addition.hxx"
#include "types_substraction.hxx"
#include "types_multiplication.hxx"

namespace jit
{
template<typename, typename>
inline static llvm::Value * add(llvm::Value * L, llvm::Value * R, llvm::IRBuilder<> & builder);

template<typename, typename>
inline static llvm::Value * sub(llvm::Value * L, llvm::Value * R, llvm::IRBuilder<> & builder);

template<typename, typename>
inline static llvm::Value * dotmul(llvm::Value * L, llvm::Value * R, llvm::IRBuilder<> & builder);

template<typename T>
inline static llvm::Value * add(llvm::Value * L, llvm::Value * R, llvm::IRBuilder<> & builder)
{
    return std::is_integral<T>::value ? builder.CreateAdd(L, R) : builder.CreateFAdd(L, R);
}

template<typename T>
inline static llvm::Value * sub(llvm::Value * L, llvm::Value * R, llvm::IRBuilder<> & builder)
{
    return std::is_integral<T>::value ? builder.CreateSub(L, R) : builder.CreateFSub(L, R);
}

template<typename T>
inline static llvm::Value * dotmul(llvm::Value * L, llvm::Value * R, llvm::IRBuilder<> & builder)
{
    return std::is_integral<T>::value ? builder.CreateMul(L, R) : builder.CreateFMul(L, R);
}

class JITVisitor;
typedef std::shared_ptr<JITVal> (*add_function)(std::shared_ptr<JITVal> &, std::shared_ptr<JITVal> &, JITVisitor &);
static add_function pAddfunction[types::InternalType::IdLast][types::InternalType::IdLast];

/*template<typename>
  inline static JITVal add(JITVal & L, JITVal & R, JITVisitor & visitor);*/

std::shared_ptr<JITVal> add_D_D(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor);
std::shared_ptr<JITVal> sub_D_D(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor);
std::shared_ptr<JITVal> dotmul_D_D(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor);

std::shared_ptr<JITVal> add_M_M(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor);
std::shared_ptr<JITVal> sub_M_M(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor);
std::shared_ptr<JITVal> dotmul_M_M(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor);

}

#endif // __JIT_OPERATIONS_HXX__
