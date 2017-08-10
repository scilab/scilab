// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=sysdiag(varargin)
    //Returns the block-diagonal system made with subsystems put in the main
    //diagonal
    // Syntax:
    // r=sysdiag(a1,a2,...,an)
    //
    // ai    : subsystems (i.e. gains, or linear systems in state-space or
    //                     transfer form)
    //Remark:
    //  At most 17 arguments...
    //Example
    // s=poly(0,'s')
    // sysdiag(rand(2,2),1/(s+1),[1/(s-1);1/((s-2)*(s-3))])
    // sysdiag(tf2ss(1/s),1/(s+1),[1/(s-1);1/((s-2)*(s-3))])


    //!
    r=varargin(1);
    [m1,n1]=size(r);
    for k=2:size(varargin)
        ak=varargin(k)
        [mk,nk]=size(ak);
        r=[r,0*ones(m1,nk);0*ones(mk,n1),ak]
        m1=m1+mk
        n1=n1+nk
    end
endfunction
