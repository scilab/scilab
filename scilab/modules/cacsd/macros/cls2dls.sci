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


function [s1]=cls2dls(s,t,fp)
    //Syntax : s1=cls2dls(sl,t [,fp])
    //
    // Given sl=[a,b,c,d] (syslin list) continuous time system, cls2dls
    // returns the sampled system obatined by the bilinear transform
    // s=(2/t)*(z-1)/(z+1).
    //
    // t sampling period
    // fp prevarping frequency in hertz
    //!

    [lhs,rhs]=argn(0)
    if typeof(s)<>"state-space"
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"cls2dls",1))
    end
    if s.dt==[] then
        warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"cls2dls",1));
        s.dt="c";
    end
    if s.dt<>"c" then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"cls2dls",1))
    end
    fs=1/t
    if rhs==3 then fp=2*%pi*fp;fs=fp/tan(fp/fs/2)/2,end //prewarping

    a=2*fs*eye()-s(2)
    ad=a\(2*fs*eye()+s(2))
    b=(ad+eye())/a*s(3);
    d=s(5)+s(4)/a*s(3)
    s1=syslin("d",ad,b,s(4),d,s(6))
endfunction
