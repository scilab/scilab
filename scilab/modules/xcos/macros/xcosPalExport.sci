//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

// Export a palette instance to an hdf5 file.
//
// @param pal the "palette" type
// @param path Fullpath to the hdf5 file (.sod)
// @return status %T if the export has been successful, %F otherwise
// @return[opt] msg Contains the error message in case of %F status or an empty
//                  string when no error as occurred.
function [status, msg] = xcosPalExport(pal, path)
    // Export a palette instance to a path.
    //
    // Syntax
    //   xcosPalExport(pal, path);
    //   status = xcosPalExport(pal, path);
    //   [status, message] = xcosPalExport(pal, path);
    //
    // Parameters
    //   pal: palette tlist; the palette to export
    //   path: string path; where to export the palette (HDF5 formatted file)
    //   status: boolean; status of the export
    //   msg: string; error message
    //
    // Description
    // Export Scilab palette instance to a file.
    //
    // Examples
    //   loadXcosLibs();
    //   pal = xcosPal();
    //
    //   sumPath = TMPDIR + "/sum.sod";
    //   bigSomPath = TMPDIR + "/sum.sod";
    //
    //   scs_m = SUM_f("define");
    //   save(sumPath, "scs_m");
    //   scs_m = BIGSOM("define");
    //   save(bigSomPath, "scs_m");
    //
    //   pal = xcosPalAddBlock(pal, sumPath);
    //   pal = xcosPalAddBlock(pal, bigSomPath);
    //
    //   xcosPalExport(pal, TMPDIR + "/palette.sod");
    //
    // See also
    //   xcosPal
    //   xcosPalAdd
    //
    // Authors
    //   Clément DAVID

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

    // workaround bug 7242
    pal = pal;

    // export the data to the temp file
    try
        save(path, "pal");
    catch
        msg = msprintf(gettext("%s: Unable to export the palette to hdf5.\n"), "xcosPalExport");
        if  lhs == 1 then
            error(msg);
        end
    end
endfunction

