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

#include <stdlib.h>
#include "jit_ocaml.hxx"

#include <all.hxx>
#include <commentexp.hxx>

extern ast::Exp* ast_saved;

unsigned char *initial_buf;
unsigned char *buf;

static unsigned int get_uint32(void)
{
  unsigned int c0 = *buf++;
  unsigned int c1 = *buf++;
  unsigned int c2 = *buf++;
  unsigned int c3 = *buf++;
  //  std::cerr << "c0 = " << c0 << std::endl;
  //  std::cerr << "c1 = " << c1 << std::endl;
  //  std::cerr << "c2 = " << c2 << std::endl;
  //  std::cerr << "c3 = " << c3 << std::endl;
  return c0 + ((c1 + ((c2 + (c3 << 8)) << 8 )) << 8 );
}

static int get_int32(void)
{
  return (int)get_uint32();
}

static unsigned int get_uint8(void)
{
  return *buf++;
}

static bool get_bool(void)
{
  return *buf++;
}

Location *get_location(void)
{
  Location *loc = new Location();
  loc->first_line = get_uint32();
  loc->first_column = get_uint32();
  loc->last_line = get_uint32();
  loc->last_column = get_uint32();
  return loc;
}

static ast::Exp* get_exp(void);

static std::list<ast::Exp*>* get_exps(void)
{
  int nitems = get_uint32();
  std::list<ast::Exp*> *list = new  std::list<ast::Exp*>;
  for(int i = 0; i < nitems; i++){
    ast::Exp* exp = get_exp();
    list->push_back(exp);
  }
  return list;
}

static std::list<ast::MatrixLineExp*>* get_MatrixLines(void)
{
  int nitems = get_uint32();
  std::list<ast::MatrixLineExp*> *list = new  std::list<ast::MatrixLineExp*>;
  for(int i = 0; i < nitems; i++){
    Location *line_loc = get_location();
    std::list<ast::Exp*>* columns = get_exps();
    ast::MatrixLineExp* line = new ast::MatrixLineExp(*line_loc,* columns);
    list->push_back(line);
  }
  return list;
}

static std::list<ast::Var*>* get_vars(void)
{
  int nitems = get_uint32();
  std::list<ast::Var*> *list = new  std::list<ast::Var*>;
  for(int i = 0; i < nitems; i++){
    ast::Var* var = dynamic_cast<ast::Var*>(get_exp());
    list->push_back(var);
  }
  return list;
}

static ast::VarDec::Kind get_VarDec_Kind(void)
{
  int code = get_uint8();
  switch(code){
  case 1 : return ast::VarDec::invalid_kind;
  case 2 : return ast::VarDec::evaluation_kind;
  case 3 : return ast::VarDec::assignment_kind;
  }
  std::cerr << "Unknown get_VarDec_Kind code " << code << std::endl;
  exit(2);
}

static ast::OpExp::Kind get_OpExp_Kind(void)
{
  int code = get_uint8();
  switch(code){
  case 1 : return ast::OpExp::invalid_kind;
  case 2 : return ast::OpExp::bool_kind;
  case 3 : return ast::OpExp::string_kind;
  case 4 : return ast::OpExp::integer_kind;
  case 5 : return ast::OpExp::float_kind;
  case 6 : return ast::OpExp::double_kind;
  case 7 : return ast::OpExp::float_complex_kind;
  case 8 : return ast::OpExp::double_complex_kind;
      
  case 9 : return ast::OpExp::bool_matrix_kind;
  case 10 : return ast::OpExp::string_matrix_kind;
  case 11 : return ast::OpExp::integer_matrix_kind;
  case 12 : return ast::OpExp::float_matrix_kind;
  case 13 : return ast::OpExp::double_matrix_kind;
  case 14 : return ast::OpExp::float_complex_matrix_kind;
  case 15 : return ast::OpExp::double_complex_matrix_kind;
  }
  std::cerr << "Unknown get_OpExp_Kind code " << code << std::endl;
  exit(2);
}

