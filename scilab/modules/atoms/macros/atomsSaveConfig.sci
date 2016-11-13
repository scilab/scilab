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

// Save the config file in a config.bak file, so that we don't lose information while testing

function atomsSaveConfig(force)

    rhs = argn(2);

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsSaveConfig",0,1));
    end

    load("SCI/modules/atoms/macros/atoms_internals/lib");

    if rhs == 0 then
        force = %F;
    end

    pathsystemuser = atomsPath("system", "user");

    proxy_attrs = ["enabled", "host", "port", "user", "password"];
    setPreferencesValue("//web/body/previous-proxy", [proxy_attrs ; getPreferencesValue("//web/body/proxy", proxy_attrs)]);

    if isfile(pathsystemuser + "config.bak") then
        if ~force then
            error(msprintf("The file config.bak already exists, aborting.\n"));
        elseif isfile(pathsystemuser + "config") then
            copyfile(pathsystemuser + "config", pathsystemuser + "config.bak");
            msprintf(gettext("config file saved to %s"), pathsystemuser + "config.bak");
        else
            error(msprintf("No config available, aborting.\n"));
        end
    elseif isfile(pathsystemuser + "config") then
        copyfile(pathsystemuser + "config", pathsystemuser + "config.bak");
        msprintf(gettext("config file saved to %s"), pathsystemuser + "config.bak");
    elseif ~isdir(pathsystemuser) then
        mkdir(pathsystemuser)
        mputl("", pathsystemuser + "config");
        mputl("", pathsystemuser + "config.bak");
    else
        mputl("", pathsystemuser + "config");
        mputl("", pathsystemuser + "config.bak");
    end

endfunction
