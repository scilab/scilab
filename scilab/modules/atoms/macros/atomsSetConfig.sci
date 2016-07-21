// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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

// End user function

// Add toolboxes to the list of packages that are automatically loaded at Scilab start
// This function has an impact on the following files :
//  -> ATOMSDIR/config

function nbChanges = atomsSetConfig(field, value)

    rhs        = argn(2);
    nbChanges      = 0;
    systemUpdateNeeded = %F;

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "atomsSetConfig", 2));
    end

    // Check input parameters type
    // =========================================================================

    if type(field) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "atomsSetConfig", 1));
    end

    if type(value) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "atomsSetConfig", 2));
    end

    // field and value must have the same size
    // =========================================================================

    if or( size(field) <> size(value) ) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), "atomsSetConfig", 1, 2));
    end

    pref_attrs = ["useProxy", "proxyHost", "proxyPort", "proxyUser", "proxyPassword";
    "enabled", "host", "port", "user", "password"];

    i=1;
    for element = field(:)'
        if strcmp("verbose",element, "i") == 0 then
            field(i) = convstr(part(element,1),"u") + part(element,2:length(element));
        else
            field(i) = convstr(part(element,1),"l") + part(element,2:length(element));
        end
        element = field(i);

        if element == "offLine" then
            // Prior to version 5.4.0, offline was called Offline
            field(i) = "offline";
            element = "offline";
        end

        if element == "useProxy" ..
            | element == "offline" ..
            | element == "autoload" ..
            | element == "autoloadAddAfterInstall" ..
            | element == "Verbose" ..
            then
            select value(i)
            case "True" then,
            case "False" then,
            case "true" then value(i)="True",
            case "false" then value(i)="False",
            else
                error(msprintf(gettext("%s: Wrong value for input configuration argument: True or False expected.\n"), value(i)));
            end
        elseif element == "proxyHost" ..
            | element == "proxyPort" ..
            | element == "proxyUser" ..
            | element == "proxyPassword" ..
            | element == "downloadTool" ..
            | element == "downloadTimeout" ..
            | element == "updateTime" ..
            then continue;
        else
            error(msprintf(gettext("%s: Wrong key for input configuration argument.\n"), element));
        end
        i = i + 1;
    end

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Define the path of the file that will record the change
    // =========================================================================
    atoms_directory =  atomsPath("system", "user");

    // Does the atoms_directory exist, if not create it
    // =========================================================================

    if ~ isdir(atoms_directory) then
        mkdir(atoms_directory);
    end

    // Get the current config struct
    // =========================================================================

    config_struct = atomsGetConfig();

    // Loop on field
    // =========================================================================

    prefs_kv = [];
    proxy_changes = 0;

    for i=1:size(field, "*")

        if (~isfield(config_struct, field(i))) | (config_struct(field(i)) <> value(i)) then
            nbChanges = nbChanges + 1;
        else
            continue;
        end

        if field(i) == "offline" then
            systemUpdateNeeded = %T;
        end

        k = find(field(i) == pref_attrs(1, :));
        if ~isempty(k) then
            if field(i) == "useProxy" then
                value(i) = convstr(value(i), "l");
            end
            prefs_kv = [prefs_kv [pref_attrs(2, k) ; value(i)]];
            nbChanges = nbChanges - 1;
            proxy_changes = proxy_changes + 1;
        else
            config_struct(field(i)) = value(i);
        end;
    end

    if ~isempty(prefs_kv) then
        setPreferencesValue("//web/body/proxy", prefs_kv);
    end

    // Shortcut
    // =========================================================================
    if nbChanges == 0 then
        nbChanges = proxy_changes;
        return;
    end

    // Apply Changes
    // =========================================================================

    config_fields    = getfield(1, config_struct);
    config_fields(1:2) = [];
    config_fields    = gsort(config_fields);

    config_str = [];

    for i=1:size(config_fields, "*")
        config_str = [ config_str ; config_fields(i) + " = " + config_struct(config_fields(i)) ];
    end

    mputl(config_str, atoms_directory + "config");

    // SystemUpdate
    // =========================================================================

    if systemUpdateNeeded then
        atomsSystemUpdate();
    end

    nbChanges = nbChanges + proxy_changes;
endfunction
