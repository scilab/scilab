// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent COUVERT
// Copyright (C) INRIA - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function mdelete(filename)

    // Scilab Project - V. Couvert
    // Delete a file given by filename (or a set of files if meta-characters used in filename)

    // Modified by Pierre MARECHAL
    // Scilab Team
    // Date : 2006-06-29
    // Fix the bug2041

    // Date : 2007-01-16
    // Fix the bug2288 (Thanks to M. Dubois, http://dubois.ensae.net )

    rhs = argn(2);
    if rhs <> 1 then
        error(77);
    end

    if type(filename) <> 10 | size(filename,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument: String expected.\n"),"mdelete"));
    end

    // Handle file path
    if getos() == "Windows" then
        filename = strsubst(filename,filesep(),"/");
    end

    // File path
    k = strindex(filename,"/");

    if k == [] then
        file_path = "./";
    else
        file_path = part(filename,1:k($));
        filename  = part(filename,k($)+1:length(filename));
    end

    if getos() == "Windows" then
        fullfilename = strsubst(file_path+filename,"/","\");
        lst_files    = listfiles(fullfilename);
    else
        lst_files = listfiles(file_path+filename);
    end

    if lst_files<>[] then
        for i=1:size(lst_files,"*")
            deletefile(lst_files(i));
        end
    end

endfunction
