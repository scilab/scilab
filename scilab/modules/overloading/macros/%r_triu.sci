// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=%r_triu(a,k)
// g_triu - implement triu function for sparse matrix, rationnal matrix ,..

[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
d=syslin(a('dt'),triu(a('num'),k),triu(a('den'),k)+tril(ones(a('den')),k-1))
endfunction
