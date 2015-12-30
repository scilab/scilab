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

void AnalysisVisitor::visit(ast::VarDec & e)
{
    // VarDec is only used in For loop for iterator declaration
    logger.log("VarDec", e.getLocation());
    const symbol::Symbol & sym = e.getSymbol();
    if (e.getInit().isListExp())
    {
        ast::ListExp & le = static_cast<ast::ListExp &>(e.getInit());
        //e.setListInfo(ForList64());
        le.accept(*this);
        Result & res = getResult();
        Info & info = dm.define(sym, res.getType(), res.isAnInt(), &e);
        info.setRange(res.getRange());
        e.getDecorator().res = res;
    }
}

} // namespace analysis
