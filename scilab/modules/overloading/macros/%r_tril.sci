// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=%r_tril(a,k)

[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
d=syslin(a('dt'),tril(a('num'),k),tril(a('den'),k)+triu(ones(a('den')),k+1))
endfunction
