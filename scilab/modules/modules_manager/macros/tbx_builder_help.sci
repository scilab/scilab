// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Run the modules+"/help/builder_help.sce" script if it exists
// See devtools_run_builder

function tbx_builder_help(module)

    fname = "tbx_builder_help"

    // CHECKING module
    // ---------------
    if argn(2)~=1 then
        msg = _("%s: Wrong number of input arguments: %d expected.\n")
        error(msprintf(msg, fname, 1))
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

    // WORK
    // ----
    mprintf(gettext("Building help...\n"))

    builder_help_dir = pathconvert(module + "/help/", %F)

    if isdir(builder_help_dir)
        // Retrieve the toolbox name
        name = tbx_get_name_from_path(module)

        // check there is a builder_help present and if so execute it with tbx_builder
        builder_help_files = findfiles(builder_help_dir, "builder*.sce");
        if ~isempty(builder_help_files)
            builder_help_files = builder_help_dir + "/" + builder_help_files;
            tbx_builder(builder_help_files);
            return
        end
        // Default behaviour when no builder file is present
        // generates the help from the la_LA directories
        d = dir(builder_help_dir);
        d = d.name(d.isdir);
        la_LA = d(grep(d, "/[a-z]{2}_[A-Z]{2}/", "r"));
        if ~isempty(la_LA)
            la_LA = builder_help_dir + "/" + la_LA;
        end

        for i = 1:size(la_LA, "*")
            tbx_build_help(name, la_LA(i))
        end
    end
endfunction
