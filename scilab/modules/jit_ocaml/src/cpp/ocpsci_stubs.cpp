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

extern "C" {
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/callback.h>
}
extern "C"
{
#include "stdarg.h"
#include "localization.h"
#include "os_swprintf.h"
}
#include "all.hxx"
#include "float.hxx"
#include "int.hxx"
#include "implicitlist.hxx"
#include "visitor_common.hxx"
#include "types_gw.hxx"
#include "context.hxx"
#include "generic_operations.hxx"
#include "overload.hxx"
#include "dollar.hxx"
#include "colon.hxx"
#include "cell.hxx"
#include "graphichandle.hxx"
#include "sparse.hxx"

/*
We must declare all stubs as C functions, otherwise they cannot be
referenced from OCaml code.
 */
extern "C" {
  value ocpsci_get_RealType_c(value sci_v);
  value ocpsci_ml2sci_float_c(value d_v);
  value ocpsci_ml2sci_int8_c(value d_v);
  value ocpsci_ml2sci_int16_c(value d_v);
  value ocpsci_ml2sci_int32_c(value d_v);
  value ocpsci_ml2sci_double_c(value d_v);
  value ocpsci_ml2sci_bool_c(value d_v);
  value ocpsci_new_bool_c(value dims_v);
  value ocpsci_ml2sci_string_c(value d_v);
  value ocpsci_ml2sci_implicitlist_c(value start_v, value step_v, value end_v);

  value ocpsci_sci2ml_double_c(value d_v, value pos_v);
  value ocpsci_sci2ml_bool_c(value d_v, value pos_v);
  value ocpsci_sci2ml_string_c(value d_v, value pos_v);
  value ocpsci_sci2ml_implicitlist_c(value l_v);
  value ocpsci_sci2ml_int8_c(value sci_v, value pos_v);
  value ocpsci_sci2ml_int16_c(value sci_v, value pos_v);
  value ocpsci_sci2ml_int32_c(value sci_v, value pos_v);

  value ocpsci_ml2sci_double_matrix_c(value matrix_v);
  value ocpsci_create_double_matrix_c(value dy_v, value dx_v, value d_v);

  value ocpsci_empty_double_c(value unit_v);

  value ocpsci_set_double_c(value dbl_v, value pos_v, value d_v);
  value ocpsci_set_bool_c(value dbl_v, value pos_v, value d_v);
  value ocpsci_set_int8_c(value dbl_v, value pos_v, value d_v);
  value ocpsci_set_int16_c(value dbl_v, value pos_v, value d_v);
  value ocpsci_set_int32_c(value dbl_v, value pos_v, value d_v);

  value ocpsci_sparsebool_set_c(value dbl_v, value row_v, value col_v, value d_v);
  value ocpsci_sparsebool_get_c(value dbl_v, value row_v, value col_v);


  value ocpsci_get_funlist_c(value unit_v);
  value ocpsci_context_get_c(value unit_v);
  value ocpsci_call_c(value fun_v, value args_v, value opts_v, value iRetCount_v);
  value ocpsci_operation_c(value oper_v, value left_v, value right_v);
  value ocpsci_ml2sci_ocamlfunction_c(value name_v, value fun_v);
  value ocpsci_set_ocaml_functions_c(value function_array_v);
  value ocpsci_clone_c(value sci_v);

  value ocpsci_refcount_c(value ptr_v);
  value ocpsci_incr_refcount_c(value ptr_v);
  value ocpsci_decr_refcount_c(value ptr_v);

  value ocpsci_generic_getSize_c(value array_v);
  value ocpsci_generic_getDimsArray_c(value array_v);
  value ocpsci_generic_getCols_c(value array_v);
  value ocpsci_generic_getRows_c(value array_v);
  value ocpsci_generic_getColumnValues_c(value array_v, value pos_v);
  value ocpsci_colon_c(value unit_v);
  value ocpsci_dollar_c(value unit_v);
  value ocpsci_list_get_c(value list_v, value pos_v);
  value ocpsci_implicitlist_extractFullMatrix_c(value list_v);
  value ocpsci_arrayof_get_c(value array_v, value pos_v);
  value ocpsci_arrayof_set_c(value array_v, value pos_v, value val_v);
  value ocpsci_map_c(value array_v);
  value ocpsci_addElementToVariable_c(value option_v, value added_v, 
				      value iRows_v, value iCols_v);


  value ocpsci_overload_buildName0_c(value name_v);
  value ocpsci_overload_buildName1_c(value name_v, value arg1_v);
  value ocpsci_overload_buildName2_c(value name_v, value arg1_v, value arg2_v);
  value ocpsci_getShortTypeStr_c(value val_v);
  value ocpsci_overload_getNameFromOper_c(value oper_v);
}

