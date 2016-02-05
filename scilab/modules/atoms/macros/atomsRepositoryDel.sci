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

// End user function

// Remove an URL to the list of repositories, and returns

function nbDel = atomsRepositoryDel(url,section)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Check write access on allusers zone
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();

    rhs   = argn(2);
    nbDel = 0;

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsRepositoryDel",1,2));
    end

    // Check URLs specified as first input argument
    // =========================================================================

    if type(url) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRepositoryDel",1));
    end

    // Allusers/user management
    //
    //   - If section is equal to "all", The repository to remove is searched in
    //     both "user" and "allusers" sections
    //       → SCI/.atoms
    //       → SCIHOME/atoms
    //
    //   - If section is equal to "allusers", The repository to remove is searched
    //     only in the "allusers" section
    //       → SCI/.atoms
    //
    //   - If section is equal to "user", The repository to remove is searched
    //     only in the "user" section
    //        → SCIHOME/atoms
    // =========================================================================

    if rhs == 1 then
        // By default, add the repository for all users (if we have write access
        // of course !)
        if ATOMSALLUSERSWRITEACCESS then
            section = "all";
        else
            section = "user";
        end

    else
        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsRepositoryDel",2));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsRepositoryDel",2));
        end

        // Check if we have the write access
        if or(section==["all","allusers"]) & ~ ATOMSALLUSERSWRITEACCESS then
            error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsRepositoryDel",pathconvert(SCI+"/.atoms")));
        end

    end

    // Define the path of the files that will record the change according to
    // the "section" value and the existence of the latter
    // =========================================================================
    atoms_files = atomsPath("system",section) + "repositories";

    // Loop on each repositories file specified as first input argument
    // =========================================================================

    for i=1:size(atoms_files,"*")

        if fileinfo(atoms_files(i)) == [] then
            continue;
        end

        // Get the URLs list in this file
        repositories = mgetl(atoms_files(i));

        // Loop on each URL specified as first input argument
        for j=1:size(url,"*")
            indice = find( repositories == url(j) );
            if indice <> [] then
                repositories(indice) = [];
                nbDel = nbDel + 1;
            end
        end

        if repositories == [] then
            mdelete(atoms_files(i));
        else
            // Apply changes on this file
            mputl(repositories,atoms_files(i));


        end
    end

    // Delete the packages file (created by atomsDESCRIPTIONget) to force reload
    // the different distant TOOLBOXES files
    // =========================================================================

    if nbDel > 0 then
        atomsDESCRIPTIONget(%T);
    end

endfunction
