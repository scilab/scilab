function [s]=variance(x,orien)
//
//This function computes  the  variance  of  the values of  a  vector or
//matrix x.
//
//For a vector or   a matrix x,  s=variance(x)  returns in  the scalar s  the
//variance of all the entries of x.
//
//s=variance(x,'r')    (or,  equivalently,    s=variance(x,1)) is     the  rowwise
//variance. It returns in each entry of the row vector s the variance of
//each column of x.
//
//s=variance(x,'c') (or, equivalently, s=variance(x,2)) is the columnwise standard
//deviation.   It  returns in  each  entry  of the   column vector y the
//variance of each row of x.
//
//author: carlos klimann
//
//date: 1999-05-12
//
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0 then error('variance requires at least one input.'), end
  [m n]=size(x);
  if rhs==1
    s=(sum((x-mean(x)).^2))/(m*n-1)
  elseif orien=='c'|orien==2 then
    s=(sum((x-mean(x,'c')*ones(x(1,:))).^2,'c')/(n-1));
  elseif orien=='r'|orien==1 then
    s=(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/(m-1));
  else
    error('2rd argument of variance must be equal to c, r, 1 or 2');
  end
endfunction
