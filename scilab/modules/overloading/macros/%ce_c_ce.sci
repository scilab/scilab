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

function r=%ce_c_ce(a,b)
    da = size(a);
    if size(da,"*") < 2 then
        da(2) = 1;
    end
    db = size(b);
    if size(db,"*") < 2 then
        db(2) = 1;
    end
    if size(da,"*") <> size(db,"*") then
        error(msprintf(_("%s: In concatenation the number of dimensions for each component must match.\n"),"ce_c_ce"));
    end

    kd = find(da<>db);
    kd(find(kd==2)) = [];
    if kd <> [] then
        error(5);
    end
    v = [matrix(1:prod(da),da(1)*da(2),prod(da)/(da(1)*da(2)));
    -matrix(1:prod(db),db(1)*db(2),prod(db)/(db(1)*db(2)))];
    val = list();
    for k = 1:size(v,"*")
        if v(k) > 0 then
            val(k) = a{v(k)};
        else
            val(k) = b{-v(k)};
        end
    end
    da(2) = da(2)+db(2);
    r = makecell(da, val(:));
endfunction



