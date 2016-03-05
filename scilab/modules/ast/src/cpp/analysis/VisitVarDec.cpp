/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "AnalysisVisitor.hxx"

namespace analysis
{

void AnalysisVisitor::visit(ast::VarDec & e)
{
    // VarDec is only used in For loop for iterator declaration
    logger.log(L"VarDec", e.getLocation());
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
