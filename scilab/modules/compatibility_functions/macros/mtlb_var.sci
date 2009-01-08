
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function [s] = mtlb_var (x,w,dim)
//
// mtlb_var --
//   This function computes  the  variance  of  the values of  a  vector or
//   matrix x.
//   It provides the same service as Octave and Matlab.
// Arguments
//   x may be a real or a complex vector or matrix.
//   s is a real scalar or real vector.
//     If x is a vector, s = mtlb_var(x) is the variance of x. 
//     If x is a matrix, s = mtlb_var(x) is a row vector containing the variance of each column of x.
//   w : type of normalization to use. Valid values are, depending on the number of columns m of x :
//     w = 0 : normalizes with m-1, provides the best unbiased estimator of the variance [default].
//     w = 1: normalizes with m, this provides the second moment around the mean. 
//   dim : the dimension along which the variance is computed (default is 1, i.e. column by column)
//     If dim is 2, the variance is computed row by row.
//
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0 then
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"variance2",1,2))
  end
  [m n]=size(x);
  if rhs==1 then 
    w=0;
  end
  if rhs<=2 then 
    dim=1;
  end
  if dim==1 then
    y(1:m,1:n) = x(1:m,1:n) - ones(m,1) * mean(x,1);
  elseif dim==2 then
    y(1:m,1:n) = x(1:m,1:n) -  mean(x,2) * ones(1,n);
  else
    error(msprintf(gettext("%s: Wrong value of dim: %d , %d or %d expected.\n"),"variance2",dim,1,2))
  end
  if dim==1 then
    if w==0 then
      if m==1 then
        error(msprintf(gettext("%s: The normalization m - 1 factor is zero.\n"),"variance2"))
      end
    end
  end
  if dim==2 then
    if w==1 then
      if n==1 then
        error(msprintf(gettext("%s: The normalization n - 1 factor is zero.\n"),"variance2"))
      end
    end
  end
  if dim==1 then
    s = ones(1,n);
    for i=1:n
      s(i) = real(y(1:m,i)' * y(1:m,i));
      if w==0 then
        s(i) = s(i) / (m - 1);
      elseif w==1 then
        s(i) = s(i) / m;
      else
        error(msprintf(gettext("%s: Wrong value of w : %d; 0 or 1 expected.\n"),"variance2",w))
      end
    end
  elseif dim==2 then
    s = ones(m,1);
    for i=1:m
      s(i) = real(y(i,1:n) * y(i,1:n)');
      if w==0 then
        s(i) = s(i) / (n - 1);
      elseif w==1 then
        s(i) = s(i) / n;
      else
        error(msprintf(gettext("%s: Wrong value of w : %d; 0 or 1 expected.\n"),"variance2",w))
      end
    end
  end
endfunction

