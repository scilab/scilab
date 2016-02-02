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

function  atomsLoadSave(loaded_mat)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsLoadSave",1));
    end

    // 1st input argument
    // =========================================================================

    if ~isempty(loaded_mat) & type(loaded_mat) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsLoadSave",1));
    end

    if ~isempty(loaded_mat) & size(loaded_mat(1,:),"*") <> 3 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx3 string matrix expected.\n"),"atomsLoadSave",1));
    end

    // Create the TMPDIR/atoms directory
    // =========================================================================

    if ~ isdir(TMPDIR+"/atoms") then
        status = mkdir( TMPDIR+"/atoms" );
        if status <> 1 then
            if ATOMSAUTOLOAD then
                mprintf( ..
                gettext("%s: The directory ""%s"" cannot been created, please check if you have write access on this directory.\n"), ..
                "atomsLoadSave", ..
                TMPDIR+"/atoms");
                return;
            else
                error(msprintf( ..
                gettext("%s: The directory ""%s"" cannot been created, please check if you have write access on this directory.\n"), ..
                "atomsLoadSave", ..
                TMPDIR+"/atoms"));
            end
        end
    end

    // Define the path of the file that will record the change
    // =========================================================================
    loaded_txt = pathconvert(TMPDIR+"/atoms/loaded.txt",%F);
    loaded_bin = pathconvert(TMPDIR+"/atoms/loaded.bin",%F);

    // Build the loaded_str
    // =========================================================================

    if ~ isempty(loaded_mat) then
        loaded_str = loaded_mat(:,1) + " - " + loaded_mat(:,2) + " - " +loaded_mat(:,3);
    end

    // If packages is empty, no need to keep the files
    // =========================================================================

    if isempty(loaded_mat) then
        mdelete(loaded_txt);
        mdelete(loaded_bin);
        return;
    end

    // write files
    // =========================================================================

    mputl( loaded_str , loaded_txt);
    save( loaded_bin  , "loaded_mat");

endfunction
