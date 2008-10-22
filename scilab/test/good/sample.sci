function s=sample(n,X,orient)
//
//This function gives a  vector (or matrix) nx1. It contains
//a random  sample of n extractions,  with replacement, from
//the matrix X.
//
//s=sample(n,X)  (or s=sample(n,X,'*'))  returns a  vector s
//whose  values are  a random  sample  of n  values from  X,
//extracted with replacement, from X .
//
//s=sample(n,X,'r')   (or,   equivalently,  s=sample(n,X,1))
//returns  a matrix  of type  size(X,'r')xn.  It  contains a
//random sample of n  rows, extracted with replacement, from
//the rows of X.
//
//s=sample(n,X,'c')   (or,   equivalently,  s=sample(n,X,2))
//returns  a matrix  of type  nxsize(X,'c').  It  contains a
//random  sample of  n columns,  extracted  with replacement
//from the columns of X.
//
//author: carlos klimann
//
//date: 2003-08-28
  [lhs,rhs]=argn(0)
  if rhs<2 | rhs>3 then error('sample requires two or three inputs.'), end
  if X==[] then s=[]; return;end
  if rhs==2 then 
    orient='*'
  end
  if orient=='*' then
    s=X(grand(n,1,'uin',1,size(X,'*'))),
    return
  end
  if orient=='r'|orient==1 then
    s=X(grand(1,n,'uin',1,size(X,'r')),:)
    return
  end
  if orient=='c'|orient==2 then
    s=X(:,grand(n,1,'uin',1,size(X,'c'))),
    return
  end
  error('orient must be *, r, 1, c or 2 ')
endfunction

