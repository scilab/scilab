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

function [flag,errmsg] = assert_checktrue ( varargin )
    //   Check that condition is true. All its components must be true.

    [lhs,rhs]=argn()
    if ( and(rhs <> [1 2]) ) then
        errmsg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        errmsg = msprintf(errmsg, "assert_checktrue", 1, 2)
        error(errmsg)
    end
    //
    // Get arguments
    condition = varargin(1)
    if rhs < 2 then
        msg = ""
    else
        msg = varargin(2)
    end
    //
    // Check types of variables
    if and(typeof(condition) <> ["boolean" "boolean sparse"]) then
        errmsg = gettext("%s: Wrong type for input argument #%d: Boolean matrix expected.\n")
        errmsg = msprintf (errmsg, "assert_checktrue", 1)
        error(errmsg)
    end
    //
    // Validate message parameter if provided
    if rhs >= 2 then
        if typeof(msg) <> "string" then
            flag = %f
            errmsg = gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n")
            errmsg = msprintf(errmsg, "assert_checktrue", 2)
            if lhs == 1 then
                error(errmsg)
            end
            return
        end
        if size(msg, "*") <> 1 then
            flag = %f
            errmsg = gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n")
            errmsg = msprintf(errmsg, "assert_checktrue", 2, 1, 1)
            if lhs == 1 then
                error(errmsg)
            end
            return
        end
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
        // Prepend custom message if provided
        if msg <> "" then
            errmsg = msg + ": " + errmsg
        end
        if ( lhs < 2 ) then
            // If no output variable is given, generate an error
            assert_generror(errmsg)
        end
    end
endfunction
