/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
extern "C" {
#include <stdio.h>

#include "stack-c.h"
#include "gw_core.h"

#include "dynamic_link.h"
#include "api_common.h"
#include "api_double.h"

#include "MALLOC.h"

#include "stack1.h"
#include "api_double.h"
#include "api_int.h"
#include "api_list.h"
#include "api_string.h"
#include "api_common.h"
#include "stack3.h"
#include "stack2.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_mem_alloc.h"

#include "stack-def.h" //#define nlgh nsiz*4  
#include "stack-c.h"  // #define Nbvars C2F(intersci).nbvars, Top & cie

}

#include <cstdlib>
#include <cstring>
#include "mc_apply.hxx"

#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

/*
 *
 fun_name='test_fun';
 c_prog=['#include  <math.h>'
 'void '+fun_name+'(void const* const* args, void *const* res) {'
 '*((double*)*res)= 2.*(((double*)args[0])[0]);'
 '}'];
 disp(c_prog);
 mputl(c_prog,fun_name+'.c');
 ilib_for_link(fun_name,fun_name+'.c',[],"c");
 exec loader.sce;
 a=mc_apply([1,2],fun_name,1)

 function a= g(arg1); a=2*arg1; disp(arg1); endfunction; // ne marche pas avec des noms du genre f5 ...

 a=mc_apply([1,2], "g")



 We can handle k Rhs  et m Lhs. Currently, only real (double) matrix are implemented, but the framework is here for any Scilab datatype.
 (see scilab_var and scilab_allocated_var class hierarchy).
 [R1, ... , Rm] = mc_apply(A1, ... , Ak, f [,Types] [,Dims])

 If Args are of différent sizes, the smallest are recycled.

 Types : matrix of <=m strings of the names (as in typeof()) of the  m lhs fo 'f'. default to "constant"
 Rows : matrix of doubles 1 x <=m or 2 x <=m giving the nb of rows (or rows and columns) of the m lhs of f. default to 1


 bug de scifunction : les macros doivent être appelées par leur nom :(
 pour la vérification qu'un nom est bien celui d'une macro :
 getmacroslist
 et dans core/src/c/getvariablesname.c puis vérifier que le type est bien sci_XXX :( :( :(


 /!\ due to matrix data layout in Scilab, a matrix [a11,a12,a13;a21,a22,a23] contains 3 (ncols) arguments of 2 (nrows) elements;



TODO: add optional list arg to specify nb of workerd, threads or processes, rand generator seeds...

*/

extern "C" {
  int  C2F(sci_mc_apply)(char *fname,unsigned long fname_len);
}


int current_top;


struct scilab_var {

  //  explicit scilab_var(int t):type(t){} we cannot construct in initializer list because getVarType does not return the type
  int type;

  static scilab_var* create(char const* type_name, int nrows, int ncols);

  virtual int get_type() const{ return type; }
  virtual void set_type( int t) { type= t; }
  virtual void set_type( char const* type_name) { 
    //    std::cerr<<"set_type("<<type_name<<")\n";
    type= (strcmp(type_name, "constant")==0) 
      ? sci_matrix
      : -1; 
  }

  virtual void debug()const{ std::cerr<<" default debug;\n";}

  virtual  std::size_t element_size() const=0;
  virtual std::size_t sizeof_data()const{ std::cerr<<"default sizeof_data\n"; return 0;}
  virtual  ~scilab_var(){}
};


struct scilab_c_function : virtual scilab_var {
  virtual  std::size_t element_size() const {return sizeof(int);}
  int nb_lhs;
  int nb_rhs;
};

struct scilab_matrix : virtual scilab_var {
  virtual void debug()const{ std::cerr<<" matrix type:"<<get_type()<<" nrows:"<<nrows<<"ncols:"<<ncols<<std::endl ;}

  int nrows, ncols;
  void set_from_model(scilab_matrix const& m, std::size_t const n){
    type= sci_matrix;
    switch(n) {
    case 0 :{
      nrows= m.nrows;
      ncols= 1;
      break;
    }
    case 1 :{
      nrows= m.nrows;
      ncols= m.ncols;
      break;
    }
    default: {
      nrows= m.nrows * m.ncols;
      ncols= n;
    }
    }
  }
      
  std::size_t rows_size() const { 
    //    std::cerr<<"nrows of scilab_matrix: "<<nrows<<std::endl;
    return nrows * element_size(); } // scilab matrices are stored column wise (as in Fortran)
 
