// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %dir_p(L)
    ll = lines();
    ll = max(ll(1), 20);
    name = L.name;
    if name == [] then
        mprintf("%s \n", "[]");
        return
    end

    // If it is a directory, add a trailing "/"
    tmp = find(L.isdir);
    if tmp~=[] then
        name(tmp) = name(tmp) + filesep();
    end

    nc = 6;
    l1 = length(name);
    while %t
        l2 = l1;
        n = ceil(size(l1, "*") / nc);
        if n*nc > size(name, "*") then
            l2(n * nc) = 0;
            l1(n * nc) = 0;
        end
        l = max(matrix(l2, -1, nc), "r");
        if (sum(l) < ll)| (nc == 1) then
            break
        end
        nc = nc - 1;
    end
    if n * nc > size(name, "*") then
        name(n*nc) = "";
    end
    name = matrix(name, -1, nc);
    txt = emptystr(n, 1);
    for k = 1:nc
        txt = txt + part(name(:, k), 1 : l(k) + 1);
    end
    txt = stripblanks(txt);
    txt(txt == "") = [];
    mprintf("%s \n",txt);
endfunction
