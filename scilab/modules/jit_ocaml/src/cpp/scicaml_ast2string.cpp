/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2013 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "jit_ocaml.hxx"

/* TODO: call the destructor of the C++ AST when it becomes useless */

ast::Exp* ast_saved = NULL;

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include "location.hxx"

#include "jit_ocaml.hxx"
#include "visitor_common.hxx"
//#include "runvisitor.hxx"
//#include "execvisitor.hxx"
//#include "timedvisitor.hxx"
#include "shortcutvisitor.hxx"
#include "printvisitor.hxx"
#include "mutevisitor.hxx"

// Needed by visitprivate(const OpExp &)
// Needed by visitprivate(const LogicalOpExp &)
#include "generic_operations.hxx"
#include "types_bitwiseOp.hxx"
#include "configvariable.hxx"
#include "overload.hxx"
#include "scilabexception.hxx"

#include "matrix_transpose_int.hxx"

extern "C" {
#include "doublecomplex.h"
#include "matrix_transpose.h"
#include "os_swprintf.h"
#include "more.h"
    //#include "HandleManagement.h"
}

#include "timer.hxx"
#include "localization.h"

#include "scilabWrite.hxx"
#include "context.hxx"

#include "all.hxx"
#include "types.hxx"
#include "alltypes.hxx"

// FIXME : remove those using
using namespace types;

namespace ast
{
class SerializerVisitor : public ConstVisitor
{
public:
    SerializerVisitor()
    {
      buf = NULL;
      buflen = 0;
      bufsize = 0;
    }

    ~SerializerVisitor()
    {
      // if we free [buf] now, it is not available for OCaml...
      // free(buf);
      //      std::cerr << "~SerializerVisitor" << std::endl;
      buf = NULL;
      bufsize = 0;
      buflen = 0;
    }

public:
  char *get_buf(void){ 
    need(1);
    set_uint32(0, buflen);
    // std::cerr << "get_buf :" << buflen << std::endl;
    //    fprintf(stderr, "%d %d %d %d\n", buf[0],buf[1],buf[2],buf[3]);
    return buf; 
  }

  void add_location(const Location* loc)
  {
    add_uint32(loc->first_line);
    add_uint32(loc->first_column);
    add_uint32(loc->last_line);
    add_uint32(loc->last_column);    
  }
  void add_ast(unsigned int code, const ast::Exp* e)
  {
    //    std::cerr << "add_ast at " << buflen << " code :" << code << std::endl;
    Location loc = e->location_get();
    add_uint8(code);
    add_location(&e->location_get());
    add_uint8(e->is_verbose());
    add_uint8(e->is_break());
    add_uint8(e->is_breakable());
    add_uint8(e->is_return());
    add_uint8(e->is_returnable());
    add_uint8(e->is_continue());
    add_uint8(e->is_continuable());
  }

  /* ensure that we have [size] bytes in the buffer */
  void need(int size)
  {
    //    std::cerr << "need " << size << std::endl;
    if( bufsize - buflen < size ){
      bufsize = 2 * bufsize + size + 2048;
      char *newbuf = (char*) malloc(bufsize);
      //      std::cerr << "need : malloc " << bufsize << std::endl;
      if( buflen > 0 )
	memcpy(newbuf, buf, buflen);
      if( buf != NULL)
	free(buf);
      else 
	buflen = 4; /* space for the size of the string */
      buf = newbuf;
    }
  }

  void add_uint8(unsigned int n)
  {
    need(1);
    buf[buflen++] = n;
  }

  void add_uint32(unsigned int n)
  {
    need(4);
    buf[buflen++] = (n & 0xff); n >>= 8;
    buf[buflen++] = (n & 0xff); n >>= 8;
    buf[buflen++] = (n & 0xff); n >>= 8;
    buf[buflen++] = (n & 0xff);
  }

  void add_double(double d)
  {
    need(8);
    *(double*)(buf + buflen) = d;
    buflen += 8;
  }

  void set_uint32(unsigned int pos, unsigned int n)
  {
    //     std::cerr << "set_uint32 : n " << n << std::endl;
     buf[pos++] = (n & 0xff); n >>= 8;
     //     std::cerr << "set_uint32 : n " << n << std::endl;
    buf[pos++] = (n & 0xff); n >>= 8;
    //     std::cerr << "set_uint32 : n " << n << std::endl;
    buf[pos++] = (n & 0xff); n >>= 8;
    //     std::cerr << "set_uint32 : n " << n << std::endl;
    buf[pos++] = (n & 0xff);
  }

