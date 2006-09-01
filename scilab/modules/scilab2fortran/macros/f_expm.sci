function [stk,nwrk,txt,top]=f_expm(nwrk)
//!purpose
//  Scilab  expm function translation
//!parameters
// - stk :
//      On entry stk is a global variable of type list
//      entries indexed from top-1+rhs:top give the definition of the rhs
//      function input variables 
//
//      After execution stk(1:lhs) must contain the definition of the
//      lhs returned variables
//
//      stk entries have the following structure:
//      stk(k)=list(definition,type_expr,type_var,nb_lig,nb_col)
//
//      *definition may be:
//         - a character string containing a Fortran  expression with
//           a scalar value ex:'a+2*b-3*c(1);
//         - a character string containing a reference to the first
//           entry of a Fortran array
//                 'a'           if a is a defined matrix
//                 'work(iwn)'   if  variable is stored in the  double
//	                         precision working array work
//                 'iwork(iiwn)' if  variable is stored in the integer
//	                         working array iwork
//      remark: complex array are defined by a definition part
//                  with 2 elements (real and imaginary parts definition)
//      *type_expr a character string: the expression type code (used
//            to indicate need of parenthesis )
//          '2' : the expression is a sum of terms
//          '1' : the expression is a product of factors
//          '0' : the expression is an atome
//          '-1': the value is stored in a Fortran array
//      *type_var a character string: codes the variable fortran type:
//          '1' : double precision
//          '0' : integer
//          '10': character
//
//      *nb_lig (, nb_col) : character strings:number of rows
//              (columns) of the matrix		
//          des chaines de caracteres

// Copyright INRIA
//
//  nwrk : this variable contain information on working arrays, error
//         indicators. It only may be modified by call to scilab functions
//         outname adderr getwrk
//
//  txt  : is a column vector of character string which contain the
//         fortran code associated to the function translation if
//         necessary.
//  top  : an integer 
//         global variable on entry
//         after execution top must be equal to top-rhs+lhs
//!
nam='expm' 
txt=[]  
s2=stk(top)
if (s2(4)=='1'&s2(5)=='1') then
  v=s2(1)
  it2=prod(size(v))-1
  if it2==0 then
    [stk,nwrk,txt,top]=f_gener(nam,nwrk)
  else
     error(nam+' complex is not implemented')
  end
else
  [s2,nwrk,t0]=typconv(s2,nwrk,'1')
  n=s2(4)
  [errn,nwrk]=adderr(nwrk,'exp fails!')
  [out,nwrk,t1]=outname(nwrk,'1',n,n,s2(1))
  [wrk,nwrk,t2]=getwrk(nwrk,'1',mulf(n,addf(mulf('4',n),'5')),'1')
  [iwrk,nwrk,t3]=getwrk(nwrk,'0',mulf('2',n),'1')
  txt=[t0;t1;t2;t3;
      gencall(['dexpm1',n,n,s2(1),out,n,wrk,iwrk,'ierr']);
      genif('ierr.ne.0',[' ierr='+string(errn);' return'])]
  [nwrk]=freewrk(nwrk,wrk)
  [nwrk]=freewrk(nwrk,iwrk)
  stk=list(out,'-1','1',n,n)
end
endfunction
