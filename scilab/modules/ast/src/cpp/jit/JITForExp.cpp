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

#include <cstdlib>

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"

namespace jit
{
void JITVisitor::visit(const ast::VarDec & e)
{

}

/*void JITVisitor::visit(const ast::ForExp & e)
  {
  const ast::VarDec & vd = static_cast<const ast::VarDec &>(e.getVardec());
  // TODO : handle for with an iterator
  if (vd.getInit().isListExp())
  {
  const symbol::Symbol & symIterator = vd.getSymbol();
  const ast::ListExp & le = static_cast<const ast::ListExp &>(vd.getInit());
  if (le.getDecorator().getResult().getRange().isValid())
  {
  // for i = start:step:end...
  // is equivalent to for (int64_t i = start; i < end + step; i += step)...

  le.getStart().accept(*this);
  llvm::Value * start = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);
  le.getStep().accept(*this);
  llvm::Value * step = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);
  le.getEnd().accept(*this);
  llvm::Value * end = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);
  end = builder.CreateAdd(end, step);

  llvm::BasicBlock * cur_block = builder.GetInsertBlock();
  llvm::BasicBlock * condBlock = llvm::BasicBlock::Create(context, "for_cond", function);
  llvm::BasicBlock * loopBlock = llvm::BasicBlock::Create(context, "for_loop", function);
  llvm::BasicBlock * afterBlock = llvm::BasicBlock::Create(context, "for_after", function);

  blocks.emplace(condBlock, afterBlock);

  llvm::Value * cmp_i1 = builder.CreateICmpSLT(start, end);
  builder.CreateCondBr(cmp_i1, loopBlock, afterBlock);

  builder.SetInsertPoint(loopBlock);
  llvm::PHINode * i = builder.CreatePHI(getTy<int64_t>(), 2);
  i->addIncoming(start, cur_block);
  JITScilabPtr & it = variables.find(symIterator)->second;
  it->storeData(*this, i);

  e.getBody().accept(*this);
  builder.CreateBr(condBlock);

  builder.SetInsertPoint(condBlock);
  llvm::Value * ipstp_i64 = builder.CreateAdd(i, step);
  i->addIncoming(ipstp_i64, condBlock);
  cmp_i1 = builder.CreateICmpSLT(ipstp_i64, end);
  builder.CreateCondBr(cmp_i1, loopBlock, afterBlock);

  builder.SetInsertPoint(afterBlock);
  }
  else
  {
  const ast::Exp & startE = le.getStart();
  const ast::Exp & stepE = le.getStep();
  const ast::Exp & endE = le.getEnd();

  // a lot of for loops are like this: for i=1:N....
  // start is an integer and step is by default 1 so we can use a classical
  // loop with integer counter.

  if (startE.isDoubleExp() && stepE.isDoubleExp())
  {
  const ast::DoubleExp & startDE = static_cast<const ast::DoubleExp &>(startE);
  const ast::DoubleExp & stepDE = static_cast<const ast::DoubleExp &>(stepE);
  int64_t start_i;
  int64_t step_i;
  if (analysis::tools::asInteger(startDE.getValue(), start_i) && analysis::tools::asInteger(stepDE.getValue(), step_i))
  {
  llvm::Value * start = getConstant(start_i);
  llvm::Value * step = getConstant(std::abs(step_i));
  le.getEnd().accept(*this);
  llvm::Value * end = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);

  if (step_i > 0)
  {
  end = builder.CreateAdd(end, step);
  }
  else
  {
  end = builder.CreateSub(end, step);
  }

  llvm::BasicBlock * cur_block = builder.GetInsertBlock();
  llvm::BasicBlock * condBlock = llvm::BasicBlock::Create(context, "for_cond", function);
  llvm::BasicBlock * loopBlock = llvm::BasicBlock::Create(context, "for_loop", function);
  llvm::BasicBlock * afterBlock = llvm::BasicBlock::Create(context, "for_after", function);

  blocks.emplace(condBlock, afterBlock);

  llvm::Value * cmp_i1;
  if (step_i > 0)
  {
  cmp_i1 = builder.CreateICmpSLT(start, end);
  }
  else
  {
  cmp_i1 = builder.CreateICmpSGT(start, end);
  }
  builder.CreateCondBr(cmp_i1, loopBlock, afterBlock);

  builder.SetInsertPoint(loopBlock);
  llvm::PHINode * i = builder.CreatePHI(getTy<int64_t>(), 2);
  i->addIncoming(start, cur_block);
  JITScilabPtr & it = variables.find(symIterator)->second;
  it->storeData(*this, i);

  e.getBody().accept(*this);
  builder.CreateBr(condBlock);

  builder.SetInsertPoint(condBlock);
  llvm::Value * ipstp_i64;
  if (step_i > 0)
  {
  ipstp_i64 = builder.CreateAdd(i, step);
  }
  else
  {
  ipstp_i64 = builder.CreateSub(i, step);
  }

  i->addIncoming(ipstp_i64, condBlock);
  if (step_i > 0)
  {
  cmp_i1 = builder.CreateICmpSLT(ipstp_i64, end);
  }
  else
  {
  cmp_i1 = builder.CreateICmpSGT(ipstp_i64, end);
  }
  builder.CreateCondBr(cmp_i1, loopBlock, afterBlock);

  builder.SetInsertPoint(afterBlock);
  }
  }
  }

  }


  // e.getBody().accept(*this);
  }*/

void JITVisitor::visit(const ast::ForExp & e)
{
    const ast::VarDec & vd = static_cast<const ast::VarDec &>(e.getVardec());
    // TODO : handle for with an iterator
    if (vd.getInit().isListExp())
    {
        const symbol::Symbol & symIterator = vd.getSymbol();
        const ast::ListExp & le = static_cast<const ast::ListExp &>(vd.getInit());

        if (le.getDecorator().getResult().getType().type != analysis::TIType::EMPTY)
        {
            llvm::Value * start = nullptr;
            llvm::Value * step = nullptr;
            llvm::Value * end = nullptr;
            const ast::Exp & startE = le.getStart();
            const ast::Exp & stepE = le.getStep();
            const ast::Exp & endE = le.getEnd();
            const analysis::TIType & startTy = startE.getDecorator().getResult().getType();
            const analysis::TIType & stepTy = stepE.getDecorator().getResult().getType();
            const analysis::TIType & endTy = endE.getDecorator().getResult().getType();
            const bool startTyIsSigned = startTy.isintegral() && startTy.issigned();
            const bool stepTyIsSigned = stepTy.isintegral() && stepTy.issigned();
            const bool endTyIsSigned = endTy.isintegral() && endTy.issigned();
            bool constantStart = false;
            bool constantStep = false;
            bool constantEnd = false;
            bool signStep;
            bool integralStep = false;
            bool integerIterator = false;
            int64_t step_i;
            int64_t start_i;
            int64_t end_i;
            double step_d;
            double start_d;
            double end_d;

            if (stepE.isDoubleExp())
            {
                // Step is constant
                constantStep = true;
                const ast::DoubleExp & stepDE = static_cast<const ast::DoubleExp &>(stepE);
                step_d = stepDE.getValue();
                signStep = step_d > 0;
                if (!signStep)
                {
                    step_d = -step_d;
                }
                if (analysis::tools::asInteger(step_d, step_i))
                {
                    integralStep = true;
                }
            }
            else
            {
                // Step is not constant
                // Check if step is zero or not
                const analysis::TIType & ty = stepE.getDecorator().getResult().getType();
                if (ty.isintegral() || ty.isreal())
                {
                    stepE.accept(*this);
                    step = getResult()->loadData(*this);
                }
                else
                {
                    // TODO: error
                    return;
                }
            }

            if (startE.isDoubleExp())
            {
                constantStart = true;
                const ast::DoubleExp & startDE = static_cast<const ast::DoubleExp &>(startE);
                start_d = startDE.getValue();
                if (analysis::tools::asInteger(start_d, start_i))
                {
                    integerIterator = integralStep;
                    if (integerIterator)
                    {
                        start = getConstant(start_i);
                        step = getConstant(step_i);
                    }
                }
                if (!start)
                {
                    start = getConstant(start_d);
                    if (!step)
                    {
                        step = getConstant(step_d);
                    }
                }
            }
            else
            {
                startE.accept(*this);
                start = getResult()->loadData(*this);
                if (!step)
                {
                    step = getConstant(step_d);
                }
            }

            if (endE.isDoubleExp())
            {
                constantEnd = true;
                const ast::DoubleExp & endDE = static_cast<const ast::DoubleExp &>(endE);
                end_d = endDE.getValue();
                if (analysis::tools::asInteger(end_d, end_i))
                {
                    end = getConstant(end_i);
                }
                else
                {
                    end = getConstant(end_d);
                }
            }
            else
            {
                endE.accept(*this);
                end = getResult()->loadData(*this);
            }

            if (le.getDecorator().getResult().getRange().isValid())
            {
                integerIterator = true;
            }

            if (integerIterator)
            {
                start = Cast::cast<int64_t>(start, startTyIsSigned, *this);
                step = Cast::cast<int64_t>(step, stepTyIsSigned, *this);
                end = Cast::cast<int64_t>(end, endTyIsSigned, *this);
                llvm::Value * one = getConstant<int64_t>(1);
                if (constantStep)
                {
                    if (signStep)
                    {
                        end = builder.CreateAdd(end, one);
                    }
                    else
                    {
                        end = builder.CreateSub(end, one);
                    }
                }
            }
            else
            {
                start = Cast::cast<double>(start, startTyIsSigned, *this);
                step = Cast::cast<double>(step, stepTyIsSigned, *this);
                end = Cast::cast<double>(end, endTyIsSigned, *this);
            }

            // integers values: for i = start:step:end...
            // is equivalent to for (int64_t i = start; i < end + 1; i += step)...

            // flaoting values: for i = start:step:end...
            // is equivalent to for (double i = start; i <= end; i += step)...

            llvm::BasicBlock * cond = llvm::BasicBlock::Create(context, "for_cond", function);
            llvm::BasicBlock * loop = llvm::BasicBlock::Create(context, "for_loop", function);
            llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "for_after", function);

            // To know how to break or continue the loop
            blocks.emplace(cond, after);

            llvm::Value * sign_step = nullptr;
            llvm::Value * zero_i64 = getConstant<int64_t>(0);
            llvm::Value * zero_dbl = getConstant<double>(0);
            llvm::Value * abs = llvm::Intrinsic::getDeclaration(&getModule(), llvm::Intrinsic::fabs, getTy<double>());
            llvm::Value * abs_arg[1];
            llvm::Value * cmp_i1 = nullptr;

            if (!constantStep)
            {
                llvm::Value * cmp_zero;
                if (integerIterator)
                {
                    cmp_zero = builder.CreateICmpEQ(step, zero_i64);
                }
                else
                {
                    cmp_zero = builder.CreateFCmpOEQ(step, zero_dbl);
                }
                llvm::BasicBlock * precond = llvm::BasicBlock::Create(context, "", function);
                builder.CreateCondBr(cmp_zero, after, precond);
                builder.SetInsertPoint(precond);
            }

            if (!integerIterator)
            {
                if (!constantStart)
                {
                    abs_arg[0] = start;
                    llvm::CallInst * abs_start = builder.CreateCall(abs, abs_arg);
                    abs_start->setTailCall(true);
                    cmp_i1 = builder.CreateFCmpUEQ(abs_start, getConstant<double>(std::numeric_limits<double>::infinity()));
                    llvm::BasicBlock * bb = llvm::BasicBlock::Create(context, "", function);
                    builder.CreateCondBr(cmp_i1, after, bb);
                    builder.SetInsertPoint(bb);
                }

                if (!constantEnd)
                {
                    abs_arg[0] = end;
                    llvm::CallInst * abs_end = builder.CreateCall(abs, abs_arg);
                    abs_end->setTailCall(true);
                    cmp_i1 = builder.CreateFCmpUEQ(abs_end, getConstant<double>(std::numeric_limits<double>::infinity()));
                    llvm::BasicBlock * bb = llvm::BasicBlock::Create(context, "", function);
                    builder.CreateCondBr(cmp_i1, after, bb);
                    builder.SetInsertPoint(bb);
                }
            }

            if (integerIterator)
            {
                if (constantStep)
                {
                    if (signStep)
                    {
                        cmp_i1 = builder.CreateICmpSLT(start, end);
                    }
                    else
                    {
                        cmp_i1 = builder.CreateICmpSGT(start, end);
                    }
                }
                else
                {
                    sign_step = builder.CreateICmpSGT(step, zero_i64);
                    llvm::Value * cmp1 = builder.CreateICmpSLT(start, end);
                    llvm::Value * cmp2 = builder.CreateICmpSGT(start, end);
                    cmp_i1 = builder.CreateSelect(sign_step, cmp1, cmp2);
                }
            }
            else
            {
                if (constantStep)
                {
                    if (signStep)
                    {
                        cmp_i1 = builder.CreateFCmpOLE(start, end);
                    }
                    else
                    {
                        cmp_i1 = builder.CreateFCmpOGE(start, end);
                    }
                }
                else
                {
                    abs_arg[0] = step;
                    llvm::CallInst * abs_step = builder.CreateCall(abs, abs_arg);
                    abs_step->setTailCall(true);
                    cmp_i1 = builder.CreateFCmpUEQ(abs_step, getConstant<double>(std::numeric_limits<double>::infinity()));
                    llvm::BasicBlock * bb = llvm::BasicBlock::Create(context, "", function);
                    builder.CreateCondBr(cmp_i1, after, bb);
                    builder.SetInsertPoint(bb);
                    sign_step = builder.CreateFCmpOGT(step, zero_dbl);
                    llvm::Value * cmp1 = builder.CreateFCmpOLE(start, end);
                    llvm::Value * cmp2 = builder.CreateFCmpOGE(start, end);
                    cmp_i1 = builder.CreateSelect(sign_step, cmp1, cmp2);
                }
            }

            builder.CreateCondBr(cmp_i1, loop, after);

            llvm::BasicBlock * cur_block = builder.GetInsertBlock();

            builder.SetInsertPoint(loop);
            llvm::PHINode * i;
            if (integerIterator)
            {
                i = builder.CreatePHI(getTy<int64_t>(), 2);
            }
            else
            {
                i = builder.CreatePHI(getTy<double>(), 2);
            }

            i->addIncoming(start, cur_block);
            JITScilabPtr & it = variables.find(symIterator)->second;
            it->storeData(*this, i);

            // Visit the loop body
            e.getBody().accept(*this);

            builder.CreateBr(cond);

            builder.SetInsertPoint(cond);
            llvm::Value * i_step;
            if (integerIterator)
            {
                if (constantStep)
                {
                    if (signStep)
                    {
                        i_step = builder.CreateAdd(i, step);
                        cmp_i1 = builder.CreateICmpSLT(i_step, end);
                    }
                    else
                    {
                        i_step = builder.CreateSub(i, step);
                        cmp_i1 = builder.CreateICmpSGT(i_step, end);
                    }
                }
                else
                {
                    i_step = builder.CreateAdd(i, step);
                    llvm::Value * cmp1 = builder.CreateICmpSLT(i_step, end);
                    llvm::Value * cmp2 = builder.CreateICmpSGT(i_step, end);
                    cmp_i1 = builder.CreateSelect(sign_step, cmp1, cmp2);
                }
            }
            else
            {
                if (constantStep)
                {
                    if (signStep)
                    {
                        i_step = builder.CreateFAdd(i, step);
                        cmp_i1 = builder.CreateFCmpOLE(i_step, end);
                    }
                    else
                    {
                        i_step = builder.CreateFSub(i, step);
                        cmp_i1 = builder.CreateFCmpOGE(i_step, end);
                    }
                }
                else
                {
                    i_step = builder.CreateFAdd(i, step);
                    llvm::Value * cmp1 = builder.CreateFCmpOLE(i_step, end);
                    llvm::Value * cmp2 = builder.CreateFCmpOGE(i_step, end);
                    cmp_i1 = builder.CreateSelect(sign_step, cmp1, cmp2);
                }
            }

            i->addIncoming(i_step, cond);

            builder.CreateCondBr(cmp_i1, loop, after);

            builder.SetInsertPoint(after);
        }
    }
}
}
