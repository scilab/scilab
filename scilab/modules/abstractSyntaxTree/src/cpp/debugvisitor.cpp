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

  static void DEBUG_START_NODE()
  {
    ++level;
  }

  static void DEBUG_END_NODE(void)
  {
    --level;
  }

  static void DEBUG(std::string str, const Exp &e)
  {
    for(int i = 0 ; i < level; ++i)
      {
	std::cerr << "  ";
      }
    if (level > 0)
      {
	std::cerr << "|_./ ";
      }
    std::cerr << str;
    
    Location loc = e.location_get();
    std::cerr << " @(" << loc.first_line << "." << loc.first_column << " -> ";
    std::cerr << loc.last_line << "." << loc.last_column << ")" << std::endl;
  }


  void DebugVisitor::visit (const MatrixExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec MatrixExp", e);
    std::list<MatrixLineExp *>::const_iterator	i;
    for (i = e.lines_get().begin() ; i != e.lines_get().end() ; ++i )
      {
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const MatrixLineExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec MatrixLineExp", e);
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
    DEBUG_START_NODE();
    DEBUG("Exec CellExp", e);
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
    DEBUG_START_NODE();
    DEBUG("Exec StringExp : "+e.value_get(), e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const CommentExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec CommentExp : "+e.comment_get(), e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const IntExp  &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec IntExp : "+e.value_get(), e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const FloatExp  &e)
  {
    DEBUG_START_NODE();
    std::ostringstream stream;
    stream << e.value_get();
    DEBUG("Exec FloatExp : "+stream.str(), e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const DoubleExp  &e)
  {
    DEBUG_START_NODE();
    std::ostringstream stream;
    stream << e.value_get();
    DEBUG("Exec DoubleExp : "+stream.str(), e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const BoolExp  &e)
  {
    DEBUG_START_NODE();
    std::ostringstream stream;
    stream << e.value_get();
    DEBUG("Exec BoolExp : "+stream.str(), e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const NilExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec NilExp", e);
    DEBUG_END_NODE();
  }
  /** \} */

  /** \name Visit Variable related nodes.
   ** \{ */
  void DebugVisitor::visit (const SimpleVar &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec SimpleVar : "+e.name_get().name_get(), e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ColonVar &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec ColonVar", e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const DollarVar &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec DollarVar", e);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ArrayListVar &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec ArrayListVar", e);
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
    DEBUG_START_NODE();
    DEBUG("Exec FieldExp", e);
    // FIXME
    {
      e.head_get()->accept(*this);
      e.tail_get()->accept(*this);
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit(const OpExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec OpExp", e);
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
    DEBUG_START_NODE();
    DEBUG("Exec LogicalOpExp", e);
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
    DEBUG_START_NODE();
    DEBUG("Exec AssignExp", e);
    //FIXME
    {
      e.left_exp_get().accept (*this);
      e.right_exp_get().accept (*this);
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit(const CallExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec CallExp", e);
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
    DEBUG_START_NODE();
    DEBUG("Exec IfExp", e);
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
    DEBUG_START_NODE();
    DEBUG("Exec TryCatchExp", e);
    // FIXME
    {
      e.try_get ().accept(*this);
      e.catch_get ().accept(*this);
    }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const WhileExp  &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec WhileExp", e);
    // FIMXE
      e.test_get().accept (*this);
      e.body_get().accept (*this);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ForExp  &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec ForExp", e);
    e.vardec_get().accept(*this);
    e.body_get().accept (*this);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const BreakExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec BreakExp", e);
    // FIXME
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ReturnExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec ReturnExp", e);
      if (!e.is_global())
      {
	e.exp_get().accept(*this);
      }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const SeqExp  &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec SeqExp", e);
    std::list<Exp *>::const_iterator	i;
    for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
      {
	if(!(*i)->is_verbose())
	  {
	    std::cerr << "__MUTE__" << std::endl;
	  }
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const ArrayListExp  &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec ArrayListExp", e);
    std::list<Exp *>::const_iterator	i;
    for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
      {
	(*i)->accept (*this);
      }
    DEBUG_END_NODE();
  }

 void DebugVisitor::visit (const AssignListExp  &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec AssignListExp", e);
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
    DEBUG_START_NODE();
    DEBUG("Exec NotExp", e);
    e.exp_get().accept (*this);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const TransposeExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec TransposeExp", e);
    e.exp_get().accept (*this);
    DEBUG_END_NODE();
  }
  /** \} */

  /** \name Visit Declaration nodes.
   ** \{ */
  /** \brief Visit Var declarations. */
  void DebugVisitor::visit (const VarDec  &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec VarDec", e);
    {
      DEBUG_START_NODE();
      DEBUG("Exec Symbol : "+e.name_get().name_get(), e);
      DEBUG_END_NODE();
    }
    e.init_get().accept(*this);
    DEBUG_END_NODE();
  }

  void DebugVisitor::visit (const FunctionDec  &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec FunctionDec", e);
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
  
  void DebugVisitor::visit (const ClassDec &e)
  {
    const slots_t&           slots = e.slots_get();
    slots_t::const_iterator  slot;
    
    DEBUG_START_NODE();
    DEBUG("Exec ClassDec");
    {
      DEBUG_START_NODE();
      DEBUG("Name: "+e.name_get()->name_get());
      DEBUG_END_NODE();
    }
    if (e.super_get()) {
      DEBUG_START_NODE();
      DEBUG("SuperClassName: "+e.super_get()->name_get());
      DEBUG_END_NODE();
    }
    for (slot = slots.begin(); slot != slots.end(); ++slot) {
      (*slot)->accept(*this);
    }
    DEBUG_END_NODE();
  }
  
  void DebugVisitor::visit (const PropertyDec &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec PropertyDec");
    {
      DEBUG_START_NODE();
      DEBUG("Name: "+e.name_get().name_get());
      DEBUG_END_NODE();
    }
    {
      DEBUG_START_NODE();
      DEBUG("Slot attributes:");
      visit(e.attributes_get());
      DEBUG_END_NODE();
    }
    if (e.default_get()) {
      e.default_get()->accept(*this);
    }
    DEBUG_END_NODE();
  }
  
  void DebugVisitor::visit (const MethodDec &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec MethodDec");
    DEBUG_START_NODE();
    {
      DEBUG("Name: "+e.name_get().name_get());
    }
    {
      DEBUG("Slot attributes:");
      DEBUG_START_NODE();
      visit(e.attributes_get());
      DEBUG_END_NODE();
    }
    {
      DEBUG("Arguments:");
      DEBUG_START_NODE();
      visit(e.args_get());
      DEBUG_END_NODE();
    }
    {
      DEBUG("Returns:");
      DEBUG_START_NODE();
      visit(e.returns_get());
      DEBUG_END_NODE();
    }
    DEBUG_END_NODE();
    {
      e.body_get().accept(*this);
    }
    DEBUG_END_NODE();
  }
  /** \} */

  /** \name Visit Type dedicated Expressions related node.
   ** \{ */
  void DebugVisitor::visit(const ListExp &e)
  {
    DEBUG_START_NODE();
    DEBUG("Exec ListExp", e);
    e.start_get().accept(*this);
    e.step_get().accept(*this);
    e.end_get().accept(*this);
    DEBUG_END_NODE();
  }
  /** \} */
}
