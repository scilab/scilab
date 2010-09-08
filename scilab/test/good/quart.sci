// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function q=quart(x,orien)
//
//This function computes the quartiles of a vector or a matrix x.
//
//For a vector  or a matrix x, q=quart(x)  returns  in the vector q  the
//three quartiles entries of x.
//
//q=quart(x,'r')  (or,   equivalently,  q=quart(x,1))   is  the  rowwise
//quartiles range.  It returns in each  column of the 3xlength(x)-matrix
//q, the quartiles of the corresponding column of x.
//
//q=quart(x,'c')  (or, equivalently,   q=quart(x,2))  is the  columnwise
//interquartile range. It returns  in  each row of  length(x)x3-matrix q
//the quartiles of each row of x.
//
//
  function y = vperctl(x,p)
  //inline function wich computes percentiles of a vector
    k=isnan(x)
    x(k)=[];
    n=size(x,'*')
    x=gsort(x,'g','i')
    data=[0     100*(0.5:(n-0.5))./n  100;
	  x(1), matrix(x,1,-1),       x(n)]
    y=interpln(data,p);
  endfunction

  if x==[] then q=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0 then error(msprintf(gettext("%s: Wrong number of input argument: At least %d expected.\n"),"quart",1)), end
  if rhs==1 then
    q=vperctl(x(:),[25 50 75])
    q=q(:)
  else
    q=[]
    if orien=='r'|orien==1 then
      for i=x
	p=vperctl(i,[25 50 75])
	q=[q p(:)]
      end
    else
      for i=x'
	p=vperctl(i,[25 50 75])
	q=[q;p(:)']
      end
    end
  end
endfunction
