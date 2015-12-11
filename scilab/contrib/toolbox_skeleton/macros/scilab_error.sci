// ====================================================================
// Template toolbox_skeleton
// This file is released under the 3-clause BSD license. See COPYING-BSD.
// ====================================================================
//
//
function scilab_error(varargin)

    argSize = size(varargin);

    //in toolboxes, use "_d" or "dgettext" to your localized messages
    if argSize <> 1 then
        error(999, msprintf(_d("toolbox_skeleton", "%s: I''m waiting for only one argument.\n"), "scilab_error"));
    end

    if argSize == 1 then
        error(999, msprintf(dgettext("toolbox_skeleton", "%s: Yeah! %d is a good number of arguments but I prefer fail, sorry.\n"), "scilab_error", 1));
    end
endfunction
// ====================================================================
