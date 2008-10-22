// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%p_c_r(m,f)
// f= %p_c_r(m,f) <=> r= [m,f]  m:polynomial mat. r: rational mat.
//!
// Copyright INRIA
f=rlist([m,f('num')],[ones(m),f('den')],f('dt'))
endfunction
