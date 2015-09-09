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
#include "calls/JITShortcutBinOpCall.hxx"
#include "Cast.hxx"

namespace jit
{
    
    JITScilabPtr JITShortcutEq::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
	analysis::TIType retType(analysis::TIType::BOOLEAN);
	return jit.equality.SS(L, Ltype, R, Rtype, retType, jit);
    }

    JITScilabPtr JITShortcutNe::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
	analysis::TIType retType(analysis::TIType::BOOLEAN);
	return jit.not_equality.SS(L, Ltype, R, Rtype, retType, jit);
    }

    JITScilabPtr JITShortcutGt::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
	analysis::TIType retType(analysis::TIType::BOOLEAN);
	return jit.greater_than.SS(L, Ltype, R, Rtype, retType, jit);
    }

    JITScilabPtr JITShortcutLt::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
	analysis::TIType retType(analysis::TIType::BOOLEAN);
	return jit.lower_than.SS(L, Ltype, R, Rtype, retType, jit);
    }

    JITScilabPtr JITShortcutGe::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
	analysis::TIType retType(analysis::TIType::BOOLEAN);
	return jit.greater_or_eq.SS(L, Ltype, R, Rtype, retType, jit);
    }

    JITScilabPtr JITShortcutLe::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
	analysis::TIType retType(analysis::TIType::BOOLEAN);
	return jit.lower_or_eq.SS(L, Ltype, R, Rtype, retType, jit);
    }
    
}
