function [s]=wcenter(x,orient)
//
//This function computes  s, the centered weighted version
//of the numerical matrix x.
//
//For a  vector or a  matrix x, s=wcenter(x)  returns in
//the  (i,j)  coefficient  of   the  matrix  s  the  value
//(x(i,j)-xbar)/sigma,  where  xbar  is  the mean  of  the
//values of  the coefficients of x and  sigma his standard
//deviation.
//
//s=wcenter(x,'r')  (or, equivalently, s=wcenter(x,1))
//is the rowwise  centre reduction of the values  of x. It
//returns    in    the     entry    s(i,j)    the    value
//(x(i,j)-xbarv(j))/sigmav(j)  with xbarv(j)  the  mean of
//the values  of the j  column and sigmav(j)  the standard
//deviation of the j column of x.
//
//s=wcenter(x,'c')  (or, equivalently, s=wcenter(x,2))
//is  the columnwise  centre  reduction of  the values  of
//x.   It  returns   in   the  entry   s(i,j)  the   value
//(x(i,j)-xbarh(i))/sigmah(i)  with xbarh(i)  the  mean of
//the  values of  the  i row  and  sigmah(i) the  standard
//deviation of the i row of x.
//
//author: carlos klimann
//
//date: 2001-03-27
//
//fixed: 2001-10-03 by ck
//
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if (rhs<1)|(rhs>2) then error('centered requires one or two inputs.'), end
  [m n]=size(x);
  if rhs==1
    xbar=(sum(x)/(m*n))
    s=x-(ones(m,n)*xbar)
    sigma=sqrt(sum(s .^ 2)/(m*n-1))
    s=s/sigma
  elseif orient=='c'|orient==2 then
    xbar=sum(x,'c')/n
    s=x-(xbar*ones(1,n))
    sigma=sqrt(sum((s.^2),'c')/(n-1))
    s=s./(sigma*ones(1,n))
  elseif orient=='r'|orient==1 then
    xbar=sum(x,'r')/m
    s=x-(ones(m,1)*xbar)
    sigma=sqrt(sum((s.^2),'r')/(m-1))
    s=s./(ones(m,1)*sigma)
  else error('Second parameter must be r, c, 1 or 2'),
  end
endfunction
