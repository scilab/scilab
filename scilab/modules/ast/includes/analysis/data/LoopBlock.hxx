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

#ifndef __LOOPBLOCK_HXX__
#define __LOOPBLOCK_HXX__

#include <unordered_map>

#include "Block.hxx"
#include "tools.hxx"

namespace analysis
{

class LoopAnalyzer;

class LoopBlockHead : public Block
{
    /*
      A loop block naturally contains phi nodes:
      a = 0;
      b = 1;
      for i=1:N
      a = a + b // the values of a and b depends from where we come
      // a = phi(before loop, end loop) + phi(before loop, end loop)
      b = b + int8(1) // b = phi(before loop, end loop) + int8(1)
      end
      So we can suppose three cases:
      i) we don't enter in the loop => easy: we just ignore the loop
      ii) the loop has only one iteration => easy: the loop block is equivalent to a normal block
      iii) the loop has more than one iteration => types and refcounts need to be computed again
      according to the inferred types and refcounts at the end of the loop.
      The best case (except empty loops) is when info are the same before and after one iteration.
      In the previous example, after one iteration a is typed double and b is int8 but after a second iteration
      a & b are int8.

      The problem is that in the most of the cases loops should have a lot of iterations so we can suppose with a
      good probability that inferred types after the loop should be ones guessed after two iterations.
    */

public:

    LoopBlockHead(const unsigned int id, Block * parent, ast::Exp * exp) : Block(id, parent, exp) { }

    inline Block * getFirstBlock()
    {
        return blocks.front();
    }

    inline Block * getSecondBlock()
    {
        return blocks.back();
    }

    Block * addBlock(const unsigned int id, BlockKind kind, ast::Exp * exp) override;
    void finalize() override;
};

class LoopBlock : public Block
{

    friend class LoopBlockHead;

    const bool first;
    std::unordered_map<ast::Exp *, symbol::Symbol> clonedSym;

public:
    LoopBlock(const unsigned int id, Block * parent, ast::Exp * exp, const bool _first) : Block(id, parent, exp), first(_first) { }

    inline LoopBlockHead * getParent()
    {
        return static_cast<LoopBlockHead *>(parent);
    }

    bool requiresAnotherTrip() override;
    Block * getDefBlock(const symbol::Symbol & sym, tools::SymbolMap<Info>::iterator & it, const bool global) override;
    Info & getInfo(const symbol::Symbol & sym) override;
    void clone(const symbol::Symbol & sym, ast::Exp * exp) override;
    void finalize() override;
};

} // namespace analysis

#endif // __LOOPBLOCK_HXX__
