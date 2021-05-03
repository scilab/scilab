// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function val = %infer_e(field, inferList)
if isdef("DEBUG") & DEBUG, disp("%infer_e"), end

    k = find(fieldnames(inferList)==field)
    
    if k<>[] then
        val = getfield(k+1,inferList)
    else
        msg = _("%s: The field ''%s'' does not exist.\n")
        error(msprintf(msg, "%m2sci.%infer_e", field))
    end
endfunction
