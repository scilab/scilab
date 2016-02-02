// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//=============================================================================
function r = dlwForceRebuild(varargin)

    [lhs, rhs] = argn(0);
    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "dlwForceRebuild"));
    end

    if rhs == 1 then
        newValue = varargin(1);
        if type(newValue) <> 4 then
            error(999,msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"), "dlwForceRebuild", 1));
        end

        if size(newValue,"*") <> 1 then
            error(999,msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "dlwForceRebuild", 1));
        end

        if newValue == %t then
            setenv("FORCE_REBUILD_DYNAMIC_LINK","YES")
        else
            setenv("FORCE_REBUILD_DYNAMIC_LINK","NO")
        end
    end

    if getenv("FORCE_REBUILD_DYNAMIC_LINK","NO") == "YES" then
        r = %t;
    else
        r = %f;
    end

endfunction
