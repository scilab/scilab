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


function r=%i_p_i(a,n)
    if or(n<0) then
        r=double(a)^n
    else
        if size(a,"*")==1 then
            r=iconvert(double(a)^double(n),inttype(a))
        else
            if size(n,"*")<>1 then
                error(43)
            else
                n=double(n)
                r=a
                if size(a,1)==size(a,2) then
                    for i=2:n,r=r*a,end
                else
                    mprintf(gettext("Warning: Syntax ""vector ^ scalar"" is obsolete. It will be removed in Scilab 6.0.\n  Use ""vector .^ scalar"" instead.\n"));
                    for i=2:n,r=r.*a,end
                end
            end
        end
    end
endfunction
