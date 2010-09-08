// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f2=%r_i_s(varargin)
//author Serge Steer INRIA
  f2=varargin($-1)
  n= varargin($)
// %r_i_s(i,j,f2,n) 

  if size(varargin)==3 then
    i= varargin(1)
    if (type(i)==10|type(i)==15) then //cell or struct case
      f2=createstruct(i,f2)
      if type(i(1))<>10 & i(2)=="entries" then
	// change struct to cell
	f=getfield(1,f2);f(1)="ce"
	setfield(1,f,f2)
      end
      return
    end
    d=ones(n);
    n(i)=f2.num,d(i)=f2.den
  else
    d=ones(n);
    n(varargin(1:$-2))=f2.num;
    d=ones(n);
    d(varargin(1:$-2))=f2.den;
  end
  f2=rlist(n,d,f2.dt)
endfunction
