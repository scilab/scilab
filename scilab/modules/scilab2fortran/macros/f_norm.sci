function [stk,nwrk,txt,top]=f_norm(nwrk)
//!purpose
//  Scilab norm function translation
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

s2=stk(top-rhs+1);
//
p='2'
if rhs==2 then
  p=stk(top);p=p(1);
end

if p=='2' then //norme 2
if s2(4)=='1'&s2(5)=='1' then
  if s2(2)=='2' then s2(1)='('+s2(1)+')',end
  stk=list('abs('+s2(1)+')',s2(2),s2(3),s2(4),s2(5))
elseif s2(4)=='1'|s2(5)=='1' then
  [s2,nwrk,txt]=typconv(s2,nwrk,'1')
  nwrk=dclfun(nwrk,'dnrm2','1')
  out=callfun(['dnrm2',mulf(s2(4),s2(5)),s2(1),'1'],'1')
  stk=list(out,'-1','1','1','1')
else
  [s2,nwrk,txt]=typconv(s2,nwrk,'1')
  n=s2(4);m=s2(5)
  if n==m then
    n1=n
    n2=n
  else
    n1='min('+addf(n,'1')+','+m+')'
    n2='min('+n+','+m+')'
  end
  [errn,nwrk]=adderr(nwrk,'echec du calcul de la norme')
  [s,nwrk,t1]=getwrk(nwrk,'1','1',n1)
  [e,nwrk,t2]=getwrk(nwrk,'1','1',m)
  [wrk,nwrk,t3]=getwrk(nwrk,'1','1',n)
  txt=[t1;t2;t3;
    gencall(['dsvdc',s2(1),n,n,m,s,e,'work',n,'work',m,wrk,'00','ierr']);
    genif('ierr.ne.0',[' ierr='+string(errn);' return'])]
  stk=list(s,'0','1','1','1')
end
elseif p=='1' then
  [s2,nwrk,txt]=typconv(s2,nwrk,'1')
  nwrk=dclfun(nwrk,'dasum','1')
  out=callfun(['dasum',mulf(s2(4),s2(5)),s2(1),'1'],'1')
  stk=list(out,'0','1','1','1')
elseif p=='inf' then
  [s2,nwrk,txt]=typconv(s2,nwrk,'1')
  if part(s2(1),1:4)==work then
    out=callfun(['abs',part(s2(1),1:length(s2(1))-1)+'-1+'+..
                    callfun(['idamax',mulf(s2(4),s2(5)),s2(1),'1'])+')'])
  else
    out=callfun(['abs',s2(1)+'('+..
          callfun(['idamax',mulf(s2(4),s2(5)),s2(1),'1'])+')'])
  end
  stk=list(out,'0','1','1','1')
else
  [s2,nwrk,txt]=typconv(s2,nwrk,'1')
  [t,nwrk,t1]=getwrk(nwrk,'1','1','1')
  [lbl,nwrk]=newlab(nwrk)
  tl1=string(10*lbl);
  var='ilb'+tl1;
  if part(s2(1),1:4)==work then
    t2=t+'='+t+'('+part(s2(1),1:length(s2(1))-1)+'+'+var+'))**'+p
  else
    t2=t+'='+t+'('+s2(1)+'('+var+'))**'+p
  end
  txt=[txt;t1;
       t+' = 0.0d0';
       ' do '+tl1+' '+var+' = 0'+','+subf(mulf(s2(4),s2(5),'1'));
       indentfor(t2);
       part(tl1+'    ',1:6)+' continue']
  stk=list('('+t+')**(1/'+p+')','0','1','1','1')
end
//
top=top-rhs+1
endfunction
