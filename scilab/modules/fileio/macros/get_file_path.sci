// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function path = get_file_path(name)
    //-------------------------------
    // returns loader full path
    path ="./";
    [units,typs,nams]=file();
    clear units typs
    for k=size(nams,"*"):-1:1
        l=strindex(nams(k),name);
        if l<>[] then
            path=part(nams(k),1:l($)-1);
            break
        end
    end
    if path == "" then path = pwd() ; end
endfunction
