// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//@warnobsolete

function [sp]=%sp_eye(m,n)
warnobsolete("speye", "5.1")
[lhs,rhs]=argn(0)
if rhs==1 then [m,n]=size(m),end
sp=speye(m,n)
endfunction