  void add_wstring(const std::wstring &w)
  {
    int size = w.size();
    const wchar_t *c_str = w.c_str();
    int final_size = size * sizeof(wchar_t);
    add_uint32(final_size);
    need(final_size);
    memcpy(buf + buflen, c_str, final_size);
    buflen += final_size;
  }

  int get_pos()
  { return buflen; }

    /*-------------.
    | Attributes.  |
    `-------------*/
protected:
   char *buf;
   int buflen;
   int bufsize;
};

template <class T>
class SerializeVisitorT : public SerializerVisitor
{
public :
    SerializeVisitorT() : SerializerVisitor()
    {
    }

public :

  void add_exps(const std::list<Exp *> exps)
  {
    int current_pos = get_pos();
    int nitems = 0;
    add_uint32(0);
    std::list<Exp *>::const_iterator it;
    for(it = exps.begin() ; it != exps.end() ; it++)
      {
	(*it)->accept(*this);
	nitems ++;
      }
    set_uint32(current_pos, nitems);
  }

  void add_vars(const ast::ArrayListVar* var)
  {
    int current_pos = get_pos();
    int nitems = 0;
    add_uint32(0);
    std::list<Var *>::const_iterator it;
    for(it = var->vars_get().begin() ; it != var->vars_get().end() ; it++)
      {
	(*it)->accept(*this);
	nitems ++;
      }
    set_uint32(current_pos, nitems);
  }

  void add_Symbol(const symbol::Symbol* e)
  {
    add_wstring(e->name_get());
  }

  void add_exp(const ast::Exp *e)
  {
    e->accept(*this);
  }

  void add_IfExp_Kind(ast::IfExp::Kind kind)
  {
    int code = 255;

    switch(kind){
    case ast::IfExp::invalid_kind : code = (1); break;
    case ast::IfExp::instruction_kind : code = (2); break;
    case ast::IfExp::expression_kind : code = (3); break;
    default : 
      //      std::cerr << "add_IfExp_Kind : " << kind << std::endl;
      code = (1); break;
    }
    add_uint8(code);
  }

  void add_OpExp_Kind(ast::OpExp::Kind kind)
  {
    int code = 254;

    switch(kind){
    case ast::OpExp::invalid_kind : code = (1); break;
    case ast::OpExp::bool_kind : code = (2); break;
    case ast::OpExp::string_kind : code = (3); break;
    case ast::OpExp::integer_kind : code = (4); break;
    case ast::OpExp::float_kind : code = (5); break;
    case ast::OpExp::double_kind : code = (6); break;
    case ast::OpExp::float_complex_kind : code = (7); break;
    case ast::OpExp::double_complex_kind : code = (8); break;
      
    case ast::OpExp::bool_matrix_kind : code = (9); break;
    case ast::OpExp::string_matrix_kind : code = (10); break;
    case ast::OpExp::integer_matrix_kind : code = (11); break;
    case ast::OpExp::float_matrix_kind : code = (12); break;
    case ast::OpExp::double_matrix_kind : code = (13); break;
    case ast::OpExp::float_complex_matrix_kind : code = (14); break;
    case ast::OpExp::double_complex_matrix_kind : code = (15); break;
      
    case ast::OpExp::matrix_kind : code = (16); break;
     }
    add_uint8(code);
  }

  void add_OpExp_Oper(const ast::OpExp::Oper oper)
  {
    int code = 253;
    switch(oper){

    case ast::OpExp::plus : code = (1); break;
    case ast::OpExp::minus: code = (2); break;
    case ast::OpExp::times: code = (3); break;
    case ast::OpExp::rdivide: code = (4); break;
    case ast::OpExp::ldivide: code = (5); break;
    case ast::OpExp::power: code = (6); break;

    case ast::OpExp::dottimes: code = (7); break;
    case ast::OpExp::dotrdivide: code = (8); break;
    case ast::OpExp::dotldivide: code = (9); break;
    case ast::OpExp::dotpower: code = (10); break;

    case ast::OpExp::krontimes: code = (11); break;
    case ast::OpExp::kronrdivide: code = (12); break;
    case ast::OpExp::kronldivide: code = (13); break;

    case ast::OpExp::controltimes: code = (14); break;
    case ast::OpExp::controlrdivide: code = (15); break;
    case ast::OpExp::controlldivide: code = (16); break;

    case ast::OpExp::eq: code = (17); break;
    case ast::OpExp::ne: code = (18); break;
    case ast::OpExp::lt: code = (19); break;
    case ast::OpExp::le: code = (20); break;
    case ast::OpExp::gt: code = (21); break;
    case ast::OpExp::ge: code = (22); break;

    case ast::OpExp::unaryMinus: code = (23); break;

    case ast::OpExp::logicalAnd: code = (24); break;
    case ast::OpExp::logicalOr: code = (25); break;
    case ast::OpExp::logicalShortCutAnd: code = (26); break;
    case ast::OpExp::logicalShortCutOr: code = (27); break;
    }
    add_uint8(code);
  }

