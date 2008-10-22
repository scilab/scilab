// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [f,g,ind]=NDcost(x,ind,fun,varargin)
//external for optim 
//computes gradient using Code differentiation
  if argn(2)<4 then varargin=list(),end
  f=fun(x,varargin(:))
  g=derivative(list(fun,varargin(:)),x)
endfunction
