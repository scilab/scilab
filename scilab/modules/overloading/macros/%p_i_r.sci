// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s1=%p_i_r(varargin)
//Author Serge Steer, INRIA
//!
  s2=varargin($)
  s1=varargin($-1)
  //s2(i,...)=s1, s1 matrix of polynomials

  num=s2.num;
  sz2=size(num)
  ind=list(); for i=sz2,ind($+1)=1:i,end
  num(varargin(1:$-2))=s1
  den=ones(num);
  den(ind(:))=s2.den;
  den(varargin(1:$-2))=ones(s1);

  s1=rlist(num,den,s2.dt)
endfunction
