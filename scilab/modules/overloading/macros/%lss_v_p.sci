// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [sr]=%lss_v_p(s,p)
// feedback sr=(eye+s*p)\s
//s=%lss_v_p(s,p) <=> sr=s/.p
// p : polynomial matrix
// s : state-space syslin list
//!
sr=s/.tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])
endfunction
