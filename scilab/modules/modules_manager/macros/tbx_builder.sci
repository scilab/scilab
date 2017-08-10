// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function tbx_builder(files)

    if type(files) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string array expected.\n"),"tbx_builder",1));
    end

    for i=1:size(files,"*") do

        if ~isfile(files(i)) then
            error(msprintf(gettext("%s: The file ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder",files(i)));
        end

        exec(files(i),-1);

    end

endfunction
