function [nonan,numb]=thrownan(x)
//
//
//This  function  returns  in  vector  nonan  the  values
//(ignoring the NANs) of a  vector or matrix x and in the
//corresponding places of vector  numb the indexes of the
//value.
//
//For  a  vector  or matrix  x,  [nonan,numb]=thrownan(x)
//considers x, whatever his dimensions are, like a linear
//vector (columns  first).
//
//author: carlos klimann
//
//date: 2002-11-28
//
  [lhs,rhs]=argn(0)
  if rhs<>1 then error('thrownan has one parameter, exactly.'), end
  if x==[] then s=[], return,end
  numb=find(bool2s(~isnan(x)))
  nonan=x(~isnan(x))

endfunction
