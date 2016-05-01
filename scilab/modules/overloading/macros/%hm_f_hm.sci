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

function M1=%hm_f_hm(M1,M2)

    d1=size(M1);
    d2=size(M2);
    if size(d1,"*")<>size(d2,"*") then
        error(msprintf(_("%s: In concatenation the number of dimensions for each component must match.\n"),"%hm_f_hm"));
    end

    kd=find(d1(2:$)<>d2(2:$))
    if kd<>[] then
        error(msprintf(_("%s: Inconsistent dimensions.\n"),"%hm_f_hm"));
    end

    d3 = d1;
    d3(1) = d1(1) + d2(1);
    M1=matrix([matrix(M1,double(d1(1)),-1) ; matrix(M2,double(d2(1)),-1)], d3)
endfunction
