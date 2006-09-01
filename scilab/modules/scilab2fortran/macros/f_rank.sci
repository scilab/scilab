function [stk,nwrk,txt,top]=f_rank(nwrk)
//!purpose
//  Scilab rank function translation
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
nam='rank'
s2=stk(top-rhs+1)
v=s2(1)
it2=prod(size(v))-1
if it2<>0 then  error(nam+' complex : not implemented'),end

[s2,nwrk,t0]=typconv(s2,nwrk,'1')
n=s2(4);m=s2(5)
if n==m then
  n1=n
  n2=n
else
  n1='min('+addf(n,'1')+','+m+')'
  n2='min('+n+','+m+')'
end
[errn,nwrk]=adderr(nwrk,'echec du calcul du rang')
[s,nwrk,t1]=getwrk(nwrk,'1','1',n1)
[e,nwrk,t2]=getwrk(nwrk,'1','1',m)
[wrk,nwrk,t3]=getwrk(nwrk,'1','1',n)
txt=[t0;t1;t2;t3;
    gencall(['dsvdc',s2(1),n,n,m,s,e,'work',n,'work',m,wrk,'00','ierr']);
    genif('ierr.ne.0',[' ierr='+string(errn);' return'])]
tol=wrk
if rhs==1 then
  nwrk=dclfun(nwrk,'d1mach','1')
  t0=' '+tol+'='+mulf(mulf(mulf('d1mach(4)',m),n),e)
else
  tol1=stk(top)
  t0=' '+tol+'='+mulf(mulf(mulf(tol1(1),m),n),e)
end
[lbl,nwrk]=newlab(nwrk)
tl1=string(10*lbl);
var='ilb'+tl1;
[lbl,nwrk]=newlab(nwrk)
tl2=string(10*lbl);

t1= genif(part(s,1:length(s)-1)+'+'+var+'-1).le.'+tol,' goto '+tl2)
txt=[txt;t0;
     ' do '+tl1+' '+var+' = 0'+','+subf(n2,'1');
     indentfor(t1);part(tl1+'    ',1:6)+' continue';
     ' '+var+'='+n2;
     part(tl2+'    ',1:6)+' continue']
[nwrk]=freewrk(nwrk,e)
[nwrk]=freewrk(nwrk,s)
[nwrk]=freewrk(nwrk,wrk)
stk=list(var,'0','0','1','1')
endfunction
