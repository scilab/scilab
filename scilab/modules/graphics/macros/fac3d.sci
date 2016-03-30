// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function fac3d(x,y,z,T,A,leg,flags,ebox)

    // Copyright INRIA

    [lhs,rhs]=argn(0)
    if rhs <= 0 then   // demo
        deff("[x,y,z]=sph(alp,tet)",["x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)"; ..
        "y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)"; ..
        "z=r*sin(alp)+orig(3)*ones(tet)"]);
        r=1;
        orig=[0 0 0];
        [x1,y1,z1]=eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20));
        [n1,m1]=size(x1);
        r=1/2;orig=[-1 0 0];
        [x2,y2,z2]=eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20));
        [n2,m2]=size(x2);
        x=[x1 x2];
        y=[y1 y2];
        z=[z1 z2];
        plot3d(x,y,z);
        return
    end

    write(%io(2),"WARNING : fac3d1 is obsolete see plot3d plot3d1 instead");

    select rhs
    case 3 then plot3d(x,y,z)
    case 4 then plot3d(x,y,z,T)
    case 5 then plot3d(x,y,z,T,A)
    case 6 then plot3d(x,y,z,T,A,leg)
    case 7 then plot3d(x,y,z,T,A,leg,flags)
    case 8 then plot3d(x,y,z,T,A,leg,flags,ebox)
    else error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "fac3d", 3, 8));
    end
endfunction
