function [s]=nansum(x,orient)
//
//This function  returns in  scalar or vector  s the  sum of the  values
//(ignoring the  NANs) of a vector  or  matrix x.
//
//For a vector or matrix  x, s=nansum(x) returns  in scalar s the sum of
//all the entries (ignoring the NANs) of x.
//
//s=nansum(x,'r')(or, equivalently, s=nansum(x,1)) returns in each entry
//of the row vector s of type 1xsize(x,'c') the  sum of each column of x
//(ignoring the NANs).
//
//s=nansum(x,'c')(or, equivalently, s=nansum(x,2)) returns in each entry
//of the column vector s of type size(x,'c')x1 the  sum of each row of x
//(ignoring the NANs).
//
//author: carlos klimann
//
//date: 2000-01-18
//
  [lhs,rhs]=argn(0)
  if rhs<1|rhs>2 then error('nanmeanf requires one or two inputs.'), end
  if x==[] then s=[], return,end
  x(isnan(x))=0
  if rhs==1 then s=sum(x),
  elseif orient=='r'|orient=='c'|orient==1|orient==2 then
    s=sum(x,orient)
  else ('Third parameter for nansum must be r, c, 1 or 2')
  end
endfunction
