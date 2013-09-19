// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
