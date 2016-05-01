//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Clement David
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
    try
        save(tempfile, "blk")
        xcosUpdateBlock(tempfile)
    catch
        error(msprintf(gettext("%s: Unable to export %s to %s.\n"), "xcosUpdateBlock", "`blk''", tempfile));
    end

endfunction
