// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===


function [Sm]=systmat(Sl);
    // System matrix of the linear system Sl (syslin list)
    // in state-space form.
    // Sm = [-sE + A   B;
    //      [    C     D]
    // To get the zeros use det or detr (for square systems)
    //!
    ty=Sl(1);
    ty=ty(1);
    if ty=="lss" then
        if Sl(7)=="d" then
            s=poly(0,"z");
        else
            s=poly(0,"s");
        end
        Sm=[-s*eye(Sl(2))+Sl(2),Sl(3);Sl(4),Sl(5)];
        return
    end
    if part(ty,1)=="d" then
        s=poly(0,"s");
        Sm=[-s*Sl(6)+Sl(2),Sl(3);Sl(4),Sl(5)];
        return
    end
endfunction
