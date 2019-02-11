// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [flag,errmsg] = assert_checktrue ( condition )
    //   Check that condition is true. All its components must be true.

    [lhs,rhs]=argn()
    if ( rhs <> 1 ) then
        errmsg = gettext("%s: Wrong number of input arguments: %d expected.\n")
        errmsg = msprintf(errmsg, "assert_checktrue", 1)
        error(errmsg)
    end
    //
    // Check types of variables
    if and(typeof(condition) <> ["boolean" "boolean sparse"]) then
        errmsg = gettext("%s: Wrong type for input argument #%d: Boolean matrix expected.\n")
        errmsg = msprintf (errmsg, "assert_checktrue", 1)
        error(errmsg)
    end
    //
    if ( and(condition) ) then
        flag = %t
        errmsg = ""
    else
        flag = %f
        if ( size(condition,"*") == 1 ) then
            k = 1
        else
            if issparse(condition)
                // Retrieving the index of the first %F component
                // (without densification)
                s = size(condition)
                v = gsort(sub2ind(s, spget(condition)),"g","i")
                if isempty(v) | v(1)>1
                    k = 1
                elseif length(v)==1
                    k = 2
                else
                    k = find(v(2:$)>(v(1:$-1)+1), 1)
                    if k==[]
                        k = v($)
                    end
                    k = k+1
                end
            else
                k = find(~condition, 1)
            end
        end
        errmsg = gettext("%s: Assertion failed: Entry %%F found in condition(%d).\n")
        errmsg = msprintf(errmsg, "assert_checktrue", k)
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            assert_generror ( errmsg )
        end
    end
endfunction
