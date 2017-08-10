// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [s]=%lss_v_lss(s1,s2)
    //s=%lss_v_lss(s1,s2)  <=> s = s1 "feedback" with s2
    //             <=> s=s1/.s2    (in state-space)
    //!
    [s1,s2]=sysconv(s1,s2)
    [a1,b1,c1,d1,x1,dom1]=s1(2:7)
    [a2,b2,c2,d2,x2]=s2(2:6);
    e12=1/(eye()+d2*d1)
    e21=eye()-d1*e12*d2;b21=b2*e21;e12=b1*e12
    a=[a1-e12*d2*c1 , -e12*c2 ; b21*c1 , a2-b21*d1*c2]
    s=tlist(["lss","A","B","C","D","X0","dt"],a,[e12;b21*d1],e21*[c1 -d1*c2],...
    e21*d1,[x1;x2],dom1)
endfunction
