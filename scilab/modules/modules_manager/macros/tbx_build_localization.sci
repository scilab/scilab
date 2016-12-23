// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tbx_build_localization(tbx_name, tbx_path)
    // tbx_build_localization(name, path)   // deprecated (6.0)
    // tbx_build_localization(name)         // deprecated (6.0)
    // tbx_build_localization(path)         // 6.0
    // tbx_build_localization()             // 6.0  path = pwd()

    fname = "tbx_build_localization"
    rhs = argn(2)

    // CHECKING INPUT PARAMETERS
    // -------------------------
    if and(rhs <> [0 1 2]) then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, fname, 0, 1))
    end

    if rhs==2
        msg = "%s: %s(name, path) is obsolete. Please use %s(path) instead.\n"
        warning(msprintf(msg, fname, fname, fname))  // no translation

    elseif rhs==0
        tbx_path = pwd()
    else
        tbx_path = tbx_name
        if type(tbx_path) <> 10 then
            msg = _("%s: Argument #%d: Text(s) expected.\n")
            error(msprintf(msg, fname, rhs))
        end
        tbx_path = tbx_path(1)
        // May be
        //  * either the former tbx_build_localization(tbx_name) (until 5.5.2)
        //  * or the new        tbx_build_localization(tbx_path) (from 6.0.0)
        if grep(tbx_path,["/" "\"])==[] && ~isdir(tbx_path) then // only name was provided
            tbx_path = pwd()
        end
        if ~isdir(tbx_path) then
            msg = _("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n")
            error(msprintf(msg, fname, tbx_path))
        end
    end

    // Is there a locales dir?
    //------------------------
    localePath = pathconvert(tbx_path + "/locales/")
    if isdir(localePath) == %f then
        // No locales is present, nothing to do
        return
    end

    // Retrieving the toolbox name
    // ---------------------------
    tbx_name = tbx_get_name_from_path(tbx_path)

    // find list of .po files
    // ----------------------
    poFiles = gsort(findfiles(localePath, "*.po"), "lr", "i");

    if getos() == "Windows" then
        cmd = SCI + filesep() + "tools/gettext/msgfmt";
    else
        cmd = "msgfmt";
    end

    mprintf(gettext("Generating localization\n"));
    for i=1:size(poFiles, "*")
        //generate moFile name and path
        lang = fileparts(poFiles(i), "fname");
        printf("-- Building for ""%s"" --\n", lang);
        moFile = localePath + lang + "/LC_MESSAGES/";
        mkdir(moFile); //to be sure path exists
        poFile = moFile + tbx_name + ".po";
        moFile = moFile + tbx_name + ".mo";


        //check mo file is newest po, don't need to generate it
        if newest(poFiles(i), moFile) == 1 then
            copyfile(localePath + poFiles(i), poFile);
            cmd1 = cmd + " -o " + moFile + " " + poFile;
            host(cmd1)
        end
    end
endfunction
