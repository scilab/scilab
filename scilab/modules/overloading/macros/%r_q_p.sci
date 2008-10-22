// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f1=%r_q_p(f1,f2)
//   r.\p
//!

f1=rlist(f1('den').*f2,f1('num').*ones(f2),f1('dt'))
endfunction
