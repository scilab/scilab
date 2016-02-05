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

void AnalysisVisitor::visit(ast::SimpleVar & e)
{
    logger.log(L"SimpleVar", e.getSymbol().getName(), e.getLocation());
    symbol::Symbol & sym = e.getSymbol();
    Info & info = dm.read(sym, &e);
    Result & res = e.getDecorator().setResult(info.type);
    res.setConstant(info.getConstant());
    res.setRange(info.getRange());
    res.setMaxIndex(info.getMaxIndex());
    setResult(res);
}

} // namespace analysis