  void add_IntExp_Prec(const ast::IntExp::Prec prec)
  {
    int code = 251;
    switch(prec){
    case ast::IntExp::_8_ : code = (1); break;
    case ast::IntExp::_16_: code = (2); break;
    case ast::IntExp::_32_: code = (3); break;
    case ast::IntExp::_64_: code = (4); break;
    }
    add_uint8(code);
  }

  void add_VarDec_Kind(const ast::VarDec::Kind kind)
  {
    int code = 250;
    switch(kind){
    case ast::VarDec::invalid_kind : code = (1); break;
    case ast::VarDec::evaluation_kind: code = (2); break;
    case ast::VarDec::assignment_kind: code = (3); break;
    }
    add_uint8(code);
  }

  void add_TransposeExp_Kind(const ast::TransposeExp::Kind kind)
  {
    int code = 249;
    switch(kind){
    case ast::TransposeExp::_Conjugate_ : code = (1); break;
    case ast::TransposeExp::_NonConjugate_: code = (2); break;
    }
    add_uint8(code);
  }

  void add_bool(bool b)
  {
    add_uint8(b);
  }

  void add_varDec(const ast::VarDec* varDec)
  {
    add_Symbol(& varDec->name_get());
    add_VarDec_Kind(varDec->kind_get());
    add_exp(& varDec->init_get());
  }

  void add_MatrixLines(const  std::list<ast::MatrixLineExp*> *lines){
    int current_pos = get_pos();
    int nitems = 0;
    add_uint32(0);
    std::list<MatrixLineExp *>::const_iterator it;
    for(it = lines->begin() ; it != lines->end() ; it++)
      {
	add_location(& (*it)->location_get());
	add_exps((*it)->columns_get());
	nitems ++;
      }
    set_uint32(current_pos, nitems);
  }

