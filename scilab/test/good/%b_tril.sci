// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Bruno Pincon
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [d]=%b_tril(a,k)
// Copyright INRIA (modified by bruno, June 10 2004)
   [lhs,rhs] = argn()
   if rhs==1 then k=0,end
   [m,n] = size(a)
   d(m,n) = %f  // create an m x n boolean mat with %f elts
   i = find(tril(ones(a),k))
   d(i) = a(i)
endfunction