  virtual std::size_t sizeof_data()const{return  ncols*nrows * element_size();}

  virtual  ~scilab_matrix(){}
};


bool is_function_or_string(scilab_var const&  var){ 
  bool res;
  std::cerr<<"in is_function_or_string type :"<<var.get_type()<<std::endl;
  switch(var.get_type()){
  case sci_c_function : { res= true; break;}
  case sci_strings : { 
    scilab_matrix const& as_matrix(dynamic_cast<scilab_matrix const&> (var));
    res = (as_matrix.nrows == 1) && (as_matrix.ncols == 1);
    break;
  }
  default: { res= false; }
  }
  return res;
}


struct scilab_real_matrix : scilab_matrix {
  explicit scilab_real_matrix(int nr=1, int nc=1){type=sci_matrix; nrows=nr; ncols=nc;}
  virtual  std::size_t element_size() const { return sizeof(double) ;}
  virtual  ~scilab_real_matrix(){}
};

struct scilab_int8_matrix : scilab_matrix {

  virtual  std::size_t element_size() const { return 1 ;}
  virtual  ~scilab_int8_matrix(){}
};
struct scilab_complex_matrix : scilab_matrix {

  virtual  std::size_t element_size() const { return 2*sizeof(double) ;}
  virtual  ~scilab_complex_matrix(){}
};

struct scilab_string_matrix : scilab_matrix {

  virtual  std::size_t element_size() const { std::cerr<<"unimplemented string matrix element size"; return 0;}
  virtual  ~scilab_string_matrix(){}
};

typedef int* sci_addr_t;
struct scilab_allocated_var : virtual scilab_var{

  static scilab_allocated_var* get(int i);
  static scilab_allocated_var* create(scilab_var const& model, std::size_t n);
  
  virtual void const* data()const=0;
  virtual void* data()=0;
  virtual void const* data_chunk(std::size_t) const=0;
  virtual void* data_chunk(std::size_t)=0;
  virtual std::size_t data_chunk_size()const {return static_cast<char const*>(data_chunk(1))- static_cast<char const*>(data_chunk(0));}
  virtual ~scilab_allocated_var(){}
  virtual bool function_name_or_ptr()const=0;
};
scilab_var* scilab_var::create(char const* type_name, int nrows, int ncols){
  scilab_matrix* res;
  res= new scilab_real_matrix();
  res->set_type(type_name);
  //  std::cerr<<"create var of type:"<<type_name<<'\n';
  res->nrows= nrows;
  res->ncols= ncols;
  return res;
}

template< typename ScilabVar>
struct scilab_allocated_concrete_var : virtual scilab_allocated_var,  ScilabVar {

  explicit scilab_allocated_concrete_var<ScilabVar>(sci_addr_t a):addr(a){
    err= getVarType( pvApiCtx, addr, &(this->type) );
    this->read_from_stack();
  }


  // if n>0 : alloc n times the model, if n=0, alloc 1/n times 
  // model rowsM x colsM -> n>1 -> alloc rows= rowsM x colsM cols= n n==0 alloc rows=rowsM cols=1
  explicit scilab_allocated_concrete_var<ScilabVar>( ScilabVar const& model, std::size_t const n=1);

  virtual void const* data()const {return data_ptr.opaque; }
  virtual void* data(){    return data_ptr.opaque; }
  void const* data_chunk(std::size_t i) const { return static_cast<void const*>(data_ptr.as_char+ ScilabVar::rows_size()*i);}
  void* data_chunk(std::size_t i) { return static_cast<void*>(data_ptr.as_char+ ScilabVar::rows_size()*i);}
  
  void read_from_stack();

  SciErr err;
  sci_addr_t addr;

  union {
    double* as_double;
    int* as_int;
    char* as_char;
    char** as_char_ptr_ptr;

    void* opaque;
    int as_sci_c_function;
  }data_ptr;

  virtual ~scilab_allocated_concrete_var<ScilabVar>(){}
  virtual bool function_name_or_ptr()const { return false;}

};

template<> void scilab_allocated_concrete_var<scilab_real_matrix>::read_from_stack(){
  
  err= getMatrixOfDouble(pvApiCtx, addr, &nrows, &ncols, &data_ptr.as_double);
}

