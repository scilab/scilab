// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=isanmfile(nam)

    // Try to find if nam is the name of a M-file
    r=%F

    // If a translation function exists
    if exists("sci_"+nam)==1 then
        r=%T
        return
    end

    // If this function is part of known untranslated functions
    // Special case of untranslated function without arguments
    if find(nam==["grid","hold","gca"])<>[] then
        r=%T
        return
    end

    // If recursive mode, try to find M-file in user defined paths
    if Recmode then
        for k=1:size(Paths,"*")
            [u,ierr]=file("open",Paths(k)+nam+".m","old")
            if ierr==0 then
                file("close",u)
                r=%T
                break
            end
        end
    else
        // Try to find file in working directory
        [u,ierr]=file("open",file_path+nam+".m","old")
        if ierr==0 then
            file("close",u)
        end
        r=(ierr==0)
    end
endfunction
