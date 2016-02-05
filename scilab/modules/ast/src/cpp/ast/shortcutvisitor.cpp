/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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

#include "shortcutvisitor.hxx"

namespace ast
{
void ShortCutVisitor::visit(const LogicalOpExp &e)
{
    ShortCutVisitor SCLeft;
    e.getLeft().accept(SCLeft);

    ShortCutVisitor SCRight;
    e.getRight().accept(SCRight);

    switch (e.getOper())
    {
        case LogicalOpExp::logicalOr :
            const_cast<LogicalOpExp&>(e).setOper(LogicalOpExp::logicalShortCutOr);
            break;
        case LogicalOpExp::logicalAnd :
            const_cast<LogicalOpExp&>(e).setOper(LogicalOpExp::logicalShortCutAnd);
            break;
        default :
            // Do Nothing !
            break;
    }
}
}
