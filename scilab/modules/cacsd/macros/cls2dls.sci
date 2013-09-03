// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
