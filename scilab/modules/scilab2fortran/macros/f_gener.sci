function [stk,nwrk,txt,top]=f_gener(nam,nwrk,targ)
//!purpose
//  Generic Scilab function translation
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
[lhs,rhs]=argn(0);if rhs==2 then targ=['1','1'],end
tin=targ(1);tout=targ(2)
cnvf='dble'
if tin=='0' then cnvf='int',end
//
s2=stk(top)
if s2(4)=='1'&s2(5)=='1' then
//cas d'un argument scalaire
  if s2(3)<>tin then s2(1)=cnvf+'('+s2(1),')',end
  stk=list(nam+'('+s2(1)+')','0',tout,s2(4),s2(5))
  return
end

if s2(3)<>tin then cnv=%t,else cnv=%f,end
if part(s2(1),1:5)=='work(' then
  pti=part(s2(1),6:length(s2(1))-1)
  [outn,nwrk,txt]=outname(nwrk,tout,s2(4),s2(5))
  in='work'
elseif part(s2(1),1:6)=='iwork(' then
  pti=part(s2(1),6:length(s2(1))-1)
  [outn,nwrk,txt]=outname(nwrk,tout,s2(4),s2(5))
  in='iwork'
else
  pti='0'
  [outn,nwrk,txt]=outname(nwrk,tout,s2(4),s2(5))
  in=s2(1)
end
if part(outn,1:5)=='work(' then 
  pto1=part(outn,6:length(outn)-1),
  out='work'
elseif part(outn,1:6)=='iwork(' then 
  pto1=part(outn,7:length(outn)-1),
  out='iwork'
else
  out=outn
  pto1='0'
end

if s2(4)=='1'|s2(5)=='1' then
  [lbl,nwrk]=newlab(nwrk)
  tl1=string(10*lbl);
  var='ilb'+tl1;
  if cnv then
    t1=' '+out+'('+addf(pto1,var)+')='+..
                              nam+'('+cnvf+'('+in+'('+addf(pti,var)+')))'
  else
    t1=' '+out+'('+addf(pto1,var)+')='+nam+'('+in+'('+addf(pti,var)+'))'
  end
  txt=[txt;' do '+tl1+' '+var+' = 0,'+subf(mulf(s2(4),s2(5)),'1');
           indentfor(t1);part(tl1+'    ',1:6)+' continue']

else
  [lbl,nwrk]=newlab(nwrk)
  tl2=string(10*lbl);
  var2='ilb'+tl2;
  [lbl,nwrk]=newlab(nwrk)
  tl1=string(10*lbl);
  var1='ilb'+tl1;
  if out=='work' then
     t1=' '+out+'('+addf(pto1,addf(var2,mulf(var1,s2(4))))+') = '
  else
     t1=' '+out+'('+var2'+','+var1+') = '
  end
  if in=='work' then
     iar=in+'('+addf(pti,addf(var2,mulf(var1,s2(4))))+')'
  else
     iar=in+'('+var2+','+var1+')'
  end
  if cnv then
    t1=t1+nam+'('+cnvf+'('+iar+'))'
  else
    t1=t1+nam+'('+iar+')'
  end
  txt=[txt;' do '+tl1+' '+var1+' = 0,'+subf(s2(5),'1');
           indentfor([' do '+tl2+' '+var2+' = 0,'+subf(s2(4),'1');
                      indentfor(t1);
                      part(tl2+'    ',1:6)+' continue']);
           part(tl1+'    ',1:6)+' continue'];

end
stk=list(outn,'-1','1',s2(4),s2(5))
endfunction