static ast::OpExp::Oper get_OpExp_Oper(void)
{
  int code = get_uint8();
  switch(code){
   case 1 : return  ast::OpExp::plus;
    case 2 : return  ast::OpExp::minus;
    case 3 : return  ast::OpExp::times;
    case 4 : return  ast::OpExp::rdivide;
    case 5 : return  ast::OpExp::ldivide;
    case 6 : return  ast::OpExp::power;

    case 7 : return  ast::OpExp::dottimes;
    case 8 : return  ast::OpExp::dotrdivide;
    case 9 : return  ast::OpExp::dotldivide;
    case 10 : return  ast::OpExp::dotpower;

    case 11 : return  ast::OpExp::krontimes;
    case 12 : return  ast::OpExp::kronrdivide;
    case 13 : return  ast::OpExp::kronldivide;

    case 14 : return  ast::OpExp::controltimes;
    case 15 : return  ast::OpExp::controlrdivide;
    case 16 : return  ast::OpExp::controlldivide;

    case 17 : return  ast::OpExp::eq;
    case 18 : return  ast::OpExp::ne;
    case 19 : return  ast::OpExp::lt;
    case 20 : return  ast::OpExp::le;
    case 21 : return  ast::OpExp::gt;
    case 22 : return  ast::OpExp::ge;

    case 23 : return  ast::OpExp::unaryMinus;

    case 24 : return  ast::OpExp::logicalAnd;
    case 25 : return  ast::OpExp::logicalOr;
    case 26 : return  ast::OpExp::logicalShortCutAnd;
    case 27 : return  ast::OpExp::logicalShortCutOr;
   }
  std::cerr << "Unknown get_OpExp_Oper code " << code << std::endl;
  exit(2);
}

static ast::IntExp::Prec get_IntExp_Prec(void)
{
  int code = get_uint8();
  switch(code){
  case 1: return ast::IntExp::_8_;
  case 2: return ast::IntExp::_16_;
  case 3: return ast::IntExp::_32_;
  case 4: return ast::IntExp::_64_;
  }
  std::cerr << "Unknown get_IntExp_Prec code " << code << std::endl;
  exit(2);
}

static ast::IfExp::Kind get_IfExp_Kind(void)
{
  int code = get_uint8();
  switch(code){
  case 1 : return ast::IfExp::invalid_kind ;
  case 2 : return ast::IfExp::instruction_kind;
  case 3 : return ast::IfExp::expression_kind;
  case 4 : return (ast::IfExp::Kind)0;
  }
  std::cerr << "Unknown get_IfExp_Kind code " << code << std::endl;
  exit(2);
}

static ast::TransposeExp::Kind get_TransposeExp_Kind(void)
{
  int code = get_uint8();
  switch(code){
  case 1 : return ast::TransposeExp::_Conjugate_;
  case 2 : return ast::TransposeExp::_NonConjugate_;
  }
  std::cerr << "Unknown get_TransposeExp_Kind code " << code << std::endl;
  exit(2);
}

static std::wstring* get_wstring(void)
{
  unsigned int size = get_uint32();
  wchar_t* ss = (wchar_t*)buf;
  // std::cerr << "size = " <<  size / sizeof(wchar_t) << std::endl;
  std::wstring* s = new std::wstring(ss, size / sizeof(wchar_t));
  buf += size;
  return s;
}

static symbol::Symbol* get_Symbol(void)
{
  std::wstring* s = get_wstring();
  return new symbol::Symbol(*s);
}

static double get_double(void)
{
  double d = *(double*)buf;
  buf += 8;
  return d;
}

static ast::VarDec* get_VarDec(Location *vardec_location)
{
  symbol::Symbol *name = get_Symbol();
  ast::VarDec::Kind kind = get_VarDec_Kind();
  ast::Exp *init = get_exp();
  ast::VarDec* vardec = new ast::VarDec(*vardec_location, *name, *init);
  vardec->kind_set(kind);
  return vardec;
  }


