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
