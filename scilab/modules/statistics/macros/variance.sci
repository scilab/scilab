
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function [s]=variance(x,orien)
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
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0 then error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"variance",1,2)), end
  [m n]=size(x);
  if rhs==1
    s=(sum((x-mean(x)).^2))/(m*n-1)
  elseif orien=='c'|orien==2 then
    s=(sum((x-mean(x,'c')*ones(x(1,:))).^2,'c')/(n-1));
  elseif orien=='r'|orien==1 then
    s=(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/(m-1));
  else
    error(msprintf(gettext("%s: Wrong value for input argument: ''%s'', ''%s'', %d or %d expected.\n"),"variance","c","r",1,2));
  end
endfunction
