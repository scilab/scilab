// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Simon GARESTE <simon.gareste@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Restore the config file using the config.bak file

function atomsRestoreConfig(quiet)

    rhs = argn(2);

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "atomsSaveConfig", 0, 1));
    end

    load("SCI/modules/atoms/macros/atoms_internals/lib");

    if rhs == 0 then
        quiet = %F;
    end

    proxy_attrs = ["enabled", "host", "port", "user", "password"];
    setPreferencesValue("//web/body/proxy", [proxy_attrs ; getPreferencesValue("//web/body/previous-proxy", proxy_attrs)]);

    pathsystemuser = atomsPath("system", "user");

    if isfile(pathsystemuser + "config.bak") then
        movefile(pathsystemuser + "config.bak", pathsystemuser + "config");
    elseif ~quiet then
        error(msprintf(gettext("File %s does not exist: could not restore previous configuration."), pathsystemuser + "config.bak"));
    end

endfunction
