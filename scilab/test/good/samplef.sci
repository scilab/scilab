function s=samplef(n,X,f,orient)
// 
//This function gives s, a  vector of lenght n.  It contains
//a  sample of  n  extractions, with  replacement, from  the
//vector  (or  matrix)  X,  each element  counted  with  the
//frequence given by the corresponding value in vector f.
//
//s=samplef(n,X,f)   (or  s=samplef(n,X,f,'*'))   returns  a
//vector s whose values are a random sample of n values from
//X,   each  value   with  a   probability  to   be  sampled
//proportional  to the corresponding  value of  f, extracted
//with replacement, from X.  f must have same lenght than X.
//
//s=samplef(n,X,f,'r')           (or,          equivalently,
//s=samplef(n,X,f,1))    returns    a    matrix   of    type
//size(X,'r')xn.  It contains a random sample of n rows from
//X, each row with  a probability to be sampled proportional
//to   the  corresponding   value  of   f,   extracted  with
//replacement, from the  rows of X. The lenght  of f must be
//equal to the number of rows of X.
//
//s=samplef(n,X,f,'c')           (or,          equivalently,
//s=samplef(n,X,f,2))    returns    a    matrix   of    type
//nxsize(X,'c').  It  contains a random sample  of n columns
//from  X, each  column  with a  probability  to be  sampled
//proportional  to the corresponding  value of  f, extracted
//with replacement, from the columns  of X.  The lenght of f
//must be equal to the number of columns of X.
//
//author: carlos klimann
//
//date: 2003-08-28
  [lhs,rhs]=argn(0)
  if rhs<3 | rhs>4 then error('samplef requires three or four inputs.'), end
  if X==[]| (or(f==0)) then s=[]; return;end
  sizx=size(X)
  val=cumsum([0;f(:)])
  y=grand(n,1,'uin',1,val($))
  ind=dsearch(y,val,'c')
  if rhs==3 then 
    orient='*'
  end
  if orient=='*' then
    if size(f,'*')<>prod(sizx) then
      error('In this case dimensions of second and third parameters must be equal'), 
    end
    s=X(ind)
    return
  end
  if orient=='r'|orient==1 then
    if size(f,'*')<>sizx(1) then
      error('Third parameter must be equal to number of rows of second parameter'), 
    end
    s=X(ind,:)
    return
  end
  if orient=='c'|orient==2 then
    if size(f,'*')<>sizx(2) then
      error('Third parameter must be equal to number of columns of second parameter'), 
    end
    s=X(:,ind)
    return
  end
  error('Fourth parameter must be *, r, c, 1 or 2')
endfunction