#define Scilab_val(v) (*((types::InternalType**) Data_custom_val(v)))

/* types class hierarchy
InternalType
    GenericType
         Container
	      List
	          TList
		       MList
         Float
         SinglePoly
         ArrayOf
              Bool 
	      Cell
	      Double
	      GraphicHandle
	      Int8
	      Int16
	      Int32
	      Int64
	      Polynom
                   Dollar
	      String
	      Struct
	      UInt8
	      UInt16
	      UInt32
	      UInt64
	 Sparse
	 SparseBool
    Callable 
         Function
	      OptFunction
	      WrapFunction
	      WrapMexFunction
	      DynamicFunction
	 Macro
         MacroFile
    ImplicitList
         Colon
    ListOperation
         ListInsert
	 ListDelete
	 ListUndefined
    SinglePoly
    SingleStruct
    ThreadId
    User
         Pointer
File
GatewayStruct
Inspector
*/
value ocpsci_get_RealType_c(value sci_v)
{
  types::InternalType *sci = Scilab_val(sci_v);
  switch(sci->getType()){
                /* Internal Type */
  case types::InternalType::RealInternal: return Val_int(0);
            /* Generic Types */
  case types::InternalType::RealGeneric: return Val_int(1);
  case types::InternalType::RealInt8: return Val_int(2);
  case types::InternalType::RealUInt8: return Val_int(3);
  case types::InternalType::RealInt16: return Val_int(4);
  case types::InternalType::RealUInt16: return Val_int(5);
  case types::InternalType::RealInt32: return Val_int(6);
  case types::InternalType::RealUInt32: return Val_int(7);
  case types::InternalType::RealInt64: return Val_int(8);
  case types::InternalType::RealUInt64: return Val_int(9);

  case types::InternalType::RealString: return Val_int(10);
  case types::InternalType::RealDouble: return Val_int(11);
  case types::InternalType::RealBool: return Val_int(12);
  case types::InternalType::RealFloat: return Val_int(13);
  case types::InternalType::RealPoly: return Val_int(14);
  case types::InternalType::RealSinglePoly: return Val_int(15);
    /* Callable */
  case types::InternalType::RealFunction: return Val_int(16);
  case types::InternalType::RealMacro: return Val_int(17);
  case types::InternalType::RealMacroFile: return Val_int(18);
    /* Implicit List */
  case types::InternalType::RealImplicitList: return Val_int(19);

    /* Container */
  case types::InternalType::RealContainer: return Val_int(20);
  case types::InternalType::RealList: return Val_int(21);
  case types::InternalType::RealTList: return Val_int(22);
  case types::InternalType::RealMList: return Val_int(23);
  case types::InternalType::RealSingleStruct: return Val_int(24);
  case types::InternalType::RealStruct: return Val_int(25);
  case types::InternalType::RealCell: return Val_int(26);
    /* User */
  case types::InternalType::RealUserType: return Val_int(27);
    /*For list operation*/
  case types::InternalType::RealListOperation: return Val_int(28); //parent type
  case types::InternalType::RealListInsertOperation: return Val_int(29);

  case types::InternalType::RealListDeleteOperation: return Val_int(30);
  case types::InternalType::RealListUndefinedOperation: return Val_int(31);
  case types::InternalType::RealFile: return Val_int(32);
  case types::InternalType::RealColon: return Val_int(33);
  case types::InternalType::RealDollar: return Val_int(34);
  case types::InternalType::RealThreadId: return Val_int(35);
  case types::InternalType::RealSparse: return Val_int(36);
  case types::InternalType::RealSparseBool: return Val_int(37);
  case types::InternalType::RealSingleHandle: return Val_int(38);
  case types::InternalType::RealHandle: return Val_int(39);
  }
  return Val_int(40);
}


void ocpsci_finalize_scilab(value ptr_v)
{
  types::InternalType *ptr = Scilab_val(ptr_v);
  ptr->DecreaseRef();
  if(ptr->isDeletable()){ delete ptr; }
}

/* TODO: this compare function is incorrect for non-scalar values, as
GenericLess returns a matrix of comparisons, not a comparison of
matrices. We should implement a lexicographic comparison on matrices.
 */

