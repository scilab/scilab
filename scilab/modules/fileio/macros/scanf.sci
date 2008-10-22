// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,..
v17,v18,v19,v20,v21,v22,v23,v24,v25,v26,v27,v28,v29,v30]= scanf(frmt)
// scanf - Emulator of C language scanf

//!
[lhs,rhs]=argn(0)
v='v'
args=strcat(v(ones(lhs,1))+string(1:lhs)',',')
buf=read(%io(1),1,1,'(a)')
execstr('['+args+']=sscanf(buf,frmt)')
endfunction