static ast::Exp* get_exp(void)
{
  ast::Exp* exp;
  // std::cerr << "get_exp at pos " << (buf - initial_buf) << std::endl;
  int code = get_uint8();
  // std::cerr << "    code = " << code << std::endl;
  Location *loc = get_location();
  int is_verbose = get_bool();
  int is_break = get_bool();
  int is_breakable = get_bool();
  int is_return = get_bool();
  int is_returnable = get_bool();
  int is_continue = get_bool();
  int is_continuable = get_bool();
  
  
  switch(code){
  case 1: {   
    std::list<ast::Exp *>* l_body = get_exps();
    exp = new ast::SeqExp(*loc, *l_body);
    break;
  }
  case 2: {
    std::wstring* s = get_wstring();
    exp = new ast::StringExp(*loc, *s);
    break;
  }
  case 3: {
    std::wstring* s = get_wstring();
    exp = new ast::CommentExp(*loc, s);
    break;
  }
  case 4: {
    ast::IntExp::Prec prec = get_IntExp_Prec();
    int value = get_int32();
    exp = new ast::IntExp(*loc, prec, value);
    break;
  }
  case 5: {
    double d = get_double();
    exp = new ast::FloatExp(*loc, d);
    break;
  }
  case 6: {
    double d = get_double();
    exp = new ast::DoubleExp(*loc,d);
    break;
  }
  case 7: {
    bool b = get_bool();
    exp = new ast::BoolExp(*loc, b);
    break;
  }
  case 8: {
    exp = new ast::NilExp(*loc);
    break;
  }
  case 9: {
    symbol::Symbol *name = get_Symbol();
    exp = new ast::SimpleVar(*loc, *name);
    break;
  }
  case 10: {
    exp = new ast::ColonVar(*loc);
    break;
  }
  case 11: {
    exp = new ast::DollarVar(*loc);
    break;
  }
  case 12: {
    std::list<ast::Var*>* vars = get_vars();
    exp = new ast::ArrayListVar(*loc, *vars);
    break;
  }
  case 13: {
    ast::Exp *head = get_exp();
    ast::Exp *tail = get_exp();
    exp = new ast::FieldExp(*loc, *head, *tail);
    break;
  }
  case 14: {
    ast::IfExp::Kind kind = get_IfExp_Kind();
    bool has_else = get_bool();
    ast::Exp* test = get_exp();
    ast::Exp* _then = get_exp();
    ast::IfExp* ifexp;
    if( has_else ){
      ast::Exp* _else = get_exp();
      ifexp = new ast::IfExp(*loc, *test, *_then, *_else);
    } else {
      ifexp = new ast::IfExp(*loc, *test, *_then);
    }
    ifexp->kind_set(kind);
    exp = ifexp;
    break;
  }
  case 15: {
    Location *try_location = get_location();
    Location *catch_location = get_location();
    std::list<ast::Exp *>* try_exps = get_exps();
    std::list<ast::Exp *>* catch_exps = get_exps();
    ast::SeqExp *_try = new ast::SeqExp(*try_location, *try_exps);
    ast::SeqExp *_catch = new ast::SeqExp(*catch_location, *catch_exps);
    exp = new ast::TryCatchExp(*loc, *_try, *_catch);
    break;
  }
  case 16: {
    ast::Exp* test = get_exp();
    ast::Exp* body = get_exp();
    exp = new ast::WhileExp(*loc, *test, *body);
    break;
  }
  case 17: {
    Location *vardec_location = get_location();
    ast::VarDec* vardec = get_VarDec(vardec_location);
    ast::Exp* body = get_exp();
    exp = new ast::ForExp(*loc, *vardec, *body);
    break;
  }
  case 18: {
    exp = new ast::BreakExp(*loc);
    break;
  }
  case 19: {
    exp = new ast::ContinueExp(*loc);
    break;
  }
  case 20: {
    bool is_global = get_bool();
    if( is_global ){
      exp = new ast::ReturnExp(*loc);
    } else {
      ast::Exp* returnExp_exp = get_exp();    
      exp = new ast::ReturnExp(*loc, returnExp_exp);
    }
    break;
  }
  case 21: {
    bool has_default = get_bool();
    ast::SeqExp * default_case = NULL;
    if( has_default ){
      Location *default_case_location = get_location();
      std::list<ast::Exp *>* default_case_exps = get_exps();
      default_case = new ast::SeqExp(*default_case_location, 
				     *default_case_exps);    
    }
    ast::Exp* select = get_exp();

    int nitems = get_uint32();
    std::list<ast::CaseExp*> *cases = new  std::list<ast::CaseExp*>;
    for(int i = 0; i < nitems; i++){

      Location *case_location = get_location();
      Location *body_location = get_location();
      ast::Exp* test = get_exp();
      std::list<ast::Exp *>* body_exps = get_exps();
      ast::SeqExp *body = new ast::SeqExp(*body_location,  *body_exps);

      ast::CaseExp* _case = new ast::CaseExp(*case_location, *test, *body);
      cases->push_back(_case);
    }
    

    if( has_default ){
      exp = new ast::SelectExp(*loc, *select, *cases, *default_case);
    } else {
      exp = new ast::SelectExp(*loc, *select, *cases);
    }
    break;
  }
    /* SHOULD NEVER HAPPEN
  case 22: {
    exp = new ast::CaseExp(*loc);
    break;
  }
    */
  case 23: {
    std::list<ast::MatrixLineExp *>* lines = get_MatrixLines();
    exp = new ast::CellExp(*loc, *lines);
    break;
  }
  case 24: {
    std::list<ast::Exp *>* exps = get_exps();
    exp = new ast::ArrayListExp(*loc, *exps);
    break;
  }
  case 25: {
    std::list<ast::Exp *>* exps = get_exps();
    exp = new ast::AssignListExp(*loc, *exps);
    break;
  }
  case 26: {
    ast::Exp* notexp = get_exp();
    exp = new ast::NotExp(*loc, *notexp);
    break;
  }
  case 27: {
    ast::TransposeExp::Kind kind = get_TransposeExp_Kind();
    ast::Exp* _exp = get_exp();    
    exp = new ast::TransposeExp(*loc, *_exp, kind);
    break;
  }
  case 28: {
    exp = get_VarDec(loc);
    break;
  }
  case 29: {
    symbol::Symbol* name = get_Symbol();
    Location *args_loc = get_location();
    Location *returns_loc = get_location();
    ast::Exp* body = get_exp();
    std::list <ast::Var*>* args_list = get_vars();
    std::list <ast::Var*>* returns_list = get_vars();
    ast::ArrayListVar *args = new ast::ArrayListVar(*args_loc, *args_list);
    ast::ArrayListVar *returns = new ast::ArrayListVar(*returns_loc, *returns_list);
    exp = new ast::FunctionDec(*loc, *name, *args, *returns, *body);
    break;
  }
  case 30: {
    ast::Exp* _start = get_exp();    
    ast::Exp* _step = get_exp();    
    ast::Exp* _end = get_exp();    
    exp = new ast::ListExp(*loc, *_start, *_step, *_end);
    break;
  }
  case 31: {
    ast::Exp* _left = get_exp();    
    ast::Exp* _right = get_exp();    
    exp = new ast::AssignExp(*loc, *_left, *_right);
    break;
  }
  case 32: {
    ast::OpExp::Kind kind = get_OpExp_Kind();
    ast::OpExp::Oper oper = get_OpExp_Oper();
    ast::Exp *left = get_exp();
    ast::Exp *right = get_exp(); 
    ast::OpExp *_opexp  = new ast::OpExp(*loc, *left, oper, *right);
    exp = _opexp;
    _opexp->kind_set(kind);
    break;
  }
  case 33: {
    ast::OpExp::Kind kind = get_OpExp_Kind();
    ast::OpExp::Oper oper = get_OpExp_Oper();
    ast::Exp *left = get_exp();
    ast::Exp *right = get_exp(); 
    ast::LogicalOpExp *_opexp  = 
      new ast::LogicalOpExp(*loc, *left, oper, *right);
    exp = _opexp;
    _opexp->kind_set(kind);
    break;
  }
  case 34: {
    std::list<ast::MatrixLineExp *>* lines = get_MatrixLines();
    exp = new ast::MatrixExp(*loc, *lines);
    break;
  }
  case 35: {
    ast::Exp* name = get_exp();    
    std::list<ast::Exp *> * args = get_exps();
    exp = new ast::CallExp(*loc, *name, *args);
    break;
  }
    /* SHOULD NEVER HAPPEN
  case 36: {
    exp = new ast::MatrixLineExp(*loc);
    break;
  }
    */
  case 37: {
    ast::Exp* name = get_exp();    
    std::list<ast::Exp *>* args = get_exps();
    exp = new ast::CellCallExp(*loc, *name, *args);
    break;
  }
  default: 
    std::cerr << "Unknown code " << code << std::endl;
    exit(2);
  }

  exp->set_verbose(is_verbose);
  if(is_break) exp->break_set();
  if(is_breakable) exp->breakable_set();
  if(is_return) exp->return_set();
  if(is_returnable) exp->returnable_set();
  if(is_continue) exp->continue_set();
  if(is_continuable) exp->continuable_set();
  
  return exp;
}

ast::Exp* scicaml_string2ast(char *buffer)
{
  // std::cerr << "scicaml_string2ast" << std::endl;
  buf = (unsigned char*)buffer;
  initial_buf = (unsigned char*)buffer;
  int buflen = get_uint32();
  ast::Exp* new_ast = get_exp();
  return new_ast;
}