int ocpsci_compare_scilab(value ptr1_v, value ptr2_v)
{
  int res;

  types::InternalType *ptr1 = Scilab_val(ptr1_v);
  types::InternalType *ptr2 = Scilab_val(ptr2_v);
  types::InternalType *pResult = GenericLess(ptr1, ptr2);
  res = bConditionState(pResult);
  delete pResult;
  if( res ) return -1;
  pResult = GenericGreater(ptr1, ptr2);
  res = bConditionState(pResult);
  delete pResult;
  if( res ) return 1;

  return 0;
}

struct custom_operations ocpsci_custom_ops = {
  (char*)"_ocpsci",
  ocpsci_finalize_scilab,
  ocpsci_compare_scilab,
  custom_hash_default, // ocpsci_custom_hash,
  custom_serialize_default, // ocpsci_custom_serialize,
  custom_deserialize_default // ocpsci_custom_deserialize
};

value Val_scilab(types::InternalType *t_s)
{
  if( t_s == NULL )
    caml_failwith("Val_scilab(NULL)");

  t_s->IncreaseRef();
  value res_v = caml_alloc_custom(&ocpsci_custom_ops, sizeof(t_s), 1, 1000);
  Scilab_val(res_v) = t_s;
  return res_v;
}


value Val_wstring(const std::wstring & w)
{
  int size = w.size();
  const wchar_t *c_str = w.c_str();
  int final_size = size * sizeof(wchar_t);
  value name_v = caml_alloc_string(final_size);
  memcpy( (void*)name_v, c_str, final_size);
  return name_v;
}


value ocpsci_ml2sci_double_c(value d_v){
  value res_v;
  double d = Double_val(d_v);
  types::Double *dbl_s = new types::Double(d);
  return Val_scilab(dbl_s);
}

value ocpsci_ml2sci_float_c(value d_v){
  value res_v;
  double d = Double_val(d_v);
  types::Float *dbl_s = new types::Float(d);
  return Val_scilab(dbl_s);
}

value ocpsci_ml2sci_int8_c(value d_v){
  value res_v;
  int d = Int32_val(d_v);
  types::Int8 *dbl_s = new types::Int8(d);
  return Val_scilab(dbl_s);
}

value ocpsci_ml2sci_int16_c(value d_v){
  value res_v;
  int d = Int32_val(d_v);
  types::Int16 *dbl_s = new types::Int16(d);
  return Val_scilab(dbl_s);
}

value ocpsci_ml2sci_int32_c(value d_v){
  value res_v;
  int d = Int32_val(d_v);
  types::Int32 *dbl_s = new types::Int32(d);
  return Val_scilab(dbl_s);
}

value ocpsci_empty_double_c(value unit_v){
  types::Double *dbl_s = types::Double::Empty();
  return Val_scilab(dbl_s);
}

value ocpsci_set_double_c(value dbl_v, value pos_v, value d_v)
{
  double d = Double_val(d_v);
  types::Double *dbl_s = Scilab_val(dbl_v)->getAs<Double>();
  dbl_s->set(Int_val(pos_v), d);
  return Val_unit;
}

value ocpsci_arrayof_get_c(value array_v, value pos_v)
{
  types::ArrayOf<types::InternalType*> *array_s = Scilab_val(array_v)->getAs<ArrayOf<types::InternalType*> >();
  return Val_scilab( array_s->get( Int_val(pos_v) ));
}

value ocpsci_arrayof_set_c(value array_v, value pos_v, value val_v)
{
  types::ArrayOf<types::InternalType*> *array_s = Scilab_val(array_v)->getAs<ArrayOf<types::InternalType*> >();
  array_s->set( Int_val(pos_v), Scilab_val(val_v) );
  return Val_unit;
}

