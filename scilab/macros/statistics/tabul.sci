function [m]=tabul(x)
//
//This function computes the frequency of values of  the components of a
//numerical o chain of characters vector or matrix x.
//
//If x  is a numerical  vector or matrix, m  is a  two column matrix who
//contains in the first column the values  of x and  in the other column
//there is the number of occurrences of each value in x.
//
//If x is composed  of chains  of characters,  m is  a list whose  first
//member is a string vector composed with the values of x and the second
//member is a vector  whose components are  the number of occurrences of
//each value in x.
//
//author: carlos klimann
//
//date: 1999-04-09
//
  if x==[] then m=%nan, return, end
  lxnozero=prod(size(x));
  xx=sort(matrix(x,lxnozero,1));
  m=[]
  m2=[]
  while lxnozero>0 do
    xnozero=find(xx<>xx(1))',
    l=lxnozero,
    lxnozero=prod(size(xnozero)),
    m=[m ; xx(1)],
    m2=[m2 ; l-lxnozero],
    xx=xx(xnozero),
  end
  if type(m)==10 then
    m=list(m,m2);
  else 
    m=[m m2]
  end
endfunction
