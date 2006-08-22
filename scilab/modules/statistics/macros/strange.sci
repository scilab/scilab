function [r]=strange(x,orien)
//
//The range  is  the distance between   the largest  and smaller  value,
//[r]=range(x) computes the range of vector or matrix x.
//
//[r]=range(x,'r')  (or equivalently  [r]=range(x,1)) give a  row vector
//with the range of each column.
//
//[r]=range(x,'c') (or equivalently [r]=range(x,2)) give a column vector
//with the range of each row.
//
//author: carlos klimann
//
//date: 1999-05-11
//
  [lhs,rhs]=argn(0)
  if rhs==0 then error('strange requires at least one input.'), end
  if rhs==1 then
    r=max(x)-min(x)
  elseif rhs==2 then
    if orien=='r'|orien==1 then 
      r=max(x,'r')-min(x,'r')
    elseif orien=='c'|orien==2 then
      r=max(x,'c')-min(x,'c')
    else 
      error('second parameter must be ''r'', ''c'',1 or 2'), 
    end
  end
endfunction
