// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function   bool=iscellstr(c)

    // This function returns 1 if input argument is a cell of strings, else it returns 0.
    // F.B

    if argn(2)<>1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"iscellstr",1));
    else
        if isempty(c) then
            bool=%F;
        else
            if typeof(c)=="ce"
                bool=%T;
                for i=1:size(c,"*")
                    if typeof(c{i})<>"string" then
                        bool=%F;
                        break
                    end
                end
            else
                bool=%F;
            end
        end
    end

endfunction
