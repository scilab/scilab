// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2015 - 2016 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function R = %st_c_st(M1,M2)

    d1 = size(M1);
    d2 = size(M2);
    if size(d1,"*")<>size(d2,"*") then
        msg = _("%s: Concatenated arrays must have the same number of dimensions.\n")
        error(msprintf(msg, "%st_c_st"));
    end

    F1 = getfield(1,M1)
    F2 = getfield(1,M2)
    if or(gsort(F1,"g","i")<>gsort(F2,"g","i")) then
        error(msprintf(_("%s: Field names mismatch.\n"),"%st_c_st"));
    end

    kd = find(d1<>d2)
    kd(find(kd==2)) = []
    if kd<>[] then
        error(msprintf(_("%s: Inconsistent dimensions.\n"),"%st_c_st"));
    end

    newdim = d1;
    newdim(2) = d1(2)+d2(2);

    R = struct();
    R(1:size(M1, "*")) = M1;
    s = size(R, "*");
    R((s+1):s+size(M2, "*")) = M2;
    R = matrix(R, newdim);
endfunction
