function [s]=nancumsum(x,orient)
//
//This function returns in scalar or vector s the sum of the
//values (ignoring the NANs) of  a vector or matrix (real or
//complex) x.
//
//This function  for a vector or a  matrix x, s=nancumsum(x)
//(or, equivalently  s=nancumsum(x,'*') returns in  scalar s
//the cumulative sum (ignoring  the NANs) of all the entries
//of x taken columnwise.
//
//s=nancumsum(x,'r')  (or,  equivalently,  s=nancumsum(x,1))
//returns in  the cols(x) sized vector s  the cumulative sum
//(ignoring    the    NANs)    of    the    rows    of    x:
//s(:,i)=nancumsum(x(:,i))
//
//s=nancumsum(x,'c')  (or,  equivalently,  s=nancumsum(x,2))
//returns in  the rows(x) sized vector s  the cumulative sum
//(ignoring     NANs)    of     the     columns    of     x:
//s(i,:)=nancumsum(x(i,:))
//
//For the  last two cases,  if a row  or column is  in whole
//composed of NAN, the corresponding place of s will contain
//a NAN.
//
//author: carlos klimann
//
//date: 2003-09-03
//
  [lhs,rhs]=argn(0)
  if rhs<1|rhs>2 then error('nancumsum requires one or two inputs.'), end
  if x==[] then s=[], return(s),end
  if rhs==1 then 
    orient='*'
  end
  if orient=='*' then
    if and(isnan(x)) then s=%nan, return, end
    x(isnan(x))=0
    sizx=size(x,'*')
    s=tril(ones(sizx,sizx))*matrix(x,sizx,1)
    return
  end
  six=size(x)
  if orient=='r'|orient==1 then
    i=and(isnan(x),'r')
    x(isnan(x))=0
    s=tril(ones(six(1),six(1)))*x
    s(i)=%nan
    return
  end
  if orient=='c'|orient==2 then
    i=and(isnan(x),'c')
    x(isnan(x))=0
    s=x*triu(ones(six(2),six(2)))
    s(i)=%nan
    return
  end
  error('Second parameter for nancumsum must be *, r, c, 1 or 2')
endfunction