template<> void scilab_allocated_concrete_var<scilab_int8_matrix>::read_from_stack(){
  err= getMatrixOfInteger8(pvApiCtx, addr, &nrows, &ncols, &data_ptr.as_char);
}
template<> void scilab_allocated_concrete_var<scilab_string_matrix>::read_from_stack(){
  getAllocatedMatrixOfString(pvApiCtx, addr, &nrows, &ncols, &data_ptr.as_char_ptr_ptr);
}
template<> void scilab_allocated_concrete_var<scilab_c_function>::read_from_stack(){
  double* unused;
  // c'est n'imp : macro qui fait return 0 et position nécessaire :(
  //  int ref(
	  //  GetRhsVar(1, EXTERNAL_DATATYPE, &nb_lhs, &nb_rhs, &data_ptr.as_sci_c_function);
}


template<> bool scilab_allocated_concrete_var<scilab_string_matrix>::function_name_or_ptr()const{
  return ncols == 1 && nrows == 1 ;
}
template<> bool scilab_allocated_concrete_var<scilab_c_function>::function_name_or_ptr()const{
  return true;
}
template<>  void const* scilab_allocated_concrete_var<scilab_c_function>::data_chunk(std::size_t i) const {  return 0; }
template<>  void * scilab_allocated_concrete_var<scilab_c_function>::data_chunk(std::size_t i) {  return 0; }


template<> void scilab_allocated_concrete_var<scilab_complex_matrix>::read_from_stack(){
  double* unused;
  err= getComplexMatrixOfDouble(pvApiCtx, addr, &nrows, &ncols, &data_ptr.as_double, &unused);// for now, assume imaginary part is stored after real part. 
}
template<> scilab_allocated_concrete_var<scilab_real_matrix>::scilab_allocated_concrete_var(scilab_real_matrix const& model, std::size_t const n ){
  this->set_from_model(model,n);
  //  std::cerr<<" allocMatrixOfDouble(pvApiCtx, ++"<<current_top<<", nrows, ncols, &data_ptr.as_double);\n";
  err= allocMatrixOfDouble(pvApiCtx, ++current_top, nrows, ncols, &data_ptr.as_double);
}
template<>  scilab_allocated_concrete_var<scilab_int8_matrix>::scilab_allocated_concrete_var(scilab_int8_matrix const& model, std::size_t const n ){
  std::cerr<<"unimplemented int8 returned value\n";
}
template<>  scilab_allocated_concrete_var<scilab_string_matrix>::scilab_allocated_concrete_var(scilab_string_matrix const& model, std::size_t const n ){
  std::cerr<<"unimplemented string returned value\n";
}

template<>  scilab_allocated_concrete_var<scilab_complex_matrix>::scilab_allocated_concrete_var(scilab_complex_matrix const& model, std::size_t const n ){
  std::cerr<<"unimplemented complex returned value\n";
}


scilab_allocated_var* scilab_allocated_var::create(scilab_var const& m, std::size_t n){
  scilab_allocated_var* res;
  switch(m.get_type()){
  case sci_matrix : {
    //    std::cerr<<" scilab_allocated_var::create sci_matrix nrows:"<<(dynamic_cast<scilab_real_matrix const&>(m).nrows)<<" ncols:"<<(dynamic_cast<scilab_real_matrix const&>(m).ncols)<<" n:"<<n<<std::endl;
    res= new scilab_allocated_concrete_var<scilab_real_matrix>(dynamic_cast<scilab_real_matrix const&>(m), n);
    break;
  }
  default : {
    std::cerr<<"unimplemented create()\n";
    res=0;
  }
  }
  return res;
}

scilab_allocated_var* scilab_allocated_var::get(int pos){
  scilab_allocated_var* res;
  sci_addr_t addr;
  SciErr err= getVarAddressFromPosition(pvApiCtx, pos, &addr);
  int type;
  err= getVarType(pvApiCtx, addr, &type);
  //    std::cerr<<"type "<<type<<"@"<<pos<<std::endl;
  switch(type) {
  case sci_matrix : { 
    if( isVarComplex(pvApiCtx, addr) ) {
      res= new scilab_allocated_concrete_var<scilab_complex_matrix>(addr);
    } else {
      res= new scilab_allocated_concrete_var<scilab_real_matrix>(addr);
    }
    break;
  }
  case sci_ints : { // TODO int subtypes
    res= new scilab_allocated_concrete_var<scilab_int8_matrix>(addr);
    break;
  }
  case sci_strings : {
    res = new scilab_allocated_concrete_var<scilab_string_matrix>(addr);
    break;
  }
  case sci_c_function : {
    res = new scilab_allocated_concrete_var<scilab_c_function>(addr);
    break;
  }
  default : { std::cerr<<"unimplemented type in scilab_allocated_var fatory !!!\n";}
  }
  return res;
}

