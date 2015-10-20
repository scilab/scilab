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

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/FunctionSignature.hxx"
#include "calls/JITCall.hxx"
#include "calls/JITSize.hxx"
#include "calls/JITZeros.hxx"
#include "calls/JITOptimizedCall1.hxx"
#include "calls/JITSign.hxx"
#include "calls/JITSqrt.hxx"
#include "calls/JITLog.hxx"
#include "calls/JITAbs.hxx"
#include "calls/JITAngle.hxx"
#include "calls/JITReal.hxx"
#include "calls/JITImag.hxx"
#include "calls/JITImult.hxx"
#include "calls/JITConj.hxx"
#include "calls/JITFloor.hxx"
#include "calls/JITCeil.hxx"
#include "calls/JITRound.hxx"
#include "calls/JITTicToc.hxx"

namespace jit
{

    JITCall::FunMap JITCall::funs = init();

    bool JITCall::call(const ast::CallExp & e, std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
    {
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e.getName());
        const symbol::Symbol & funSym = var.getSymbol();
        const std::wstring & funName = funSym.getName();
        auto i = funs.find(funName);
        if (i != funs.end())
        {
            // the function is a "special" call
            // such as sinus, cosinus, ...
            return i->second->invoke(e, typesOut, out, jit);
        }
        else if (uint64_t id = e.getDecorator().getResult().getFunctionId())
        {
	    return callCompiledMacro(id, e, typesOut, out, jit);
	}
            /*else if (types::InternalType * pIT = symbol::Context::getInstance()->get(sym))
              {
              if (pIT->isCallable())
              {
              switch (pIT->getType())
              {
              case types::InternalType::ScilabFunction:
              case types::InternalType::ScilabMacro:
              case types::InternalType::ScilabMacroFile:
              case types::InternalType::ScilabLibrary:
              default:
              return false;
              }
              }
              }*/
	
	return false;
    }


        JITCall::FunMap JITCall::init()
        {
            FunMap map;

            map.emplace(L"tic", std::shared_ptr<JITCall>(new JITTic()));
            map.emplace(L"toc", std::shared_ptr<JITCall>(new JITToc()));
            map.emplace(L"size", std::shared_ptr<JITCall>(new JITSize()));
            map.emplace(L"zeros", std::shared_ptr<JITCall>(new JITZeros()));
            map.emplace(L"sin", std::shared_ptr<JITCall>(new JITOptimizedCall1("sin", {{ analysis::TIType::COMPLEX, "csin" }}, llvm::Intrinsic::sin)));
            map.emplace(L"cos", std::shared_ptr<JITCall>(new JITOptimizedCall1("cos", {{ analysis::TIType::COMPLEX, "ccos" }}, llvm::Intrinsic::cos)));
            map.emplace(L"exp", std::shared_ptr<JITCall>(new JITOptimizedCall1("exp", {{ analysis::TIType::COMPLEX, "cexp" }}, llvm::Intrinsic::exp)));
            map.emplace(L"floor", std::shared_ptr<JITCall>(new JITFloor()));
            map.emplace(L"ceil", std::shared_ptr<JITCall>(new JITCeil()));
            map.emplace(L"round", std::shared_ptr<JITCall>(new JITRound()));
            map.emplace(L"atan", std::shared_ptr<JITCall>(new JITOptimizedCall1("atan", {{ analysis::TIType::DOUBLE, "atan" }, { analysis::TIType::COMPLEX, "catan" }})));
            map.emplace(L"cosh", std::shared_ptr<JITCall>(new JITOptimizedCall1("cosh", {{ analysis::TIType::DOUBLE, "cosh"}, { analysis::TIType::COMPLEX, "ccosh" }})));
            map.emplace(L"sinh", std::shared_ptr<JITCall>(new JITOptimizedCall1("sinh", {{ analysis::TIType::DOUBLE, "sinh" }, { analysis::TIType::COMPLEX, "csinh" }})));
            map.emplace(L"erf", std::shared_ptr<JITCall>(new JITOptimizedCall1("erf", {{ analysis::TIType::DOUBLE, "erf" }})));
            map.emplace(L"erfc", std::shared_ptr<JITCall>(new JITOptimizedCall1("erfc", {{ analysis::TIType::DOUBLE, "erfc" }})));
            map.emplace(L"erfcx", std::shared_ptr<JITCall>(new JITOptimizedCall1("erfcx")));
            map.emplace(L"sign", std::shared_ptr<JITCall>(new JITSign()));
            map.emplace(L"sqrt", std::shared_ptr<JITCall>(new JITSqrt()));
            map.emplace(L"log", std::shared_ptr<JITCall>(new JITLog(JITLog::BASE_E)));
            map.emplace(L"log2", std::shared_ptr<JITCall>(new JITLog(JITLog::BASE_2)));
            map.emplace(L"log10", std::shared_ptr<JITCall>(new JITLog(JITLog::BASE_10)));
            map.emplace(L"log1p", std::shared_ptr<JITCall>(new JITOptimizedCall1("log1p", {{ analysis::TIType::DOUBLE, "log1p" }})));
            map.emplace(L"abs", std::shared_ptr<JITCall>(new JITAbs()));
            map.emplace(L"angle", std::shared_ptr<JITCall>(new JITAngle()));
            map.emplace(L"real", std::shared_ptr<JITCall>(new JITReal()));
            map.emplace(L"imag", std::shared_ptr<JITCall>(new JITImag()));
            map.emplace(L"imult", std::shared_ptr<JITCall>(new JITImult()));
            map.emplace(L"conj", std::shared_ptr<JITCall>(new JITConj()));
            return map;
        }

/*
  elem_func_gw.cpp  sci_atan.cpp   sci_cosh.cpp     sci_expm.cpp   sci_imult.cpp    sci_log.cpp     sci_rat.cpp    sci_size.cpp  sci_zeros.cpp
  sci_abs.cpp       sci_atanh.cpp  sci_cumprod.cpp  sci_eye.cpp    sci_int.cpp      sci_matrix.cpp  sci_real.cpp   sci_sum.cpp
  sci_acos.cpp      sci_ceil.cpp   sci_cumsum.cpp   sci_floor.cpp  sci_isequal.cpp  sci_max.cpp     sci_round.cpp  sci_tan.cpp
  sci_acosh.cpp     sci_clean.cpp  sci_diag.cpp     sci_frexp.cpp  sci_isreal.cpp   sci_ones.cpp    sci_sign.cpp   sci_tanh.cpp
  sci_asin.cpp      sci_conj.cpp   sci_dsearch.cpp  sci_gsort.cpp  sci_kron.cpp     sci_prod.cpp    sci_sin.cpp    sci_tril.cpp
  sci_asinh.cpp     sci_cos.cpp    sci_exp.cpp      sci_imag.cpp   sci_log1p.cpp    sci_rand.cpp    sci_sinh.cpp   sci_triu.cpp
*/


