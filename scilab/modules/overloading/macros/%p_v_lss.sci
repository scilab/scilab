// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [sr]=%p_v_lss(p,s)
//  feedback sr=(eye+p*s)\p
//s=%p_v_lss(p,s) <=> sr=p/.s
// p : polynomial matrix
// s : state-space syslin list
//!

sr=tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])/.s
endfunction
