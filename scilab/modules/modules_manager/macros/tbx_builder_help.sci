// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
// Copyright (C) 2016, 2018, 2021 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function tbx_builder_help(module, languages)
    // Run any modules+"/help/builder*.sce" script if it exists
    //
    // languages: vector of la_LA strings. "en", "fr", "ja", "pt", "ru"
    // shortcuts are expanded to "en_US","fr_FR","ja_JP", "pt_BR", "ru_RU"

    fname = "tbx_builder_help"

    // CHECKING module
    // ---------------
    if argn(2)<1 | argn(2)>2 then
        msg = _("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, fname, 1, 2))
    end
    if type(module) <> 10 then
        msg = _("%s: Argument #%d: Text(s) expected.\n")
        error(msprintf(msg, fname, 1))
    end
    module = module(1)

    if ~isdir(module) then
        msg = _("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n")
        error(msprintf(msg, fname, module))
    end

    // Languages
    allLanguages = %T
    if ~isdef("languages","l") then
        languages = []
    else
        allLanguages = %F
        if type(languages)<>10 then
            msg = "%s: Argument #%d: Strings expected.\n";
            error(msprintf(msg, fname, 2));
        end
        la = ["en" "fr" "ja" "pt" "ru"];
        la_LA = ["en_US" "fr_FR" "ja_JP" "pt_BR" "ru_RU"];
        for i = 1:size(la,"*")
            languages(languages==la(i)) = la_LA(i);
        end
        kOK = grep(languages, "/^[a-z]{2}_[A-Z]{2}$/", "r")
        k = setdiff(size(languages,"*"), kOK)
        if k <> []
            msg = _("%s : Argument #%d: Wrong languages specification %s.\n")
            warning(msprintf(msg, fname, 2, "''" + strcat(languages(g),"'',") + "''"))
        end
        if kOK==[]
            msg = _("%s : Argument #%d: No valid languages specification to process.\n")
            error(msprintf(msg, fname, 2))
        end
        language = languages(kOK)
    end

    // WORK
    // ----
    mprintf(gettext("Building help...\n"))

    builder_help_dir = pathconvert(module + "/help/", %F)

    if isdir(builder_help_dir)
        // Retrieve the toolbox name
        name = tbx_get_name_from_path(module)
        TOOLBOX_NAME = name;
        TOOLBOX_TITLE = name;

        // check there is a builder_help present and if so execute it with tbx_builder
        builder_help_files = findfiles(builder_help_dir, "builder*.sce");
        if allLanguages & ~isempty(builder_help_files)
            builder_help_files = builder_help_dir + "/" + builder_help_files;
            for f = builder_help_files'
                exec(f,-1);
            end
            return
        end
        // Default behaviour when no builder file is present
        // generates the help from the la_LA directories
        d = dir(builder_help_dir);
        d = d.name(d.isdir);
        la_LA = d(grep(d, "/[a-z]{2}_[A-Z]{2}/", "r"));
        if ~allLanguages
            la_LA = intersect(la_LA, languages)
        end
        // The help browser renders the DocBook sources directly: what is left
        // to build is the images, generated for every language at once.
        if ~isempty(la_LA)
            tbx_build_help(name, builder_help_dir)
        end
    end
endfunction