/*
  wrapper on a native c function or a scilab macro called on scilab variables.

  constructed on :
  - scilab variable for the function (external native function name, sci_c_function (buggy) or macro name)scilab matrices of arguments
  - expected lhs

  upon construction, allocate scilab result variables and computes all necessary meta data:
  
  std::vector<std::size_t> args_sizes(function_rhs), args_nb(function_rhs), res_sizes(Lhs);
  std::vector<void const*> args_mem(function_rhs);
  std::vector<void**> res_mem(Lhs);


  for native c function
*/
struct wrapper {

 
  typedef std::vector<std::size_t> sizes_container;

  template<typename VarsIt>
  wrapper(VarsIt args_begin, VarsIt function_it, VarsIt args_end, std::size_t function_lhs)
    :function_rhs(std::distance(args_begin, function_it)), res_sizes(function_lhs){
    for(VarsIt it(args_begin); it != function_it; ++it) {
      register_arg(dynamic_cast<scilab_matrix const&> (**it));
    }
    //    std::cerr<<"**function_it with var of type"<<dynamic_cast<scilab_allocated_var const&>(**function_it).get_type()<<std::endl;
    n= *std::max_element(args_nb.begin(), args_nb.end());
    get_function(dynamic_cast<scilab_allocated_var const&>(**function_it));
    alloc_complete_results(function_it+1, args_end, function_lhs);
  }

  void register_arg(scilab_matrix const& m) { 
    args_data.push_back((dynamic_cast<scilab_allocated_var const&> (m)).data());
    args_sizes.push_back( m.rows_size());
    args_nb.push_back(m.ncols);
    scilab_function_rhs_models.push_back(&m);
  }

  void operator()(void const** args, void ** res)  { 
    // std::cerr<<"wrapped args "<<*(double*)*args<<" wrapped_res "<<*(double*)*res<<std::endl;
    (*this.*(this->f_ptr))( args, res ); 
    // std::cerr<<"after call wrapped args "<<*(double*)*args<<" wrapped_res "<<*(double*)*res<<std::endl;
  }

  struct handle {
    handle(wrapper& r):w(r){}
    void operator()(void const** args, void ** res) const {
      //      std::cerr<<"habdle wrapped args "<<*(double*)*args<<" handle wrapped_res "<<*(double*)*res<<std::endl;

      w( args, res ); }
    wrapper& w;
  };

  handle get_handle() {return handle(*this); }

  typedef union {
    double* double_ptr;
    void* opaque_ptr;
  } union_of_ptrs_t;

  template<typename VarPtrIt>
  void alloc_complete_results(VarPtrIt res_type_begin, VarPtrIt res_type_end, std::size_t nb_lhs){
    SciErr err;
    char const* type_name="constant";
    std::vector<char const*> type_names(Lhs, "constant");
    typedef std::pair<std::size_t, std::size_t> dim_t;
    std::vector<dim_t>dims(Lhs, dim_t(1,1));

    if(res_type_begin != res_type_end){
      if((**res_type_begin).get_type()==sci_strings){
	//	std::cerr<<"we have a type lhs arg\n";
	// process it 	scilab_allocated_concrete_var<scilab_string_matrix> const& str(dynamic_cast<scilab_allocated_concrete_var<scilab_string_matrix> const&>(v));
	++res_type_begin;
      }
      if(res_type_begin != res_type_end){
	if((**res_type_begin).get_type()==sci_matrix) {
	  //	  std::cerr<<"we have a dim lhs arg\n";
	  scilab_allocated_concrete_var<scilab_real_matrix> const& m(dynamic_cast<scilab_allocated_concrete_var<scilab_real_matrix> const&>(**res_type_begin));

	  switch(m.ncols) {
	  case 2:{
	    //	    std::cerr<<"we have rows and cols\n";
	    for(std::size_t i(0); i< m.nrows && i< dims.size(); ++i){
	      dims[i].first=static_cast<std::size_t>(m.data_ptr.as_double[i]);
	      dims[i].second=static_cast<std::size_t>(m.data_ptr.as_double[i+m.nrows]);

	    }

	    break;
	  }
	  case 1: {
	    //	    std::cerr<<"we have rows \n";
	    for(std::size_t i(0); i<m.nrows && i< dims.size(); ++i){
	      dims[i].first=static_cast<std::size_t>(m.data_ptr.as_double[i]);
	    }
	    break;
	  }
	  default : {
	    std::cerr<<"bad nb of cols cols for dim arg : should have 1(rows) ou 2 (rows and cols) cols\n";
	  }
	  }
	}
      }
    }
    for(std::size_t i(0); i!= Lhs; ++i){
      scilab_function_lhs_models.push_back(scilab_var::create(type_names[i], dims[i].first, dims[i].second));
    }
    res_data.resize(nb_lhs);
    res_sizes.resize(nb_lhs);
    
    // we want to have to result var at hand before calling the scilab macro so we must create it now before the args
    for(std::size_t i(0); i!= nb_lhs; ++i) {
      union_of_ptrs_t u;
      //      std::cerr<<"alloc matrix of size "<< 1 <<" "<<n<<std::endl;
      scilab_collections_of_lhs.push_back( scilab_allocated_var::create(*scilab_function_lhs_models[i], n));
      //      scilab_collections_of_lhs[i]->debug();
      res_data[i]= scilab_collections_of_lhs[i]->data();
      res_sizes[i]= scilab_collections_of_lhs[i]->data_chunk_size();
    }

    //    std::cerr<<"allocating"<<nb_lhs<<" res scilab variables with "<<std::distance(res_type_begin, res_type_end)<<" type informations\n";
    
  }


