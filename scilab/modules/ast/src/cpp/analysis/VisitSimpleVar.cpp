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

#include "AnalysisVisitor.hxx"

namespace analysis
{

void AnalysisVisitor::visit(ast::SimpleVar & e)
{
    logger.log("SimpleVar", e.getSymbol().getName(), e.getLocation());
    symbol::Symbol & sym = e.getSymbol();
    Info & info = dm.read(sym, &e);
    Result & res = e.getDecorator().setResult(info.type);
    res.setConstant(info.getConstant());
    res.setRange(info.getRange());
    res.setMaxIndex(info.getMaxIndex());
    setResult(res);
}

} // namespace analysis
