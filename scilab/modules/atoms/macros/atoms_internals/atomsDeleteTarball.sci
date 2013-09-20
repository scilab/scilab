// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Simon GARESTE <simon.gareste@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

// Deletes the tarball used to install a package

function remList = atomsDeleteTarball(package,section,version)

    remList = [];

    // Check input parameters
    // =========================================================================
    rhs = argn(2);

    // Input argument number
    // =========================================================================

    if rhs < 2 | rhs > 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsDeleteTarball",2,3))
    end

    // 1st input argument
    // =========================================================================
    if type(package) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",1));
    end

    if size(package(1,:),"*") > 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",1));
    end

    // 2nd input argument
    // =========================================================================

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",2));
    end

    if size(section(1,:),"*") > 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",2));
    end

    // 3rd input argument
    // =========================================================================
    if rhs==3 then
        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsRemove",2));
        end

        if type(version) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",3));
        end

        if size(version(1,:),"*") > 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",3));
        end
        // If no 3rd argument is provided, 2nd argument is the version, section by default set to "all"
    else
        version = section;
        section = "all";
    end
    // Remove leading and trailing whitespace
    // =========================================================================
    package = stripblanks(package);

    //Getting the list of archives kept
    if section=="user" then
        path = [atomsPath("install","user")+"archives/"];
        tarballs = ls(path);
    elseif section=="allusers"
        path = [atomsPath("install","allusers")+"archives/"];
        tarballs = ls(path);
    else
        path = [atomsPath("install","user")+"archives/",atomsPath("install","allusers")+"archives/"];
        tarballs=[ls(path(1));ls(path(2))];
    end

    selected_package = grep(tarballs,package);

    [nr,nc]=size(selected_package);

    //Deleting archives matching name and version of the module
    for i=1:nc
        ok = grep(tarballs(selected_package(i)),"_"+version+".");
        if ok==1 then
            if section=="user" | section=="allusers" then
                if isfile(path+tarballs(selected_package(i))) then
                    deletefile(path+tarballs(selected_package(i)));
                    atomsDisp(msprintf(gettext("%s%s deleted.\n\n"),path,tarballs(selected_package(i))));
                else
                    atomsDisp(msprintf(gettext("%s%s does not exist.\n\n"),path,tarballs(selected_package(i))));
                end
            else
                if isfile(path(1)+tarballs(selected_package(i))) then
                    deletefile(path(1)+tarballs(selected_package(i)));
                    atomsDisp(msprintf(gettext("%s%s deleted.\n\n"),path(1),tarballs(selected_package(i))));
                else
                    atomsDisp(msprintf(gettext("%s%s does not exist.\n\n"),path(1),tarballs(selected_package(i))));
                end
                if isfile(path(2)+tarballs(selected_package(i))) then
                    deletefile(path(2)+tarballs(selected_package(i)))
                    atomsDisp(msprintf(gettext("%s%s deleted.\n\n"),path(2),tarballs(selected_package(i))));
                else
                    atomsDisp(msprintf(gettext("%s%s does not exist.\n\n"),path(2),tarballs(selected_package(i))));
                end

            end
        end
    end
endfunction