  int get_function(scilab_allocated_var const& v) {
    //    std::cerr<<"in get_function with var of type"<<v.type<<std::endl;
    function.to_call=0;
    switch(v.get_type()) {
    case sci_c_function : {
      int m_fun, n_fun;
      GetRhsVar(2, EXTERNAL_DATATYPE, &m_fun, &n_fun, &scilab_function);
      f_ptr = &wrapper::macro<false>;
      break;
    }
    case sci_strings : {
      scilab_allocated_concrete_var<scilab_string_matrix> const& str(dynamic_cast<scilab_allocated_concrete_var<scilab_string_matrix> const&>(v));
      char * funName =str.data_ptr.as_char_ptr_ptr[0];
      int found;
      found=SearchInDynLinks(funName, &function.to_load);
      f_ptr= &wrapper::native_function;
      if(found == -1) 
	{ 
	  function.to_call=0;
	  scilab_function_name= funName;
	  scilab_function_name_length= std::strlen(scilab_function_name);
	  f_ptr= &wrapper::macro<true>;
	  //SciError( a pa trouvé);
	    
	}
    }
    }
  }



  void const*const* args_data_begin() const { return &args_data[0];}
  std::size_t const* args_sizes_begin() const { return &args_sizes[0];}
  std::size_t const* args_nb_begin() const { return &args_nb[0];}
  std::size_t tasks_nb() const { return n;}
  void * const* res_data_begin() { return &res_data[0];}
  std::size_t const* res_sizes_begin() const { return &res_sizes[0];}

  std::size_t nb_rhs()const{ return function_rhs;}

