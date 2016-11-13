// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Call .quit file of loaded modules

function result = atomsQuit()

    result = %T;

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // If the autoload system is disabled, no need to continue
    // =========================================================================
    if atomsGetConfig("autoload") == "False" then
        return;
    end

    atomsModulesLoaded = atomsGetLoaded();
    sizeLoaded = size(atomsModulesLoaded);
    for i = 1:sizeLoaded(1)

        this_package_name = atomsModulesLoaded(i, 1);
        this_package_path = atomsModulesLoaded(i, 4);
        this_package_version = atomsModulesLoaded(i, 2);

        if ~isempty(this_package_path) then
            if isdir(this_package_path) then
                moduleLoadedFullFilename = fullfile(this_package_path, "unloader.sce");
                if isfile(moduleLoadedFullFilename) then
                    if exec(moduleLoadedFullFilename, "errcatch") <> 0 then
                        txt = msprintf(gettext("%s: An error occurred while unloading ''%s-%s'':\n"), "atomsQuit", this_package_name, this_package_version);
                        warning(txt);
                        result = %F;
                    end
                end
            end
        end
    end
endfunction
