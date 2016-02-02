// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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

function  atomsSaveInstalled(installed_struct,section)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsSaveInstalled",2));
    end

    // Check type of input argument type
    // =========================================================================

    if type(installed_struct) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsSaveInstalled",1));
    end

    // Process the 2nd input argument : section
    // Allusers can be equal to "user" or "allusers"

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsInstall",2));
    end

    if and(section<>["user","allusers"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsInstall",2));
    end

    // Check if we have the write access
    if (section=="allusers") & ~ atomsAUWriteAccess() then
        error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstall",2,atomsPath("system","allusers")));
    end

    // Define the path of the file that will record the change according to
    // the "allusers" value
    // =========================================================================
    atoms_directory = atomsPath("system",section);

    // Does the atoms_directory exist, if not create it
    // =========================================================================

    if ~ isdir(atoms_directory) then
        mkdir(atoms_directory);
    end

    // Define the path of the file that will record the change
    // =========================================================================
    installed_txt = atoms_directory+"installed.txt";
    installed_bin = atoms_directory+"installed.bin";

    // Build the installed_mat and the installed_str
    // =========================================================================

    installed_mat = [];
    installed_str = [];

    packages      = getfield(1,installed_struct);
    packages(1:2) = [];

    for i=1:size(packages,"*")

        this_package_details  = installed_struct(packages(i));

        this_package_name     = this_package_details(1);
        this_package_version  = this_package_details(2);
        this_package_path     = this_package_details(3);
        this_package_allusers = this_package_details(4);
        this_package_status   = this_package_details(5);

        this_package_path     = strsubst(this_package_path,pathconvert(SCI    ,%F),"SCI");
        this_package_path     = strsubst(this_package_path,pathconvert(SCIHOME,%F),"SCIHOME");

        this_package_details(3)       = this_package_path;
        installed_struct(packages(i)) = this_package_details;

        installed_mat = [ installed_mat ; this_package_name this_package_version this_package_allusers this_package_path this_package_status];
        installed_str = [ installed_str ; "[ "+this_package_name+" - "+this_package_version+" - "+this_package_allusers+" - "+this_package_path+" - "+this_package_status+" ]" ];

    end

    // If packages is empty, no need to keep the files
    // =========================================================================

    if isempty(packages) then
        mdelete(installed_txt);
        mdelete(installed_bin);
        return;
    end

    // write files
    // =========================================================================

    mputl( installed_str , installed_txt);
    save( installed_bin , "installed_struct" , "installed_mat" );

endfunction
