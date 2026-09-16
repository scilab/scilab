// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [flag,errmsg] = assert_checkfalse ( varargin )
    //   Check that condition is false. All its components must be False.

    [lhs,rhs]=argn()
    if ( and(rhs <> [1 2]) ) then
        errmsg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        errmsg = msprintf(errmsg, "assert_checkfalse", 1, 2)
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
        errmsg = sprintf(errmsg, "assert_checkfalse", 1)
        error(errmsg)
    end
    //
    // Validate message parameter if provided
    if rhs >= 2 then
        if typeof(msg) <> "string" then
            flag = %f
            errmsg = gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n")
            errmsg = sprintf(errmsg, "assert_checkfalse", 2)
            if lhs == 1 then
                error(errmsg)
            end
            return
        end
        if size(msg, "*") <> 1 then
            flag = %f
            errmsg = gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n")
            errmsg = sprintf(errmsg, "assert_checkfalse", 2, 1, 1)
            if lhs == 1 then
                error(errmsg)
            end
            return
        end
    end
    //
    if ( and(~condition) ) then
        flag = %t
        errmsg = ""
    else
        flag = %f
        k = find(condition, 1);
        errmsg = gettext("%s: Assertion failed: Entry %%T found in condition(%d).\n")
        errmsg = msprintf(errmsg, "assert_checkfalse", k)
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