value ocpsci_map_c(value sci_v)
{
  types::InternalType *sci_s = Scilab_val(sci_v)->getAs<types::InternalType>();  
  types::InternalType *res_s = NULL;
  switch(sci_s->getType()){
  case types::InternalType::RealDouble: {
    types::Double *pb = sci_s->getAs<types::Double>();
    res_s = new Double(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealInt8: {
    types::Int8 *pb = sci_s->getAs<types::Int8>();
    res_s = new Int8(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealInt16: {
    types::Int16 *pb = sci_s->getAs<types::Int16>();
    res_s = new Int16(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealInt32: {
    types::Int32 *pb = sci_s->getAs<types::Int32>();
    res_s = new Int32(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealInt64: {
    types::Int64 *pb = sci_s->getAs<types::Int64>();
    res_s = new Int64(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealUInt8: {
    types::UInt8 *pb = sci_s->getAs<types::UInt8>();
    res_s = new UInt8(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealUInt16: {
    types::UInt16 *pb = sci_s->getAs<types::UInt16>();
    res_s = new UInt16(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealUInt32: {
    types::UInt32 *pb = sci_s->getAs<types::UInt32>();
    res_s = new UInt32(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealUInt64: {
    types::UInt64 *pb = sci_s->getAs<types::UInt64>();
    res_s = new UInt64(pb->getDims(), pb->getDimsArray());
    break;
  }
  case types::InternalType::RealSparseBool: {
    types::SparseBool *pb = sci_s->getAs<types::SparseBool>();
    res_s = new SparseBool(pb->getRows(), pb->getCols());
    break;
  }

  case types::InternalType::RealCell: 
  case types::InternalType::RealHandle: 
  case types::InternalType::RealString: 
  case types::InternalType::RealStruct: 
  case types::InternalType::RealPoly: 
  case types::InternalType::RealDollar:
  default:
    res_s = sci_s->clone();
  }
  return Val_scilab(res_s);
}

value ocpsci_list_get_c(value list_v, value pos_v)
{
  types::List *list_s = Scilab_val(list_v)->getAs<List>();
  return Val_scilab( list_s->get(Int_val(pos_v) ) );
}

value ocpsci_generic_getColumnValues_c(value array_v, value pos_v)
{
  types::GenericType *array_s = Scilab_val(array_v)->getAs<GenericType>();
  return Val_scilab( array_s->getColumnValues( Int_val(pos_v) ) );
}

value ocpsci_generic_getDimsArray_c(value array_v)
{
  types::GenericType *array_s = Scilab_val(array_v)->getAs<GenericType>();
  
  int ndims = array_s->getDims();
  if( ndims == 0 ){
    return Atom(0);
  } else {
    value res_v = caml_alloc( ndims, 0);
    int *dims = array_s->getDimsArray();
    for( int i=0; i < ndims; i++ )
      Field( res_v, i ) = Val_int ( dims[i] );
    return res_v;
  }
}

value ocpsci_generic_getCols_c(value array_v)
{
  types::GenericType *array_s = Scilab_val(array_v)->getAs<GenericType>();
  return Val_int( array_s->getCols() );
}

value ocpsci_generic_getRows_c(value array_v)
{
  types::GenericType *array_s = Scilab_val(array_v)->getAs<GenericType>();
  return Val_int( array_s->getRows() );
}

value ocpsci_generic_getSize_c(value array_v)
{
  types::GenericType *array_s = Scilab_val(array_v)->getAs<GenericType>();
  return Val_int( array_s->getSize() );
}

value ocpsci_sci2ml_double_c(value sci_v, value pos_v)
{
  types::Double *ptr_s = Scilab_val(sci_v)->getAs<types::Double>();
  return caml_copy_double(ptr_s->get(Int_val(pos_v)));
}

value ocpsci_sci2ml_int8_c(value sci_v, value pos_v)
{
  types::Int8 *ptr_s = Scilab_val(sci_v)->getAs<types::Int8>();
  return caml_copy_int32(ptr_s->get( Int_val(pos_v) ));
}

value ocpsci_sci2ml_int16_c(value sci_v, value pos_v)
{
  types::Int16 *ptr_s = Scilab_val(sci_v)->getAs<types::Int16>();
  return caml_copy_int32(ptr_s->get( Int_val(pos_v) ));
}

value ocpsci_sci2ml_int32_c(value sci_v, value pos_v)
{
  types::Int32 *ptr_s = Scilab_val(sci_v)->getAs<types::Int32>();
  return caml_copy_int32(ptr_s->get( Int_val(pos_v) ));
}


value ocpsci_set_int8_c(value dbl_v, value pos_v, value d_v)
{
  int d = Int32_val(d_v);
  types::Int8 *dbl_s = Scilab_val(dbl_v)->getAs<Int8>();
  dbl_s->set( Int_val(pos_v), d);
  return Val_unit;
}

value ocpsci_set_int16_c(value dbl_v, value pos_v, value d_v)
{
  int d = Int32_val(d_v);
  types::Int16 *dbl_s = Scilab_val(dbl_v)->getAs<Int16>();
  dbl_s->set( Int_val(pos_v) , d);
  return Val_unit;
}

value ocpsci_set_int32_c(value dbl_v, value pos_v, value d_v)
{
  int d = Int32_val(d_v);
  types::Int32 *dbl_s = Scilab_val(dbl_v)->getAs<Int32>();
  dbl_s->set( Int_val(pos_v), d);
  return Val_unit;
}


value ocpsci_ml2sci_bool_c(value d_v){
  value res_v;
  int d = Int_val(d_v);
  types::Bool *b_s = new types::Bool(d);
  return Val_scilab(b_s);
}

value ocpsci_new_bool_c(value dims_v){
  int ndims = Wosize_val(dims_v);
  int dims[ ndims ];
  for(int i = 0; i < ndims; i++)
    dims[i] = Int_val( Field(dims_v, i) );

  types::Bool *b_s = new types::Bool(ndims, dims);
  return Val_scilab(b_s);
}

value ocpsci_sci2ml_bool_c(value sci_v, value pos_v)
{
  types::Bool *ptr_s = Scilab_val(sci_v)->getAs<types::Bool>();
  return Val_int(ptr_s->get(Int_val(pos_v)));
}

value ocpsci_set_bool_c(value sci_v, value pos_v, value bool_v)
{
  types::Bool *ptr_s = Scilab_val(sci_v)->getAs<types::Bool>();
  ptr_s->set( Int_val(pos_v), Int_val(bool_v) );
  return Val_unit;
}

value ocpsci_sparsebool_get_c(value sci_v, value row_v, value col_v)
{
  types::SparseBool *ptr_s = Scilab_val(sci_v)->getAs<types::SparseBool>();
  return Int_val(ptr_s->get( Int_val(row_v), Int_val(col_v)));
}

value ocpsci_sparsebool_set_c(value sci_v, value row_v, value col_v, value bool_v)
{
  types::SparseBool *ptr_s = Scilab_val(sci_v)->getAs<types::SparseBool>();
  ptr_s->set( Int_val(row_v), Int_val(col_v), Int_val(bool_v) );
  return Val_unit;
}

value ocpsci_get_funlist_c(value unit_v)
{
  CAMLparam0();
  CAMLlocal1(res_v);
  int nvals = 0;
  symbol::Context *ctx = symbol::Context::getInstance();
  std::list<symbol::Symbol> funs = ctx->getFunctionList(L"");
  std::list<symbol::Symbol>::iterator i;
  for(i = funs.begin(); i != funs.end(); ++i){
    nvals++;
  }
  res_v = caml_alloc(nvals, 0);
  int pos = 0;
  for(i = funs.begin(); i != funs.end(); ++i){
    value str_v = Val_wstring( (*i).name_get() );
    caml_modify( &Field(res_v,pos), str_v);
    pos++;
  }
  CAMLreturn(res_v);
}

value ocpsci_ml2sci_implicitlist_c(value start_v, value step_v, value end_v){
  value res_v;
  types::InternalType *start_s = Scilab_val(start_v);
  types::InternalType *step_s = Scilab_val(step_v);
  types::InternalType *end_s = Scilab_val(end_v);
  types::ImplicitList *l_s = new types::ImplicitList(start_s, step_s, end_s);
  return Val_scilab(l_s);
}

value ocpsci_sci2ml_implicitlist_c(value l_v)
{
  CAMLparam0();
  CAMLlocal1(res_v);
  
  types::ImplicitList *l_s = Scilab_val(l_v)->getAs<types::ImplicitList>();  
  res_v = caml_alloc(3,0);

  value tmp_v = Val_scilab( l_s->getStart() );
  caml_modify( &Field(res_v, 0), tmp_v );

  tmp_v = Val_scilab( l_s->getStep() );
  caml_modify( &Field(res_v, 1), tmp_v ); 
  tmp_v = Val_scilab( l_s->getEnd() );
  caml_modify( &Field(res_v, 2), tmp_v );

  CAMLreturn(res_v);
}

value ocpsci_implicitlist_extractFullMatrix_c(value l_v)
{
  types::ImplicitList *l_s = Scilab_val(l_v)->getAs<types::ImplicitList>();  
  return Val_scilab( l_s->extractFullMatrix() );
}

value ocpsci_clone_c(value sci_v)
{
  types::InternalType *sci_s = Scilab_val(sci_v);

  return Val_scilab( sci_s->clone() );
}

value ocpsci_sci2ml_string_c(value s_v, value pos_v)
{
  types::String *s_s = Scilab_val(s_v)->getAs<types::String>();
  return Val_wstring( s_s->get(Int_val(pos_v)) );
}

static std::wstring Wstring_val(value s_v)
{
  int size = caml_string_length(s_v);
  const wchar_t *c_str = (const wchar_t *)(s_v);  
  return std::wstring(c_str, size / sizeof(wchar_t));
}


value ocpsci_ml2sci_string_c(value s_v){
  value res_v;
  std::wstring ws = Wstring_val(s_v);
  types::String *s_s = new types::String(ws.c_str());
  return Val_scilab(s_s);
}


value ocpsci_context_get_c(value name_v)
{
  std::wstring s = Wstring_val(name_v);
  symbol::Context *ctx = symbol::Context::getInstance();
  const symbol::Symbol sy = symbol::Symbol(s);
  types::InternalType *v_s = ctx->get(sy);
  if( v_s == NULL) caml_raise_not_found();
  return Val_scilab(v_s);
}

value ocpsci_ml2sci_double_matrix_c(value matrix_v)
{
  value res_v;
  int dy = Wosize_val(matrix_v);
  int dx = 0;
  for(int i=0; i < dy; i++){
    int rdx = Wosize_val(Field(matrix_v,i)) / Double_wosize;
    if( rdx > dx ) dx = rdx;
  }
  types::Double *dbl_s = new types::Double(dy, dx, false);
  for(int y=0; y < dy; y++){
    value row_v = Field( matrix_v, y);
    int dx = Wosize_val(row_v) / Double_wosize;
    for(int x = 0; x < dx; x++)
      dbl_s->set(y, x, Double_field( row_v, x ));
  }
  return Val_scilab(dbl_s);
}

value ocpsci_create_double_matrix_c(value dy_v, value dx_v, value d_v)
{
  value res_v;
  int dx = Int_val(dx_v);
  int dy = Int_val(dy_v);
  double d = Double_val(d_v);
  types::Double *dbl_s = new types::Double(dy,dx,false);
  for(int iPos = 0; iPos < dx * dy; iPos++)
    dbl_s->set(iPos, d);
  return Val_scilab(dbl_s);
}

ast::OpExp::Oper OpExpOper_val(value oper_v)
{
  int code = Int_val(oper_v);

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
  caml_failwith("Unknown OpExp::Oper");
}


value ocpsci_operation_c(value oper_v, value left_v, value right_v)
{
  types::InternalType *left_s = Scilab_val(left_v);
  types::InternalType *right_s = Scilab_val(right_v);
  ast::OpExp::Oper oper_s = OpExpOper_val(oper_v);

  types::InternalType *res_s = NULL;

  try {
        switch (oper_s)
        {
            case OpExp::plus :
            {
                res_s = GenericPlus(left_s, right_s);
                break;
            }
            case OpExp::unaryMinus :
            {
                res_s = GenericUnaryMinus(right_s);
                break;
            }
            case OpExp::minus :
            {
                res_s = GenericMinus(left_s, right_s);
                break;
            }
            case OpExp::times:
            {
                res_s = GenericTimes(left_s, right_s);
                break;
            }
            case OpExp::ldivide:
            {
                break;
            }
            case OpExp::rdivide:
            {
                res_s = GenericRDivide(left_s, right_s);
                break;
            }
            case OpExp::dotrdivide :
            {
                res_s = GenericDotRDivide(left_s, right_s);
                break;
            }
            case OpExp::dottimes :
            {
                res_s = GenericDotTimes(left_s, right_s);
                break;
            }
            case OpExp::dotpower :
            {
                res_s = GenericDotPower(left_s, right_s);
                break;
            }
            case OpExp::eq :
            {
                res_s = GenericComparisonEqual(left_s, right_s);
                break;
            }
            case OpExp::ne :
            {
                res_s = GenericComparisonNonEqual(left_s, right_s);
                break;
            }
            case OpExp::lt :
            {
                res_s = GenericLess(left_s, right_s);
                break;
            }
            case OpExp::le :
            {
                res_s = GenericLessEqual(left_s, right_s);
                break;
            }
            case OpExp::gt :
            {
                res_s = GenericGreater(left_s, right_s);
                break;
            }
            case OpExp::ge :
            {
                res_s = GenericGreaterEqual(left_s, right_s);
                break;
            }
            case OpExp::power :
            {
                res_s = GenericPower(left_s, right_s);
                break;
            }
            case OpExp::krontimes :
            {
                res_s = GenericKrontimes(left_s, right_s);
                break;
            }
            case OpExp::kronrdivide :
            {
                res_s = GenericKronrdivide(left_s, right_s);
                break;
            }
            case OpExp::kronldivide :
            {
                res_s = GenericKronldivide(left_s, right_s);
                break;
            }
	    /* For these ones, we must check if both arguments are boolean,
	       otherwise we fall back in the overloaded case. */
            case LogicalOpExp::logicalShortCutOr :
            case LogicalOpExp::logicalOr :
            case LogicalOpExp::logicalShortCutAnd :
            case LogicalOpExp::logicalAnd :
            default :
                break;
        }


    if (res_s == NULL)
      caml_failwith( "OVERLOAD" );

  } catch (ScilabError error)
    {
      /* TODO: how to convert ScilabError to string ? */
      caml_failwith("ScilabError");
    }
  
  return Val_scilab(res_s);
}

static value ocaml_functions_v = Val_int(0);

value ocpsci_set_ocaml_functions_c(value function_array_v)
{
  if( ocaml_functions_v == Val_int(0) ){
    caml_register_global_root( &ocaml_functions_v );
  }
  ocaml_functions_v = function_array_v;
}

namespace types 
{

class OCamlFunction : public Function
{
private :
  OCamlFunction(OCamlFunction* _pOCamlFunction);
public :
  OCamlFunction(std::wstring _wstName, int _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule);
  
  Callable::ReturnValue call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);
  InternalType*           clone();
  int              getFunc() { return m_iOCamlFunc; }

private :
  int              m_iOCamlFunc;
};
}

OCamlFunction::OCamlFunction(std::wstring _wstName, int _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule)
{
    m_wstName = _wstName;
    m_iOCamlFunc = _pFunc;
    m_wstModule = _wstModule;
    m_pLoadDeps = _pLoadDeps;
}

OCamlFunction::OCamlFunction(OCamlFunction* _pOCamlFunction)
{
    m_wstModule  = _pOCamlFunction->getModule();
    m_wstName    = _pOCamlFunction->getName();
    m_iOCamlFunc  = _pOCamlFunction->getFunc();
    m_pLoadDeps = _pOCamlFunction->getDeps();
}

InternalType* OCamlFunction::clone()
{
    return new OCamlFunction(this);
}

Function::ReturnValue OCamlFunction::call(typed_list &in, optional_list &opt, 
					  int _iRetCount, typed_list &out, 
					  ast::ConstVisitor* execFunc)
{
  CAMLparam0();
  CAMLlocal3(args_v, opts_v, opt_v);

    if (m_pLoadDeps != NULL)
    {
        m_pLoadDeps();
    }

    ReturnValue retVal = Callable::OK;

    args_v = caml_alloc( in.size(), 0);
    for(int i = 0 ; i < in.size() ; i++)
    {
      value arg_v = Val_scilab(in[i]->clone());
      caml_modify( &Field(args_v, i), arg_v );
    }

    opts_v = caml_alloc( opt.size(), 0);
    for(int i = 0 ; i < opt.size() ; i++)
    {
      opt_v = caml_alloc( 2, 0 );

      value name_v = Val_wstring( opt[i].first );
      caml_modify( &Field( opt_v,0), name_v );

      value arg_v = Val_scilab(opt[i].second);
      caml_modify( &Field( opt_v,1), arg_v );
      
      caml_modify( &Field( opts_v, i), opt_v );
    }

    value f = Field(ocaml_functions_v, m_iOCamlFunc);
    value ret_v = caml_callback3(f, args_v, opts_v, Val_int(_iRetCount));

    if( ret_v == Val_int(0) ) {
      // TODO we need find the error message ?
      retVal = Callable::Error;
    } else     
      if( Wosize_val(ret_v) == 0 ){
	retVal = Callable::OK_NoResult;
      } else {
	retVal = Callable::OK;
	int nreturns = Wosize_val(ret_v);
	for(int i=0; i<nreturns; i++){
	  out.push_back( Scilab_val(Field(ret_v,i)) );
	}
      }
    

    CAMLreturnT( ReturnValue, retVal );
}

value ocpsci_ml2sci_ocamlfunction_c(value name_v, value fun_v){
  value res_v;
  int iFunc = Int_val(fun_v);
  const std::wstring wstName = Wstring_val( name_v );  

  types::OCamlFunction *fun_s = new types::OCamlFunction(wstName, iFunc, NULL, L"__ocaml__");
  return Val_scilab(fun_s);
}



value ocpsci_call_c(value fun_v, value args_v, value opts_v, value iRetCount_v)
{
  types::InternalType* fun_s = Scilab_val(fun_v);

  OCamlFunction* pF = dynamic_cast<OCamlFunction*>(fun_s);
  if( pF != NULL ){

    int m_iOCamlFunc = pF->getFunc();
    value f = Field(ocaml_functions_v, m_iOCamlFunc);    
    
    return caml_callback3(f, args_v, opts_v, iRetCount_v);

  }

  types::Callable *pCall = fun_s->getAs<types::Callable>();
  types::typed_list out;
  types::typed_list in;
  types::optional_list opt;
  int iRetCount = Int_val(iRetCount_v);
  
  int nargs = Wosize_val(args_v);
  for(int i = 0; i < nargs; i++)
    in.push_back(Scilab_val(Field(args_v, i)));

  int nopts = Wosize_val(opts_v);
  for(int i = 0; i < nopts; i++){
    value pair_v = Field(opts_v, i);
    const std::wstring s = Wstring_val( Field(pair_v, 0) );
    types::InternalType *v_s = Scilab_val( Field(pair_v, 1) );
    opt.push_back(std::pair<std::wstring, types::InternalType*>(s, v_s));
  }

  // TODO: last parameter should be a visitor. What for ?
  types::Function::ReturnValue Ret = pCall->call(in, opt, iRetCount, out, NULL);

  if( Ret == types::Callable::OK ){
    CAMLparam0();
    CAMLlocal1(rets_v);
    int nrets = out.size();
    rets_v = caml_alloc(nrets, 0);
    for(int i = 0; i < nrets; i++){
      value ret_v = Val_scilab(out[i]);
      caml_modify( &Field(rets_v, i), ret_v );
    }
    value res_v = caml_alloc(1,0);
    Field( res_v, 0 ) = rets_v;
    CAMLreturn(res_v);
  } else {
    return Val_int(0); // None
  }

}



value ocpsci_refcount_c(value ptr_v)
{
  types::InternalType* ptr_s = Scilab_val(ptr_v);
  return Val_int( ptr_s->getRef() );
}

value ocpsci_incr_refcount_c(value ptr_v)
{
  types::InternalType* ptr_s = Scilab_val(ptr_v);
  ptr_s->IncreaseRef();
  return Val_unit;
}

value ocpsci_decr_refcount_c(value ptr_v)
{
  types::InternalType* ptr_s = Scilab_val(ptr_v);
  ptr_s->DecreaseRef();
  return Val_unit;
}

value ocpsci_colon_c(value unit_v)
{
  return Val_scilab(new types::Colon());
}

value ocpsci_dollar_c(value unit_v)
{
  return Val_scilab(new types::Dollar());
}

value ocpsci_addElementToVariable_c(value option_v, value added_v, 
				      value iRows_v, value iCols_v)
{
  InternalType *p_s;
  
  if( option_v == Val_int(0) ) { p_s = NULL; } 
  else {
    p_s = Scilab_val( Field(option_v, 0) );
  }

  InternalType *added_s = Scilab_val(added_v);
  InternalType *res_s = 
    AddElementToVariable(p_s, added_s, 
			 Int_val(iRows_v), Int_val(iCols_v));

  if( res_s == p_s ) return Field(option_v, 0);
  return Val_scilab( res_s );
}

value ocpsci_overload_buildName0_c(value name_v)
{
  std::wstring _stFunctionName = Wstring_val(name_v);
  return Val_wstring( L"%_" + _stFunctionName );
}

value ocpsci_overload_buildName1_c(value name_v, value arg1_v)
{
  std::wstring _stFunctionName = Wstring_val(name_v);
  InternalType *arg1_s = Scilab_val( arg1_v );

  return 
    Val_wstring( L"%" + _stFunctionName + L"_" + arg1_s->getShortTypeStr() );
}

value ocpsci_overload_buildName2_c(value name_v, value arg1_v, value arg2_v)
{
  std::wstring _stFunctionName = Wstring_val(name_v);
  InternalType *arg1_s = Scilab_val( arg1_v );
  InternalType *arg2_s = Scilab_val( arg2_v );

  return 
    Val_wstring( L"%" + arg1_s->getShortTypeStr() + L"_"
		 + _stFunctionName + L"_" + arg2_s->getShortTypeStr() );
}

value ocpsci_getShortTypeStr_c(value val_v)
{
  return Val_wstring( Scilab_val(val_v)->getShortTypeStr() );
}




value ocpsci_overload_getNameFromOper_c(value oper_v)
{
  int code = Int_val(oper_v);
  return Val_wstring( Overload::getNameFromOper( OpExpOper_val(oper_v) ) );
}
