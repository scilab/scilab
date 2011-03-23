//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function %diagram_xcos(scs_m)
// Overload function when calling xcos with a diagram
//
// Calling Sequence
//  xcos(scs_m);
//
// Parameters
//  scs_m: the diagram instance

    // generate a unique temp path
    timestamp = getdate();
    tempfile = TMPDIR + filesep() + "xcos_scs_m_" + string(timestamp(10)) + ".h5";
    while isfile(tempfile)
        timestamp = timestamp + 1;
        tempfile = TMPDIR + filesep() + "xcos_scs_m_" + string(timestamp(10)) + ".h5";
    end

    // export to hdf5
    if export_to_hdf5(tempfile, "scs_m") then
        // load the exported diagram
        xcos(tempfile);
    else
        error(msprintf(gettext("%s: Unable to export %s to %s.\n"), "xcos", "`scs_m''", tempfile));
    end
endfunction
