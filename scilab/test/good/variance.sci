
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function [s]=variance(x,orien,w)
//
//This function computes  the  variance  of  the values of  a  vector or
//matrix x.
//
//For a vector or   a matrix x,  s=variance(x)  returns in  the scalar s  the
//variance of all the entries of x.
//
//s=variance(x,'r')    (or,  equivalently,    s=variance(x,1)) is     the  rowwise
//variance. It returns in each entry of the row vector s the variance of
//each column of x.
//
//s=variance(x,'c') (or, equivalently, s=variance(x,2)) is the columnwise standard
//deviation.   It  returns in  each  entry  of the   column vector y the
//variance of each row of x.
//
//
  if x==[] then 
    s=%nan;
    return;
  end
  [lhs,rhs]=argn(0)
  if rhs==0 then 
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"variance",1,2))
  end
  [m n]=size(x);
  if rhs<=2 then 
    w=0;
  end
  if rhs==1
    if w==0 then
      s=(sum((x-mean(x)).^2))/(m*n-1);
    elseif w==1 then
      s=(sum((x-mean(x)).^2))/(m*n);
    else
      error(msprintf(gettext("%s: Wrong value of w : %d; 0 or 1 expected.\n"),"variance",w))
    end
  elseif orien=='c'|orien==2 then
    y(1:m,1:n) = x(1:m,1:n) -  mean(x,2) * ones(1,n);
    s = ones(m,1);
    for i=1:m
      s(i) = real(y(i,1:n) * y(i,1:n)');
      if w==0 then
        s(i) = s(i) / (n - 1);
      elseif w==1 then
        s(i) = s(i) / n;
      else
        error(msprintf(gettext("%s: Wrong value of w : %d; 0 or 1 expected.\n"),"variance",w))
      end
    end
  elseif orien=='r'|orien==1 then
    y(1:m,1:n) = x(1:m,1:n) - ones(m,1) * mean(x,1);
    s = ones(1,n);
    for i=1:n
      s(i) = real(y(1:m,i)' * y(1:m,i));
      if w==0 then
        s(i) = s(i) / (m - 1);
      elseif w==1 then
        s(i) = s(i) / m;
      else
        error(msprintf(gettext("%s: Wrong value of w : %d; 0 or 1 expected.\n"),"variance",w))
      end
    end
  else
    error(msprintf(gettext("%s: Wrong value for input argument: ''%s'', ''%s'', %d or %d expected.\n"),"variance","c","r",1,2));
  end
endfunction