  template<bool byName>
  void macro(void const** args, void ** res)  {
    // rhs models from 
    int save_Nbvars= Nbvars, save_current_top=current_top;
    for( std::vector<scilab_var const*>::const_iterator it(scilab_function_rhs_models.begin())
	   ; it != scilab_function_rhs_models.end(); ++it, ++args){

      scilab_allocated_var* scilab_arg= scilab_allocated_var::create(**it,0);
      //            std::cerr<<"creating temp arg for macro of type"<<scilab_arg->get_type()<<std::endl;
      memcpy(scilab_arg->data(), *args,scilab_arg->sizeof_data());
      //            std::cerr<<"value:"<<*((double*)scilab_arg->data())<<std::endl;
    }

    int  sci_rhs = scilab_function_rhs_models.size();
    int  sci_lhs = scilab_function_lhs_models.size();

    std::size_t dummy_vars(0);
    for( ;sci_rhs+dummy_vars < sci_lhs+max_safety_lhs; ++dummy_vars){
      //      std::cerr<<"alloc dummy var";
      scilab_allocated_concrete_var<scilab_real_matrix> dummy(scilab_real_matrix());
    }



    int sci_arg_pos = current_top-sci_rhs+1;
    //  fprintf(stderr, " Nbvars = %d\n", 	Nbvars);

    //  std::cerr<<"sci_rhs :"<<sci_rhs<<" Nbvars: "<<Nbvars<<std::endl;
    Nbvars = Rhs+Lhs+sci_rhs; // sinon MALLOC incorrect Size Error File src/c/stack2.c Line 3311
    // r = scilabfoo(x)	
    // C2F(scifunction) call a scilab function
    //  std::cerr<<"sci_rhs :"<<sci_rhs<<" Nbvars: "<<Nbvars<<std::endl;
    if(byName){
      C2F(scistring)(&sci_arg_pos, scilab_function_name, &sci_lhs, &sci_rhs, scilab_function_name_length);
    } else {
      C2F(scifunction)(&sci_arg_pos, &scilab_function, &sci_lhs, &sci_rhs);
    }
    //    fprintf(stderr, " Nbvars = %d\n", 	Nbvars);
    // result r is now on first position on stack
    {
      double* tmp;
      int* addr;
      int rm1, rn1;
      //      std::cerr<<"Nbvars:"<<Nbvars<<"Top:"<<Top<<"curent_top:"<<current_top<<std::endl;
      Nbvars = Rhs+Lhs+sci_rhs+dummy_vars;
      int res_pos=Rhs+Lhs+1; // 

      for( std::vector<scilab_var const*>::iterator it(scilab_function_lhs_models.begin()); it != scilab_function_lhs_models.end(); ++it, ++res_pos, ++res){

	//      std::cerr<<"res_pos :"<<res_pos<<" Nbvars: "<<Nbvars<<std::endl;
        scilab_allocated_var* scilab_res= scilab_allocated_var::get(res_pos);
	/*	for(int tmpi(0); tmpi <  (scilab_res->sizeof_data()/sizeof(double)); ++tmpi)
	{ std::cerr<<"res:"<< (*(((double*)scilab_res->data())+tmpi))<<std::endl; }*/
	memcpy(*res, scilab_res->data(), scilab_res->sizeof_data());
      }
      Nbvars= save_Nbvars;
      current_top=save_current_top;
    }

    //    std::cerr<<"calling a macro"<<(byName ? "by name" : "by ref")<<std::endl;
  }
  void native_function(void const** args, void ** res) {  
    // std::cerr<<"before function.to_call "<<*(double*)*args<<" wrapped_res "<<*(double*)*res<<std::endl;
    function.to_call(args, res); 
  
    //  std::cerr<<"calling a native compiled function "<<(*(double*)*res)<<std::endl;
  }


  static unsigned int const max_safety_lhs = 20; // we prealloc as much scilab var more than requested lhs in case the scilab macro call back returns more thant requested.

  std::size_t function_rhs, n;
  sizes_container args_sizes, args_nb, res_sizes;
  std::vector<void const*> args_data;
  std::vector<void *> res_data;


  void(wrapper::*f_ptr)(void const** args, void ** res);

  int scilab_function;
  char* scilab_function_name;
  std::size_t scilab_function_name_length;
  typedef void (*function_to_call_t)(void const*const*, void*const*);
  typedef void (*wrapper_function_t)(double const*, double*);
  typedef void (*loaded_function_t)();

  union{
    function_to_call_t to_call;
    loaded_function_t to_load;
    wrapper_function_t wrapper;
  } function;

  std::vector<scilab_var const*> scilab_function_lhs_models, scilab_function_rhs_models; 
  std::vector<scilab_allocated_var*>scilab_collections_of_lhs;

};


int  C2F(sci_mc_apply)(char *fname,unsigned long fname_len) 
{
  typedef std::vector<scilab_allocated_var*> vars_container_t;
  current_top= Rhs;
  vars_container_t stack(Rhs);

  for(int i(0); i!=Rhs; ++i){
    stack[i]= scilab_allocated_var::get(i+1);
  }
  vars_container_t::iterator function_arg= std::find_if(stack.begin(), stack.end(), std::mem_fun(&scilab_allocated_var::function_name_or_ptr));
  //  std::cerr<<"function @"<<std::distance(stack.begin(), function_arg)<<std::endl;
  wrapper w(stack.begin(), function_arg, stack.end(), Lhs);
  bool const with_threads(w.function.to_call);
  make_parallel_wrapper(w.args_data_begin(), w.args_sizes_begin(), w.args_nb_begin(), w.nb_rhs(), w.tasks_nb()
			,  w.res_data_begin(), w.res_sizes_begin(), Lhs, w.get_handle())(with_threads);

  for(std::size_t i(0); i != Lhs; ++i) {
    LhsVar(i+1)=Rhs+i+1;
  }


  PutLhsVar(); // to be moved to gateway

  return 0;
}
