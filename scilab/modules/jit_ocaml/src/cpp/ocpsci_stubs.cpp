
extern "C" {
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/alloc.h>
#include <caml/custom.h>
}

#include "visitor_common.hxx"
#include "types_gw.hxx"
#include "context.hxx"
#include "generic_operations.hxx"

/*
We must declare all stubs as C functions, otherwise they cannot be
referenced from OCaml code.
 */
extern "C" {
  value ocpsci_get_RealType_c(value sci_v);
  value ocpsci_sci2ml_double_c(value d_v);
  value ocpsci_ml2sci_double_c(value d_v);
  value ocpsci_sci2ml_bool_c(value d_v);
  value ocpsci_ml2sci_bool_c(value d_v);
  value ocpsci_get_funlist_c(value unit_v);
  value ocpsci_context_get_c(value unit_v);
  value ocpsci_call_c(value fun_v, value args_v, value opts_v, value iRetCount_v);
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
  "_ocpsci",
  ocpsci_finalize_scilab,
  ocpsci_compare_scilab,
  custom_hash_default, // ocpsci_custom_hash,
  custom_serialize_default, // ocpsci_custom_serialize,
  custom_deserialize_default // ocpsci_custom_deserialize
};

value Val_scilab(types::InternalType *t_s)
{
  t_s->IncreaseRef();
  value res_v = caml_alloc_custom(&ocpsci_custom_ops, 4, 0, 1);
  Scilab_val(res_v) = t_s;
  return res_v;
}

value ocpsci_ml2sci_double_c(value d_v){
  value res_v;
  double d = Double_val(d_v);
  types::Double *dbl_s = new types::Double(d);
  return Val_scilab(dbl_s);
}

value ocpsci_sci2ml_double_c(value sci_v)
{
  types::Double *ptr_s = Scilab_val(sci_v)->getAs<types::Double>();
  return caml_copy_double(ptr_s->get(0));
}

value ocpsci_ml2sci_bool_c(value d_v){
  value res_v;
  int d = Int_val(d_v);
  types::Bool *b_s = new types::Bool(d);
  return Val_scilab(b_s);
}

value ocpsci_sci2ml_bool_c(value sci_v)
{
  types::Bool *ptr_s = Scilab_val(sci_v)->getAs<types::Bool>();
  return Val_int(ptr_s->get(0));
}

value ocpsci_get_funlist_c(value unit_v)
{
  CAMLparam0();
  CAMLlocal1(res_v);
  int nvals = 0;
  symbol::Context *ctx = symbol::Context::getInstance();
  std::list<symbol::Symbol> funs = ctx->get_funlist(L"");
  std::list<symbol::Symbol>::iterator i;
  for(i = funs.begin(); i != funs.end(); ++i){
    nvals++;
  }
  res_v = caml_alloc(nvals, 0);
  int pos = 0;
  for(i = funs.begin(); i != funs.end(); ++i){
    const symbol::Symbol sy = *i;
    const std::wstring& w = (*i).name_get();
    
    int size = w.size();
    const wchar_t *c_str = w.c_str();
    int final_size = size * sizeof(wchar_t);
    value str_v = caml_alloc_string(final_size);
    memcpy( (void*)str_v, c_str, final_size);
    caml_modify( &Field(res_v,pos), str_v);
    pos++;
  }
  CAMLreturn(res_v);
}

/* TODO: this function does not always return values that
   should be available. Why ? */

static std::wstring Wstring_val(value s_v)
{
  int size = caml_string_length(s_v);
  const wchar_t *c_str = (const wchar_t *)(s_v);  
  return std::wstring(c_str, size / sizeof(wchar_t));
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

value ocpsci_call_c(value fun_v, value args_v, value opts_v, value iRetCount_v)
{
  types::InternalType* fun_s = Scilab_val(fun_v);
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
