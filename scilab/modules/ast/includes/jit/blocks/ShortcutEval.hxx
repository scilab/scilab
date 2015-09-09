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

#ifndef __SHORTCUT_EVAL_HXX__
#define __SHORTCUT_EVAL_HXX__

#include "JITVisitor.hxx"

namespace jit
{
    /**
     * The goal of this class is to generate IR for a shortcut evaluation of conditions in 'if'
     * or 'while' blocks.
     * For example if we have something like:
     *   if ((A & B) | (C | D)) then P else Q end
     * then we should have something like:
     * 
     * i1 t = test A
     * br t L2, L1
     * L2:
     *   i1 t = test B
     *   br t P, L1
     * L1:
     *   i1 t = test C
     *   br t P, L3
     * L3:
     *   i1 t = test D
     *   br t P, Q
     * P:
     *   ...
     * Q:
     *   ...
     *
     * To improve performance, we could test A in using a shortcut evaluation, for example:
     *   if (A == 0) then ...
     * it is not required to create a boolean matrix containing all the a_{ij} == 0 and make a 'and' on its elements,
     * we can just break the loop when a a_{ij} != 0.
     */
    class ShortcutEval : public ast::ConstVisitor, public analysis::Chrono
    {

        JITVisitor & jit;
        llvm::BasicBlock * trueBlock;
        llvm::BasicBlock * falseBlock;
	JITScilabPtr _result;

    public:

        ShortcutEval(JITVisitor & _jit, llvm::BasicBlock * t, llvm::BasicBlock * f) : jit(_jit), trueBlock(t), falseBlock(f) { }

	inline JITScilabPtr & getResult()
        {
            return  _result;
        }

    private:

        inline void setResult(JITScilabPtr & val)
        {
            _result = val;
        }

        inline void setResult(JITScilabPtr && val)
        {
            _result = val;
        }

        inline void setBlocks(llvm::BasicBlock * t, llvm::BasicBlock * f)
            {
                trueBlock = t;
                falseBlock = f;
            }

        inline llvm::BasicBlock * getTrue()
            {
                return trueBlock;
            }

        inline llvm::BasicBlock * getFalse()
            {
                return falseBlock;
            }

    public:

        void visit(const ast::SimpleVar & e);
        void visit(const ast::NotExp & e);
        void visit(const ast::TransposeExp & e);
        void visit(const ast::MatrixExp & e);
        void visit(const ast::CellExp & e);
        void visit(const ast::CallExp & e);
        void visit(const ast::OpExp & e);
        void visit(const ast::LogicalOpExp & e);
        void visit(const ast::DoubleExp & e);
        void visit(const ast::BoolExp & e);

	void visit(const ast::ListExp & e)
	    {
	    }

        void visit(const ast::IfExp & e)
            {
            }

        void visit(const ast::DollarVar & e)
            {
            }

        void visit(const ast::ColonVar & e)
            {
            }

        void visit(const ast::ArrayListVar & e)
            {
            }

        void visit(const ast::StringExp & e)
            {
            }

        void visit(const ast::CommentExp & e)
            {
            }

        void visit(const ast::NilExp & e)
            {
            }

        void visit(const ast::CellCallExp & e)
            {
            }

        void visit(const ast::AssignExp & e)
            {
            }

        void visit(const ast::WhileExp & e)
            {
            }

        void visit(const ast::ForExp & e)
            {
            }

        void visit(const ast::BreakExp & e)
            {
            }

        void visit(const ast::ContinueExp & e)
            {
            }

        void visit(const ast::TryCatchExp & e)
            {
            }

        void visit(const ast::SelectExp & e)
            {
            }

        void visit(const ast::CaseExp & e)
            {
            }

        void visit(const ast::ReturnExp & e)
            {
            }

        void visit(const ast::FieldExp & e)
            {
            }

        void visit(const ast::MatrixLineExp & e)
            {
            }

        void visit(const ast::SeqExp & e)
            {
            }

        void visit(const ast::ArrayListExp & e)
            {
            }

        void visit(const ast::AssignListExp & e)
            {
            }

        void visit(const ast::VarDec & e)
            {
            }

        void visit(const ast::FunctionDec & e)
            {
            }

        void visit(const ast::OptimizedExp & e)
            {
            }

        void visit(const ast::MemfillExp & e)
            {
            }

        void visit(const ast::DAXPYExp & e)
            {
            }

        void visit(const ast::IntSelectExp & e)
            {
            }

        void visit(const ast::StringSelectExp & e)
            {
            }

    private:

	JITScilabPtr toBool(JITScilabPtr & L, const analysis::TIType & Ltype);
	llvm::Value * toBoolS(JITScilabPtr & L, const analysis::TIType & Ltype);
	llvm::Value * toBoolM(JITScilabPtr & L, const analysis::TIType & Ltype);

	inline void defaultVisit(const ast::Exp & e)
	    {
		e.accept(jit);
		setResult(toBool(getResult(), e.getDecorator().getResult().getType()));
	    }
    };

} // namespace analysis

#endif // __SHORTCUT_EVAL_HXX__
