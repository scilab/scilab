function [s]=center(x,orient)
//
//This function  computes s,  the centered version  of the
//numerical matrix x.
//
//For a vector  or a matrix x, s=center(x)  returns in the
//(i,j)   coefficient   of   the   matrix  s   the   value
//(x(i,j)-xbar), where  xbar is the mean of  the values of
//the coefficients of x.
//
//s=center(x,'r') (or, equivalently, s=center(x,1)) is the
//rowwise center reduction of  the values of x. It returns
//in  the entry  s(i,j) the  value  (x(i,j)-xbarv(j)) with
//xbarv(j) the mean of the values of the j column.
//
//s=center(x,'c') (or, equivalently, s=center(x,2)) is the
//columnwise  centre reduction  of  the values  of x.   It
//returns in the  entry s(i,j) the value (x(i,j)-xbarh(i))
//with xbarh(i) the mean of the values of the i row.
//
//author: carlos klimann
//
//date: 2001-10-05
//
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if (rhs<1)|(rhs>2) then error('center requires one or two inputs.'), end
  [m n]=size(x);
  if rhs==1
    xbar=(sum(x)/(m*n))
    s=x-(ones(m,n)*xbar)
  elseif orient=='c'|orient==2 then
    xbar=sum(x,'c')/n
    s=x-(xbar*ones(1,n))
  elseif orient=='r'|orient==1 then
    xbar=sum(x,'r')/m
    s=x-(ones(m,1)*xbar)
  else error('Second center parameter must be r, c, 1 or 2'),
  end
endfunction
