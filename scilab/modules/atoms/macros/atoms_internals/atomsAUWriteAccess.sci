// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
        mkdir(SCI + "/.scilab");
        rmdir(SCI + "/.scilab");
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
