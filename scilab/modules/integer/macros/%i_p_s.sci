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


function r = %i_p_s(a,n)
    if or(int(n)<>n)|or(n<0) then
        r=double(a)^n
    else
        if size(a,"*")==1 then
            r=iconvert(double(a)^n,inttype(a))
        else
            if size(n,"*")<>1 then
                msg = _("%s: Non implemented feature.\n")
                error(msprintf(msg, "%i_p_s"))
            else
                r=a
                if size(a,1)==size(a,2) then
                    for i=2:n,r=r*a,end
                else
                    msg = _("%s: ""%s"" expected instead of ""%s"".\n")
                    error(msprintf(msg, "%i_p_s", ".^", "^"));
                end
            end
        end
    end
endfunction
