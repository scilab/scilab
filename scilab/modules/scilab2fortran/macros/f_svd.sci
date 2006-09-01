function [stk,nwrk,txt,top]=f_svd(nwrk)
//!purpose
//  Scilab svd function translation
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
nam='svd'
s2=stk(top-rhs+1)

v=s2(1)
it2=prod(size(v))-1
if it2<>0 then  error(nam+' complex --> not implemented'),end

[s2,nwrk,t0]=typconv(s2,nwrk,'1')
n=s2(4);m=s2(5)
if n==m then
  n1=n
  n2=n
else
  n1='min('+addf(n,'1')+','+m+')'
  n2='min('+n+','+m+')'
end

if lhs==1 then
  [errn,nwrk]=adderr(nwrk,'SVD computation fails')
  [out,nwrk,t1]=outname(nwrk,'1','1',n1)
  [e,nwrk,t2]=getwrk(nwrk,'1','1',m)
  [wrk,nwrk,t3]=getwrk(nwrk,'1','1',n)
  txt=[t0;t1;t2;t3;
       gencall(['dsvdc',s2(1),n,n,m,out,e,'work',n,'work',m,wrk,'00','ierr']);
       genif('ierr.ne.0',[' ierr='+string(errn);' return'])]
  stk=list(out,'-1','1','1',n1)
  [nwrk]=freewrk(nwrk,wrk)
  [nwrk]=freewrk(nwrk,e)
elseif lhs==3 then
  [errn,nwrk]=adderr(nwrk,'SVD computation fails')
  [o,nwrk,t1]=outname(nwrk,['1','1','1'],[n,n,m],[n,m,m])
  [d,nwrk,t2]=getwrk(nwrk,'1','1',n1)
  [e,nwrk,t3]=getwrk(nwrk,'1','1',m)
  txt=[t0;t1;t2;t3;
      gencall(['dsvdc',s2(1),n,n,m,d,e,o(3),n,o(1),m,o(2),'11','ierr']);
      genif('ierr.ne.0',[' ierr='+string(errn);' return']);
      gencall(['dset',mulf(m,n),'0.0d0',o(2),'1']);
      gencall(['dcopy',n1,d,'1',o(2),addf(n,'1')])];
  [nwrk]=freewrk(nwrk,d)
  [nwrk]=freewrk(nwrk,e)
  stk=list(list(o(1),'-1','1',m,m),list(o(2),'-1','1',n,m),..
           list(o(3),'-1','1',n,n))
else
  [errn,nwrk]=adderr(nwrk,'SVD fails')
  [o,nwrk,t1]=outname(nwrk,['0','1','1','1'],['1',n,n,m],['1',n,m,m])
  [d,nwrk,t2]=getwrk(nwrk,'1','1',n1)
  [e,nwrk,t3]=getwrk(nwrk,'1','1',m)
  txt=[t0;t1;t2;t3;
      gencall(['dsvdc',s2(1),n,n,m,d,e,o(4),n,o(2),m,o(3),'11','ierr']);
      genif('ierr.ne.0',[' ierr='+string(errn);' return']);
      gencall(['dset',mulf(m,n),'0.0d0',o(3),'1']);
      gencall(['dcopy',n1,d,'1',o(3),addf(n,'1')])];
  tol=e
  if rhs==1 then
    nwrk=dclfun(nwrk,'d1mach','1')
    t0=' '+tol+'='+mulf(mulf(mulf('d1mach(4)',m),n),d)
  else
    tol1=stk(top)
    t0=' '+tol+'='+mulf(mulf(mulf(tol1(1),m),n),d)
  end
  [lbl,nwrk]=newlab(nwrk)
  tl1=string(10*lbl);
  var='ilb'+tl1;
  [lbl,nwrk]=newlab(nwrk)
  tl2=string(10*lbl);
  t1=[' '+o(1)+'='+var;
      genif(part(d,1:length(d)-1)+'+'+var+'-1).le.'+tol,' goto '+tl2)]
  txt=[txt;t0;
     ' do '+tl1+' '+var+' = 0'+','+subf(n2,'1');
     indentfor(t1);part(tl1+'    ',1:6)+' continue';
     ' '+o(1)+'='+n2;
     part(tl2+'    ',1:6)+' continue']
  [nwrk]=freewrk(nwrk,d)
  [nwrk]=freewrk(nwrk,e)
  stk=list(list(o(1),'-1','0','1','1'),list(o(2),'-1','1',m,m),..
           list(o(3),'-1','1',n,m),list(o(4),'-1','1',n,n))

end
top=top-rhs+1
endfunction
