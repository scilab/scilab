// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// =============================================================================
function varargout = loadwave(filename)

    lhs = argn(1);
    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"loadwave",1));
        return
    end

    if lhs > 2 then
        error(msprintf(gettext("%s: Wrong number of output arguments.\n"),"loadwave"));
        return
    end

    if ~(type(filename) == 10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n" ),"loadwave",1));
        return
    end

    if ~isfile(filename) then
        error(msprintf(gettext("%s: Cannot open file %s.\n"),"loadwave",filename));
    end

    y = wavread(filename);

    if (lhs == 1) then
        varargout = list(y);
    elseif (lhs == 2) then
        info_wav = wavread(filename,"info");
        varargout = list(y, info_wav);
    else
        error(msprintf(gettext("%s: Wrong number of output argument(s).\n" ),"loadwave"));
        return
    end

endfunction
// =============================================================================
