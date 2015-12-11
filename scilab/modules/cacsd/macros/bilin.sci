// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
