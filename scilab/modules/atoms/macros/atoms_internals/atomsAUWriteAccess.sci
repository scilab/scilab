// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Add an URL to the list of repositories, and returns

function res = atomsAUWriteAccess()

    res = %F;

    // Cache
    if isdef("ATOMSALLUSERSWRITEACCESS") then
        res = ATOMSALLUSERSWRITEACCESS;
        return;
    end

    atoms_system_directory  = atomsPath("system" ,"allusers");
    atoms_install_directory = atomsPath("install","allusers");

    // Physical test

    if ~ isdir(atoms_system_directory) then
        if mkdir(atoms_system_directory) <> 1 then
            return;
        end
    end

    if execstr("mputl(""dummy"",atoms_system_directory+""dummy"");","errcatch") == 0 then
        res = %T;
        mdelete(atoms_system_directory+"dummy");
    else
        return;
    end

    if execstr("mputl(""dummy"",atoms_install_directory+""dummy"");","errcatch") <> 0 then
        res = %F;
    else
        mdelete(atoms_install_directory+"dummy");
    end

endfunction
