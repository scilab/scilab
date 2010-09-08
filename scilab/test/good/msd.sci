// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 


function m=msd(x,orien)
//
//This function computes  the mean squared  deviation of the values of a
//vector or matrix x.
//
//For a vector or a matrix x, m=msd(x) returns in  the scalar y the mean
//squared deviation of all the entries of x.
//
//m=msd(x,'r') (or,   equivalently,   m=msd(x,1)) is the   rowwise  mean
//squared  deviation. It returns  in each entry of the  row vector y the
//mean squared deviation of each column of x.
//
//m=msd(x,'c') (or,   equivalently, m=msd(x,2)) is  the  columnwise mean
//squared deviation. It returns in each entry of the column vector y the
//mean squared deviation of each row of x.
//
//Reference: Wonacott  T.H.& Wonacott  R.J. .-  Introductory Statistics,
//5th edition, John Wiley, 1990.
//
//
  rhs=argn(2)
  if rhs==0 then error(msprintf(gettext("%s: Wrong number of input argument: At least %d expected.\n"),"msd",1)), end
  if x==[] then s=%nan, return, end
  if rhs==1 then orien='*',end
  if orien=='*' then
    m=sqrt(sum((x-mean(x)).^2)/size(x,orien));
  elseif orien=='c'|orien==2 then
    m=sqrt(sum((x-mean(x,orien)*ones(x(1,:))).^2,orien)/size(x,orien));
  elseif orien=='r'|orien==1 then
    m=sqrt(sum((x-ones(x(:,1))*mean(x,orien)).^2,orien)/size(x,orien));
  else
	error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', %d or %d expected.\n"),"stdevf",2,"r","c",1,2)),
  end
endfunction