  void visitprivate_SeqExp(const SeqExp *e){ /* done */
    add_ast(1,e);
    add_exps(e->exps_get());
  }
  void visitprivate_StringExp(const StringExp *e){ /* done */
    add_ast(2,e);
    add_wstring(e->value_get());
  }
  void visitprivate_CommentExp(const CommentExp *e){ /* done */
    add_ast(3,e);
    add_wstring(e->comment_get());
  }
  void visitprivate_IntExp(const IntExp *e){ /* done */
    add_ast(4,e);
    add_IntExp_Prec(e->prec_get());
    add_uint32(e->value_get());
  }
  void visitprivate_FloatExp(const FloatExp *e){  /* done */
    add_ast(5,e);
    add_double(e->value_get());
  }
  void visitprivate_DoubleExp(const DoubleExp *e){ /* done */
    add_ast(6,e);
    add_double(e->value_get());
  }
  void visitprivate_BoolExp(const BoolExp *e){ /* done */
    add_ast(7,e);
    add_bool(e->value_get());
  }
  void visitprivate_NilExp(const NilExp *e){ /* done */
    add_ast(8,e);
  }
  void visitprivate_SimpleVar(const SimpleVar *e){ /* done */
    add_ast(9,e);
    add_Symbol(& e->name_get());
  }
  void visitprivate_ColonVar(const ColonVar *e){ /* done */
    add_ast(10,e);
  }
  void visitprivate_DollarVar(const DollarVar *e){ /* done */
    add_ast(11,e);
  }
  void visitprivate_ArrayListVar(const ArrayListVar *e){ /* done */
    add_ast(12,e);
    add_vars(e);
  }
  void visitprivate_FieldExp(const FieldExp *e){ /* done */
    add_ast(13,e);
    add_exp(e->head_get());
    add_exp(e->tail_get());
  }
  void visitprivate_IfExp(const IfExp *e){ /* done */
    add_ast(14,e);
    add_IfExp_Kind(e->kind_get());
    bool has_else = e->has_else();
    add_bool(has_else);
    add_exp(& e->test_get());
    add_exp(& e->then_get());
    if( has_else ) add_exp(& e->else_get());
  }
  void visitprivate_TryCatchExp(const TryCatchExp *e){ /* done */
    add_ast(15,e);
    add_location(& e->try_get().location_get());
    add_location(& e->catch_get().location_get());
    add_exps(e->try_get().exps_get());
    add_exps(e->catch_get().exps_get());
  }
  void visitprivate_WhileExp(const WhileExp *e){ /* done */
    add_ast(16,e);
    add_exp(& e->test_get());
    add_exp(& e->body_get());
  }
  void visitprivate_ForExp(const ForExp *e){  /* done */
    add_ast(17,e);
    add_location(& e->vardec_get().location_get());
    add_varDec(& e->vardec_get());
    add_exp(& e->body_get());
  }
  void visitprivate_BreakExp(const BreakExp *e){ /* done */
    add_ast(18,e);
  }
  void visitprivate_ContinueExp(const ContinueExp *e){ /* done */
    add_ast(19,e);
  }
  void visitprivate_ReturnExp(const ReturnExp *e){ /* done */
    add_ast(20,e);
    bool is_global = e->is_global();
    add_bool(is_global);
    if( !is_global ) /* otherwise exp is NULL */
      add_exp(& e->exp_get());
  }
  void visitprivate_SelectExp(const SelectExp *e){
    add_ast(21,e);
    ast::SeqExp *default_case = e->default_case_get();
    bool has_default = default_case != NULL;
    add_bool( has_default );
    if( has_default ) {
      add_location(& default_case->location_get());
      add_exps(default_case->exps_get());
    }
    add_exp(e->select_get());

    int current_pos = get_pos();
    int nitems = 0;
    add_uint32(0);
    cases_t::const_iterator it;
    for(it = e->cases_get()->begin() ; it != e->cases_get()->end() ; it++)
            {
	      const ast::CaseExp *ce = *it;
	      nitems ++;
	      add_location(& ce->location_get() );
	      add_location(& ce->body_get()->location_get() );
	      add_exp( ce->test_get() );
	      add_exps( ce->body_get()->exps_get() );
            }
    set_uint32(current_pos, nitems);
  }
  void visitprivate_CaseExp(const CaseExp *e){ /* SHOULD NEVER HAPPEN */
    add_ast(22,e);
  }
  void visitprivate_CellExp(const CellExp *e){ /* done */
    add_ast(23,e);
    add_MatrixLines(& e->lines_get());
  }
  void visitprivate_ArrayListExp(const ArrayListExp *e){ /* done */
    add_ast(24,e);
    add_exps(e->exps_get());
  }
  void visitprivate_AssignListExp(const AssignListExp *e){ /* done */
    add_ast(25,e);
    add_exps(e->exps_get());
  }
  void visitprivate_NotExp(const NotExp *e){ /* done */
    add_ast(26,e);
    add_exp(& e->exp_get() );
  }
  void visitprivate_TransposeExp(const TransposeExp *e){ /* done */
    add_ast(27,e);
    add_TransposeExp_Kind(e->conjugate_get());
    add_exp(& e->exp_get());
  }
  void visitprivate_VarDec(const VarDec *e){
    add_ast(28,e);
    add_varDec(e);
  }
  void visitprivate_FunctionDec(const FunctionDec *e){ /* done */
    add_ast(29,e);
    add_Symbol(& e->name_get());    
    add_location(& e->args_get().location_get());
    add_location(& e->returns_get().location_get());
    add_exp(& e->body_get());
    add_vars(& e->args_get());
    add_vars(& e->returns_get());
  }
  void visitprivate_ListExp(const ListExp *e){ /* done */
    add_ast(30,e);
    add_exp(& e->start_get() );
    add_exp(& e->step_get() );
    add_exp(& e->end_get() );
  }
  void visitprivate_AssignExp(const AssignExp *e){
    add_ast(31,e);
    add_exp(& e->left_exp_get());
    add_exp(& e->right_exp_get());
    /* TODO: e->right_val_get() - initialized during execution */
  }
  void visitprivate_OpExp(const OpExp *e){ /* done */
    add_ast(32,e);    
    add_OpExp_Kind(e->kind_get());
    add_OpExp_Oper(e->oper_get());
    e->left_get().accept(*this);
    e->right_get().accept(*this);
  }
  void visitprivate_LogicalOpExp(const LogicalOpExp *e){ /* done */
    add_ast(33,e);
    add_OpExp_Kind(e->kind_get());
    add_OpExp_Oper(e->oper_get());
    e->left_get().accept(*this);
    e->right_get().accept(*this);
  }
  void visitprivate_MatrixExp(const MatrixExp *e) /* done */
  {
    add_ast(34,e);
    add_MatrixLines(& e->lines_get());
  }
  void visitprivate_CallExp(const CallExp *e){ /* done */
    add_ast(35,e);
    add_exp(& e->name_get());
    add_exps(e->args_get());
  }
  void visitprivate_MatrixLineExp(const MatrixLineExp *e){ /* SHOULD NEVER HAPPEN */
    add_ast(36,e);
  }
  void visitprivate_CellCallExp(const CellCallExp *e){ /* done */
    add_ast(37,e);
    add_exp(& e->name_get());
    add_exps(e->args_get());
  }

};

class SerializeVisitor : public SerializeVisitorT<SerializeVisitor>
{
  void visit (const SeqExp  &e)
  {
    visitprivate_SeqExp(&e);
  }
  
