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

#include "Jitter.hxx"
#include "AnalysisVisitor.hxx"
#include "JITVisitor.hxx"

namespace jit
{

    bool Jitter::analyzeAndJIT(const ast::CallExp & ce, const types::typed_list & in, types::typed_list & out)
    {
	static analysis::AnalysisVisitor analysis;
	static jit::JITVisitor jit;
	static bool init = false;

	if (!init)
	{
	    analysis.registerFBlockEmittedListener(&jit);
	}
	
	const ast::exps_t args = ce.getArgs();
	if (args.size() == 1)
	{
	    ast::Exp & arg = *args.back();
	    if (arg.isCallExp())
	    {
		ast::CallExp * ce = static_cast<ast::CallExp *>(arg.clone());
		ce->accept(analysis);
		jit.compile();
		if (const uint64_t fid = ce->getDecorator().getResult().getFunctionId())
		{
		    jit.makeCallFromScilab(fid, in, out);
		    delete ce;
		    jit.reset();
		    analysis.reset();

		    return true;
		}

		delete ce;
		jit.reset();
		analysis.reset();
	    }
	}

	return false;
    }
}
