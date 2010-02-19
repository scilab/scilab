/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "debugvisitor.hxx"

namespace ast
{
  static int level = -1;

  static void DEBUG_START_NODE(const Exp &e)
  {
    Location loc = e.location_get();
    std::cout << loc.first_line << "." << loc.first_column << std::endl;
    std::cout << loc.last_line << "." << loc.last_column << std::endl;
    ++level;
  }

  static void DEBUG_END_NODE(void)
  {
    --level;
  }

  static void DEBUG(std::string str)
  {
    for(int i = 0 ; i < level; ++i)
      {
	std::cerr << "  ";
      }
    if (level > 0)
      {
	std::cerr << "|_./ ";
      }
    std::cerr << str << std::endl;
  }


  void DebugVisitor::visit (const MatrixExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec MatrixExp");
    std::list<MatrixLineExp *>::const_iterator	i;
    for (i = e.lines_get().begin() ; i != e.lines_get().end() ; ++i )
      {
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const MatrixLineExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec MatrixLineExp");
    std::list<Exp *>::const_iterator	i;
    for (i = e.columns_get().begin() ; i != e.columns_get().end() ; ++i)
      {
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }
  /** \} */

  void DebugVisitor::visit (const CellExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec CellExp");
    std::list<MatrixLineExp *>::const_iterator	i;
    for (i = e.lines_get().begin() ; i != e.lines_get().end() ; ++i )
      {
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }

  /** \} */

  /** \name Visit Constant Expressions nodes.
   ** \{ */
  void DebugVisitor::visit (const StringExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec StringExp : "+e.value_get());
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const CommentExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec CommentExp : "+e.comment_get());
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const IntExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec IntExp : "+e.value_get());
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const FloatExp  &e)
  {
    DEBUG_START_NODE(e);
    std::ostringstream stream;
    stream << e.value_get();
    DEBUG("Exec FloatExp : "+stream.str());
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const DoubleExp  &e)
  {
    DEBUG_START_NODE(e);
    std::ostringstream stream;
    stream << e.value_get();
    DEBUG("Exec DoubleExp : "+stream.str());
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const BoolExp  &e)
  {
    DEBUG_START_NODE(e);
    std::ostringstream stream;
    stream << e.value_get();
    DEBUG("Exec BoolExp : "+stream.str());
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const NilExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec NilExp");
    DEBUG_END_NODE();
  }
  /** \} */

  /** \name Visit Variable related nodes.
   ** \{ */
  void DebugVisitor::visit (const SimpleVar &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec SimpleVar : "+e.name_get().name_get());
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ColonVar &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec ColonVar");
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const DollarVar &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec DollarVar");
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ArrayListVar &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec ArrayListVar");
    std::list<Var *>::const_iterator	i;
    for (i = e.vars_get().begin() ; i != e.vars_get().end() ; ++i)
      {
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }
  /** \} */

  /** \name Visit Control Expressions or Instructions nodes.
   ** \{ */

  void DebugVisitor::visit (const FieldExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec FieldExp");
    // FIXME
    {
      e.head_get()->accept(*this);
      e.tail_get()->accept(*this);
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit(const OpExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec OpExp");
    // FIXME
    {
      e.left_get().accept(*this);
      //e.oper_get();
      e.right_get().accept(*this);
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit(const LogicalOpExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec LogicalOpExp");
    // FIXME
    {
      e.left_get().accept(*this);
      //e.oper_get();
      e.right_get().accept(*this);
    }
    DEBUG_END_NODE();
  }

	void DebugVisitor::visit (const AssignExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec AssignExp");
    //FIXME
    {
      e.left_exp_get().accept (*this);
      e.right_exp_get().accept (*this);
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit(const CallExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec CallExp");
    e.name_get().accept (*this);
    // FIXME
    {
      std::list<Exp *>::const_iterator	i;

      for (i = e.args_get().begin (); i != e.args_get().end (); ++i)
	{
	  (*i)->accept (*this);
	}
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const IfExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec IfExp");
    // FIXME
    {
      e.test_get ().accept(*this);
      e.then_get ().accept(*this);
      if (e.has_else())
	{
	  e.else_get ().accept(*this);
	}
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const TryCatchExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec TryCatchExp");
    // FIXME
    {
      e.try_get ().accept(*this);
      e.catch_get ().accept(*this);
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const WhileExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec WhileExp");
    // FIMXE
      e.test_get().accept (*this);
      e.body_get().accept (*this);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ForExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec ForExp");
    e.vardec_get().accept(*this);
    e.body_get().accept (*this);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const BreakExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec BreakExp");
    // FIXME
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ReturnExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec ReturnExp");
    // FIXME
    /*
      if (!e.is_global())
      {
      e.exp_get().accept(*this);
      }
    */
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const SeqExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec SeqExp");
    std::list<Exp *>::const_iterator	i;
    for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
      {
	if(!(*i)->is_verbose())
	  {
	    DEBUG("__MUTE__");
	  }
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ArrayListExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec ArrayListExp");
    std::list<Exp *>::const_iterator	i;
    for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
      {
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }

 void DebugVisitor::visit (const AssignListExp  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec AssignListExp");
    std::list<Exp *>::const_iterator	i;
    for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
      {
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }
  /** \} */

  /** \name Visit Single Operation nodes.
   ** \{ */
  void DebugVisitor::visit (const NotExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec NotExp");
    e.exp_get().accept (*this);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const TransposeExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec TransposeExp");
    e.exp_get().accept (*this);
    DEBUG_END_NODE();
  }
  /** \} */

  /** \name Visit Declaration nodes.
   ** \{ */
  /** \brief Visit Var declarations. */
  void DebugVisitor::visit (const VarDec  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec VarDec");
    {
      DEBUG_START_NODE(e);
      DEBUG("Exec Symbol : "+e.name_get().name_get());
      DEBUG_END_NODE();
    }
    e.init_get().accept(*this);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const FunctionDec  &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec FunctionDec");
    // FIXME

    // First ask if there are some return values.
    //visit(e.returns_get());

    // Then get the function name
    //visit(e.name_get());

    // Then get function args
    //visit(e.args_get());

    // Now debug function body
    e.body_get().accept(*this);

    DEBUG_END_NODE();
  }
  /** \} */

  /** \name Visit Type dedicated Expressions related node.
   ** \{ */
  void DebugVisitor::visit(const ListExp &e)
  {
    DEBUG_START_NODE(e);
    DEBUG("Exec ListExp");
    e.start_get().accept(*this);
    e.step_get().accept(*this);
    e.end_get().accept(*this);
    DEBUG_END_NODE();
  }
  /** \} */
}