  void visit (const MatrixExp &e)
  {
    visitprivate_MatrixExp(&e);
  }
  
  void visit (const MatrixLineExp &e)
  {
    visitprivate_MatrixLineExp(&e);
  }
  
  void visit (const CellExp &e)
  {
    visitprivate_CellExp(&e);
  }
  
  void visit (const StringExp &e)
  {
    visitprivate_StringExp(&e);
  }
  
  void visit (const CommentExp &e)
  {
    visitprivate_CommentExp(&e);
  }
  
  void visit (const IntExp &e)
  {
    visitprivate_IntExp(&e);
  }
  
  void visit (const FloatExp &e)
  {
    visitprivate_FloatExp(&e);
  }
  
  void visit (const DoubleExp &e)
  {
    visitprivate_DoubleExp(&e);
  }
  
  void visit (const BoolExp &e)
  {
    visitprivate_BoolExp(&e);
  }
  
  void visit (const NilExp &e)
  {
    visitprivate_NilExp(&e);
  }
  
  void visit (const SimpleVar &e)
  {
    visitprivate_SimpleVar(&e);
  }
  
  void visit (const ColonVar &e)
  {
    visitprivate_ColonVar(&e);
  }
  
  void visit (const DollarVar &e)
  {
    visitprivate_DollarVar(&e);
  }
  
  void visit (const ArrayListVar &e)
  {
    visitprivate_ArrayListVar(&e);
  }
  
  void visit (const FieldExp &e)
  {
    visitprivate_FieldExp(&e);
  }
  
  void visit (const OpExp &e)
  {
    visitprivate_OpExp(&e);
  }
  
  void visit (const LogicalOpExp &e)
  {
    visitprivate_LogicalOpExp(&e);
  }
  
  void visit (const AssignExp &e)
  {
    visitprivate_AssignExp(&e);
  }
  
  void visit (const CellCallExp &e)
  {
    visitprivate_CellCallExp(&e);
  }
  
  void visit (const CallExp &e)
  {
    visitprivate_CallExp(&e);
  }

  void visit (const IfExp &e)
  {
    visitprivate_IfExp(&e);
  }
  
  void visit (const TryCatchExp &e)
  {
    visitprivate_TryCatchExp(&e);
  }
  
  void visit (const WhileExp &e)
  {
    visitprivate_WhileExp(&e);
  }
  
  void visit (const ForExp &e)
  {
    visitprivate_ForExp(&e);
  }
  
  void visit (const BreakExp &e)
  {
    visitprivate_BreakExp(&e);
  }
  
  void visit (const ContinueExp &e)
  {
    visitprivate_ContinueExp(&e);
  }
  
  void visit (const ReturnExp &e)
  {
    visitprivate_ReturnExp(&e);
  }
  
  void visit (const SelectExp &e)
  {
    visitprivate_SelectExp(&e);
  }
  
  void visit (const CaseExp &e)
  {
    visitprivate_CaseExp(&e);
  }
  
  void visit (const ArrayListExp &e)
  {
    visitprivate_ArrayListExp(&e);
  }
  
  void visit (const AssignListExp &e)
  {
    visitprivate_AssignListExp(&e);
  }
  
  void visit (const NotExp &e)
  {
    visitprivate_NotExp(&e);
  }
  
  void visit (const TransposeExp &e)
  {
    visitprivate_TransposeExp(&e);
  }
  
  void visit (const VarDec &e)
  {
    visitprivate_VarDec(&e);
  }
  
  void visit (const FunctionDec &e)
  {
    visitprivate_FunctionDec(&e);
  }
  
  void visit(const ListExp &e)
  {
    visitprivate_ListExp(&e);
  }
};

}



char* scicaml_ast2string(ast::Exp* ast)
{
  // std::cerr << "scicaml_ast2string" << std::endl;

  ast::SerializeVisitor visitor;

  ast->accept(visitor);
  ast_saved = ast;
  return visitor.get_buf();
}
