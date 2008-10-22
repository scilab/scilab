// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%p_q_r(p,r)
// r= p.\ r   polynomial./rational
//!

r=rlist(ones(p).*r('num'),p.*r('den'),r('dt'))
endfunction
