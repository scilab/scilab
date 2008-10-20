function [stk,nwrk,txt,top]=f_size(nwrk)
//!purpose
//  Scilab size function translation
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

txt=[]
if rhs==1 then
  s=stk(top)
  if lhs==1 then
    [out,nwrk,t1]=outname(nwrk,'0','1','2')
    if part(out,1:6)=='iwork(' then
      pti=part(out,7:length(out)-1)
      txt=[' iwork('+pti+') = '+s(4);
	  ' iwork('+addf(pti,'1')+') = '+s(5);t1]
    else
      txt=[' '+out+'(1) = '+s(4);
	  ' '+out+'(2) = '+s(5);]
    end
    stk=list(out,'-1','0','1','2')
  else
    stk=list(list(s(4),'0','0','1','1'),list(s(5),'0','0','1','1'))
  end
else
  s1=stk(top)
  top=top-1
  s=stk(top)
  if s1(1)=='''*''' then
    stk=list(s(4)+'*'+s(5),'1','0','1','1')
  elseif s1(1)=='''r'''| s1(1)=='1' then 
    stk=list(s(4),'1','0','1','1')
  elseif s1(1)=='''c'''| s1(1)=='2' then 	
    stk=list(s(5),'1','0','1','1')
  end
end
endfunction
