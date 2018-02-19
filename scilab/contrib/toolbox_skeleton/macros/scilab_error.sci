// ====================================================================
// Template toolbox_skeleton
// This file is released under the 3-clause BSD license. See COPYING-BSD.
// ====================================================================
//
//
function scilab_error(varargin)

    argSize = size(varargin);

    if argSize <> 1 then
        error(msprintf(_("toolbox_skeleton", "%s: I''m waiting for only one argument.\n"), "scilab_error"));
    end

    if argSize == 1 then
        error(msprintf(gettext("toolbox_skeleton", "%s: Yeah! %d is a good number of arguments but I prefer fail, sorry.\n"), "scilab_error", 1));
    end
endfunction
// ====================================================================
