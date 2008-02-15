// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%r_c_p(f,p)
// f=%r_c_p(f,p) <=> f=[f,p]  [rational, polynomial]

f=rlist([f('num'),p],[f('den'),ones(p)],f('dt'))
endfunction
