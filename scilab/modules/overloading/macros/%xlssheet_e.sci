// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function R=%xlssheet_e(varargin)
  //extract a subsheet out of an xls sheet
  s=varargin($)
  R=s.value;T=s.text
  R=R(varargin(1:$-1))
  T=T(varargin(1:$-1))
  if and(isnan(R)) then
    R=T
  elseif or(isnan(R)) then
    R=mlist(['xlssheet','name','text','value'],'',T,R)
  end
endfunction
