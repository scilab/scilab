// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,..
    v17,v18,v19,v20,v21,v22,v23,v24,v25,v26,v27,v28,v29,v30]= scanf(frmt)
    // scanf - Emulator of C language scanf

    //!
    [lhs,rhs]=argn(0)
    v="v"
    args=strcat(v(ones(lhs,1))+string(1:lhs)',",")
    buf=read(%io(1),1,1,"(a)")
    execstr("["+args+"]=msscanf(buf,frmt)")
endfunction
