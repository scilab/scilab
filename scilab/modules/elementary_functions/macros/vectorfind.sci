// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function ind=vectorfind(m,v,job)

    rhs = argn(2);

    // Check number of inputs, at least 2 needed
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "vectorfind", 2, 3));
    end

    // Set default value for job if not given
    if rhs == 2 then
        job = "r";
    end

    // Check that m and v have the same type
    if typeof(m) <> typeof(v) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same type expected.\n"), "vectorfind", 1, 2));
    end

    if min(size(v))<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Vector expected.\n"),"vectorfind",2));
    end

    if convstr(part(job,1))=="r" then
        if size(v,"*")<>size(m,2) then
            error(msprintf(gettext("%s: Wrong size for input arguments: Incompatible sizes.\n"),"vectorfind"));
        end
        ind=1:size(m,1)
        for k=1:size(m,2)
            ind=ind(find(m(ind,k)==v(k)));
            if ind==[] then
                break
            end
        end
    elseif convstr(part(job,1))=="c" then
        if size(v,"*")<>size(m,1) then
            error(msprintf(gettext("%s: Wrong size for input arguments: Incompatible sizes.\n"),"vectorfind"));
        end
        ind=1:size(m,2)
        for k=1:size(m,1)
            ind=ind(find(m(k,ind)==v(k)))
            if ind==[] then
                break
            end
        end
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"vectorfind",3,"r[ow]","c[olumn]"));
    end
endfunction

