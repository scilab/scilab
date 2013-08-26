//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Clement David
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

function %Block_xcosUpdateBlock(blk)
    // Overload function when calling xcosUpdateBlock with a block

    // generate a unique temp path
    timestamp = getdate();
    tempfile = TMPDIR + filesep() + "xcosUpdateBlock_" + string(timestamp(10)) + ".sod";
    while isfile(tempfile)
        timestamp = timestamp + 1;
        tempfile = TMPDIR + filesep() + "xcosUpdateBlock_" + string(timestamp(10)) + ".sod";
    end

    // export to hdf5
    blk = blk;
    if export_to_hdf5(tempfile, "blk") then
        xcosUpdateBlock(tempfile)
    else
        error(msprintf(gettext("%s: Unable to export %s to %s.\n"), "xcosUpdateBlock", "`blk''", tempfile));
    end

endfunction
