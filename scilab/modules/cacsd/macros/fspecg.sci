
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

function [gm]=fspecg(g)
    // gm=fspecg(g) : stable factorization.
    // g and gm are continuous-time linear systems in state-space form.
    //       gtild(g)*g = gtild(gm)*gm
    //with gm stable.
    // Imaginary-axis poles are forbidden.
    //
    //!
    if typeof(g)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear system expected.\n"),"fspecg",1))
    end
    if g.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"fspecg",1))
    end

    [a,b,c,d]=abcd(g),
    g=[];
    a=-a;
    b=-b;
    h=[-a',c'*c;
    0*eye(a),a];
    x=ric_desc(h);h=[]
    gm=syslin("c",-a'+c'*c*x,-c',b'-d'*c*x,d')';
endfunction