    bool JITCall::callCompiledMacro(const uint64_t id, const ast::CallExp & e, std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
    {
	// The macro has been compiled so we can call it
	if (const JITInfo * info = jit.getInfo(id))
	{
	    llvm::Function * toCall = jit.getFunction(info->getName());
	    const ast::exps_t args = e.getArgs();
	    std::vector<llvm::Value *> llvmArgs;
	    
	    // Put in arguments
	    for (const auto arg : args)
	    {
		arg->accept(jit);
		JITScilabPtr & res = jit.getResult();
		const analysis::TIType & argType = arg->getDecorator().getResult().getType();
		FunctionSignature::getFunctionArgs(jit, llvmArgs, In<JITScilabPtr, 0>(res));
		
		if (!argType.isscalar())
		{
		    FunctionSignature::getFunctionArgs(jit, llvmArgs,
						       In<llvm::Value>(res->loadRows(jit)),
						       In<llvm::Value>(res->loadCols(jit)),
						       In<llvm::Value>(jit.getConstant<int64_t>(0)));
		}
	    }
	    
	    // Put out arguments
	    if (out.empty())
	    {
		out.emplace_back(jit.getTemp(e.getDecorator().getResult().getTempId()));
		typesOut.emplace_back(e.getDecorator().getResult().getType());
	    }
	    // TODO: handle correctly multi output args
	    for (auto & argOut : out)
	    {
		FunctionSignature::getFunctionArgs(jit, llvmArgs, In<JITScilabPtr, 1>(argOut));
		if (!typesOut.front().isscalar())
		{
		    FunctionSignature::getFunctionArgs(jit, llvmArgs,
						       In<llvm::Value>(argOut->getRows(jit)),
						       In<llvm::Value>(argOut->getCols(jit)),
						       In<llvm::Value>(argOut->getRefCount(jit)));
		}
	    }

	    jit.getBuilder().CreateCall(toCall, llvmArgs);
	    //setResult(out);
	}
    }
    
}
