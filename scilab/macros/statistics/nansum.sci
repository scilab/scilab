function [s]=nansum(x,orient)
//
//This function returns in scalar or vector s the sum of the
//values (ignoring the NANs) of a vector or matrix x.
//
//For a vector or matrix  x, s=nansum(x) returns in scalar s
//the sum of all the entries (ignoring the NANs) of x.
//
//s=nansum(x,'r')(or,  equivalently,  s=nansum(x,1)) returns
//in each  entry of the  row vector s of  type 1xsize(x,'c')
//the sum of each column of x (ignoring the NANs).
//
//s=nansum(x,'c')(or,  equivalently,  s=nansum(x,2)) returns
//in each entry of the column vector s of type size(x,'c')x1
//the sum of each row of x (ignoring the NANs).
//
//For the  last two cases,  if a row  or column is  in whole
//composed of NAN, the corresponding place of s will contain
//a NAN.
//
//author: carlos klimann
//
//date: 2000-01-18
//
//fixed: 2003/09/03
//error texts and all NAN rows or columns
//
  [lhs,rhs]=argn(0)
  if rhs<1|rhs>2 then error('nansum requires one or two inputs.'), end
  if x==[] then s=[], return(s),end
  if rhs==1 then 
    if and(isnan(x)) then s=%nan, return, end
    x(isnan(x))=0
    s=sum(x)
    return
  end
  if orient=='r'|orient==1 then
    i=and(isnan(x),'r')
    x(isnan(x))=0
    s=sum(x,'r')
    s(i)=%nan
    return
  end
  if orient=='c'|orient==2 then
    i=and(isnan(x),'c')
    x(isnan(x))=0
    s=sum(x,'c')
    s(i)=%nan
    return
  end
  error('Second parameter must be *, r or c')
endfunction
