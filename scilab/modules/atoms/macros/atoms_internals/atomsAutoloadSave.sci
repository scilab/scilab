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

function  atomsAutoloadSave(autoloaded_mat,section)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsAutoloadSave",2));
    end

    // 1st input argument
    // =========================================================================

    if ~isempty(autoloaded_mat) & type(autoloaded_mat) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAutoloadSave",1));
    end

    if ~isempty(autoloaded_mat) & size(autoloaded_mat(1,:),"*") <> 3 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx3 string matrix expected.\n"),"atomsAutoloadSave",1));
    end

    // 2nd input argument
    // =========================================================================

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsAutoloadSave",2));
    end

    if and(section<>["user","allusers"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsAutoloadSave",2));
    end

    // Check if we have the write access
    if (section=="allusers") & ~ atomsAUWriteAccess() then
        error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsAutoloadSave",pathconvert(SCI+"/.atoms")));
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
    autoloaded_txt = atoms_directory+"autoloaded.txt";
    autoloaded_bin = atoms_directory+"autoloaded.bin";

    // Build the installed_str
    // =========================================================================

    if ~ isempty(autoloaded_mat) then
        autoloaded_str = autoloaded_mat(:,1) + " - " + autoloaded_mat(:,2) + " - " +autoloaded_mat(:,3);
    end

    // If packages is empty, no need to keep the files
    // =========================================================================

    if isempty(autoloaded_mat) then
        mdelete(autoloaded_txt);
        mdelete(autoloaded_bin);
        return;
    end

    // write files
    // =========================================================================

    mputl( autoloaded_str , autoloaded_txt);
    save( autoloaded_bin  , "autoloaded_mat");

endfunction
