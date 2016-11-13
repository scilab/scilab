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


function Sl2=bilin(Sl1,v)

    [A,B,C,D]=abcd(Sl1);
    dom=Sl1("dt");
    [ra,ca] = size(A);
    a=v(1);d=v(2);c=v(3);b=v(4);
    i=inv(a*eye(ra,ra)-c*A);
    AB=(b*A-d*eye(ra,ra))*i;
    BB=(a*b-c*d)*i*B;
    CB=C*i;
    DB=D+c*C*i*B;
    if dom=="c" then Sl2=syslin("d",AB,BB,CB,DB);
    else Sl2=syslin("c",AB,BB,CB,DB);end


endfunction
