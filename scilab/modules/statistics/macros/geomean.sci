function gm=geomean(x,orien)
//
//This function computes the geometric mean of a vector or matrix x.
//
//For a vector  or  matrix  x,  gm=geomean(x) returns in  scalar  gm the
//geometric mean of all the entries of x.
//
//gm=geomean(x,'r')(or,  equivalently,   gm=gmean(x,1)) returns  in each
//entry of the row vector gm the geometric mean of each column of x.
//
//gm=geomean(x,'c')(or,   equivalently,  gm=gmean(x,2)) returns in  each
//entry of the column vector gm the geometric mean of each row of x.
//
//References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
//Statistics, J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 1999-06-10
//
  if x==[] then gm=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0 then error('geomean requires at least one input.'), end
  if rhs==1 then
    gm=prod(x)^(1/length(x))
  elseif rhs==2
    gm=prod(x,orien).^(1/size(x,orien))
  else 
    error('The number of input parameters must be 1 or 2')
  end
endfunction
