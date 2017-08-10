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

function xh=hilb(n,wtype,par)
    //xh=hilb(n [,wtype [,par]])
    //returns the first n points of the
    //Hilbert IIR filter  centered around the origin.
    //That is, xh=(2/(n*pi))*(sin(n*pi/2))**2.
    //Window type and window parameters are optional.
    // n     :Number of points in filter
    // wtype :window type ('re','tr','hn','hm','kr','ch')
    //       :     default wtype='re'
    // par   :window parameter for wtype='kr' or 'ch'
    //       :     default par=[0 0]
    //       :see the macro window for more help
    // xh    :Hilbert transform
    //
    //!
    // References:
    // http://ieeexplore.ieee.org/iel4/78/7823/00330385.pdf?tp=&arnumber=330385&isnumber=7823
    //  A. Reilly, G. Frazer, and B. Boashash, "Analytic signal generation
    //  Tips and traps,¡ IEEE Trans. Signal Processing, vol. 42,
    //  pp.3241-3245, Nov. 1994.
    [lhs,rhs]=argn(0);
    if rhs==1 then,
        wtype="re";
        par=[0 0];
    elseif rhs==2 then,
        par=[0 0];
    end,

    if type(n)<>1|size(n,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),"hilb",1));
    end
    if (n - floor(n) <> 0) | (floor(n/2) == n/2) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An odd integer expected.\n"),"hilb",1));
    end
    if and(wtype<>["re","tr","hn","hm","kr","ch"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n") ,"hilb",2,"''re'',''tr'',''hn'',''hm'',''kr'',''ch''"));

    end
    no2=int(n/2)
    if no2==n/2 then,
        xh0=[]
    else
        xh0=0
    end,
    th=zeros(1,no2);
    th(1:2:no2)=ones(1:2:no2)./(1:2:no2);
    xh=[-th(no2:-1:1),xh0,th];
    xh=2*xh/%pi;

    [win_l,cwp]=window(wtype,n,par);
    xh=xh.*win_l;
endfunction
