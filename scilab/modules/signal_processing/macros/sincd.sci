// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [s]=sincd(n,flag)
    //<s>=sincd(n,flag)
    //macro which calculates the function Sin(N*x)/Sin(x)
    //  n    : value of N in the preceding function
    //  flag : flag=1 : the function is centered around the origin
    //       : flag=2 : the function is delayed by %pi/(2*n)
    //  s    : vector of values of the function on a dense
    //       : grid of frequencies
    //!

    npt=4*n;
    pas=%pi/npt;
    om=0:pas:%pi;
    eps=(-1)**(n-1);
    select flag
    case 1,
        s1=sin(n*om);s2=sin(om);
        s1(1)=n;s2(1)=1;s1(npt+1)=n*eps;s2(npt+1)=1;
        s=s1./s2;
        s=[s(npt+1:-1:2) s];
        s=s/n;
    case 2,
        om=om-ones(om)*%pi/2/n;
        s1=sin(n*om);
        s2=sin(om);
        s1(3)=n;s2(3)=1;
        s=s1./s2;
        s=[eps*s s(2:npt+1)];
        s=s/n;
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"sincd",2,"1,2"));
    end;
endfunction
