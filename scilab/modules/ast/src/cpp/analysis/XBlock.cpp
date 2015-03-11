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

#include "data/XBlock.hxx"

namespace analysis
{
void XBlock::finalize() { }

Block * XBlockHead::addBlock(const unsigned int id, BlockKind kind, ast::Exp * exp)
{
    Block * b = new XBlock(id, this, exp);
    blocks.push_back(b);

    return b;
}

void XBlockHead::finalize()
{
    pullup(symMap);
    std::vector<Block *>::iterator begin = blocks.begin();
    for (std::vector<Block *>::iterator i = ++blocks.begin(), end = blocks.end(); i != end; ++i)
    {
        merge((*begin)->getMap(), (*i)->getMap());
    }
    pullup((*begin)->getMap());
}
}//jit("if a==1;then a=2; else b=2; end")
