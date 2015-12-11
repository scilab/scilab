// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
