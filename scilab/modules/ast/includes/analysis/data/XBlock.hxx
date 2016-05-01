/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __XBLOCK_HXX__
#define __XBLOCK_HXX__

#include "Block.hxx"

namespace analysis
{

class XBlock : public Block
{

public:

    XBlock(const unsigned int id, Block * parent, ast::Exp * exp) : Block(id, parent, exp) { }

    void finalize() override;
};

class XBlockHead : public Block
{
    std::vector<Block *> testBlocks;

public:

    XBlockHead(const unsigned int id, Block * parent, ast::Exp * exp) : Block(id, parent, exp) { }

    virtual ~XBlockHead()
    {
        std::for_each(testBlocks.begin(), testBlocks.end(), [](Block * b)
        {
            delete b;
        });
    }

    Block * addBlock(const unsigned int id, BlockKind kind, ast::Exp * exp) override;
    void finalize() override;
};

} // namespace analysis

#endif // __XBLOCK_HXX__
