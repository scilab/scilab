// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
