function [hm]=harmean(x,orien)
//This function computes the harmonic mean of a vector or matrix x.
//
//For a vector  or  matrix  x,  hm=harmean(x) returns in  scalar  hm the
//harmonic mean of all the entries of x.
//
//hm=harmean(x,'r')(or, equivalently,  hm=harmean(x,1)) returns  in each
//entry of the row vector hm the harmonic mean of each column of x.
//
//hm=harmean(x,'c')(or, equivalently,  hm=harmean(x,2)) returns  in each
//entry of the column vector hm the harmonic mean of each row of x.
//
//Nota: In the computation of the harmonic mean we consider only the non
//zero values of x.
//
//References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
//Statistics, J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 1999-06-11
//
  if x==[] then hm=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0 then error('harmean requires at least one input.'), end
  if rhs==1 then
    hm=1/(sum(1 ./x(x<>0))/sum(x<>0))
    return
  elseif rhs==2 then
    if orien=='r'|orien==1 then
      le=ones(1,size(x,1))*bool2s(x<>0)
    elseif orien=='c'|orien==2 then
      le=bool2s(x<>0)*ones(size(x,2),1)
    else error('Second parameter must be ''r'', ''c'', 1 or 2'), end
      x(x==0)=%inf
      hm=sum(1 ./x,orien) ./le
  else 
    error('The number of input parameters must be one or two.'),
  end
endfunction
