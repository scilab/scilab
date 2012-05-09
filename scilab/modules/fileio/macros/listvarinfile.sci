// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function varargout = listvarinfile(filename)

    [lhs, rhs] = argn();
    if rhs <> 1 then
        error(999, msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "listvarinfile", 1));
    end

    if typeof(filename) <> "string" | size(filename, "*") <> 1 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "listvarinfile", 1));
    end

    if isfile(filename) & is_hdf5_file(filename) then
        listvarFunction = listvar_in_hdf5;
    else
        listvarFunction = %_listvarinfile;
    end

    //check output argument count
    if lhs == 1 then
        a = listvarFunction(filename);
        varargout = list(a);
    elseif lhs == 2 then
        [a,b] = listvarFunction(filename);
        varargout = list(a,b);
    elseif lhs == 3 then
        [a,b,c] = listvarFunction(filename);
        varargout = list(a,b,c);
    elseif lhs == 4 then
        [a,b,c,d] = listvarFunction(filename);
        varargout = list(a,b,c,d);
    end
endfunction
