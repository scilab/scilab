// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tbx_name = tbx_get_name_from_path(tbx_path)
    // Internal function
    // tbx_path: Single string: path pointing to the toolbox root

    fname = "tbx_get_name_from_path"

    if ~isdir(tbx_path) then
        msg = _("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n")
        error(msprintf(msg, fname, tbx_path))
    end

    // Retrieve the toolbox name
    tmp = pathconvert(tbx_path+"/etc/") + "*.start"
    start_file = listfiles(tmp)
    if start_file==[]
        msg = "%s: File ""%s"" required (even if empty).\n" // no translation
        error(msprintf(msg, fname, tmp))
    else
        start_file = start_file(1)
    end
    tbx_name = fileparts(start_file, "fname")
endfunction
