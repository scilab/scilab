// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2021 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
// Get the configuration of the atoms system
//
function result = atomsGetConfig(field)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================
    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"), "atomsGetConfig", 0, 1));
    end

    // Check input parameters type
    // =========================================================================
    if (rhs > 0) & (type(field) <> 10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "atomsGetConfig", 1));
    end

    // Check input parameters dimensions
    // =========================================================================
    if (rhs > 0) & (size(field, "*") <> 1) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"), "atomsGetConfig", 1));
    end


    if rhs <> 0 then
        supported_field = ["useProxy", "proxyHost", "proxyPort", ..
        "proxyUser", "proxyPassword", "offline", ..
        "autoload", "autoloadAddAfterInstall", ..
        "verbose", "downloadTool", "downloadTimeout", "updateTime"];

        if ~or(convstr(supported_field) == convstr(field)) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d.\n"),"atomsGetConfig", 1));
        end
    end

    pref_attrs = ["useProxy", "proxyHost", "proxyPort", "proxyUser", "proxyPassword";
    "enabled", "host", "port", "user", "password"];

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Define the default value of the result according to the number of input
    // arguments
    // =========================================================================

    if rhs == 0 then
        result = struct();
    else
        i = find(pref_attrs(1, :) == field);
        if ~isempty(i) then
            result = getPreferencesValue("//web/body/proxy", pref_attrs(2, i));
            if result == "true" then
                result = "True";
            elseif result == "false" then
                result = "False";
            end
            return;
        end

        result = "";
    end

    // Define the path of the file that will record the change
    // =========================================================================
    atoms_directory = atomsPath("system", "user");

    // Does the SCIHOME/atoms/config exist, if yes load it
    // =========================================================================

    if fileinfo(atoms_directory + "config") <> [] then
        config_lines = mgetl(atoms_directory + "config");
    else
        if (rhs == 0)
            values = getPreferencesValue("//web/body/proxy", pref_attrs(2, :));
            for i = 1:size(pref_attrs, "c")
                if ~isempty(values(i)) then
                    if values(i) == "true" then
                        values(i) = "True";
                    elseif values(i) == "false" then
                        values(i) = "False";
                    end
                    result(pref_attrs(1, i)) = values(i);
                end
            end
        end
        return;
    end

    // Loop on each URL specified as input argument
    // =========================================================================

    for i=1:size(config_lines, "*")
        if isempty( stripblanks(config_lines(i))) then
            continue;
        end

        if regexp(config_lines(i),"/^[a-zA-Z0-9]*\s=\s/","o") == 1 then
            // process field
            current_field_length  = regexp(config_lines(i),"/\s=\s/","o")
            current_field     = part(config_lines(i),1:current_field_length-1);
            current_value     = part(config_lines(i),current_field_length+3:length(config_lines(i)));

            if (rhs == 1) & (current_field == field) then
                result = current_value;
                return;
            elseif(rhs == 0)
                result(current_field) = current_value;
            end
        else
            error(msprintf(gettext("%s: The config file (''%s'') is not well formated at line %d\n"),"atomsGetConfig",atoms_directory+"config",i));
        end
    end

    if (rhs == 0)
        values = getPreferencesValue("//web/body/proxy", pref_attrs(2, :));
        for i = 1:size(pref_attrs, "c")
            if ~isempty(values(i)) then
                if values(i) == "true" then
                    values(i) = "True";
                elseif values(i) == "false" then
                    values(i) = "False";
                end
                result(pref_attrs(1, i)) = values(i);
            end
        end
    end

endfunction
