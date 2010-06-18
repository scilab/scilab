//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// Export a palette instance to an hdf5 file.
//
// @param pal the "palette" type
// @param path Fullpath to the hdf5 file (.h5)
// @return status %T if the export has been successful, %F otherwise
// @return[opt] msg Contains the error message in case of %F status or an empty
//                  string when no error as occurred.
function [status, msg] = xcosPalExport(pal, path)
    status = %F;
    msg = "";
    
    // Checking arguments
    [lhs,rhs] = argn(0)
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "xcosPalExport", 2));
    end
    
    if lhs > 2 then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d to %d expected.\n"), "xcosPalExport", 1, 2));
    end
    
    if typeof(pal) <> "palette" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": palette type expected.\n"), "xcosPalExport", "pal"));
    end
    
    if typeof(path) <> "string" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": full path string expected.\n"), "xcosPalExport", "path"));
    end
    fd = mopen(path, "wb");
    [err, msg] = merror(fd);
    if err <> 0 then
        mclose(fd);
        error(msg);
    end
    
    path = fullpath(path);
    mclose(fd);
    
    // export the data to the temp file
    pal = pal; // #7242 workaround
    status = export_to_hdf5(path, "pal");
    if ~status then
        msg = msprintf(gettext("%s: Unable to export the palette to hdf5.\n"), "xcosPalExport");
        if  lhs == 1 then
            error(msg);
        end
    end
endfunction

