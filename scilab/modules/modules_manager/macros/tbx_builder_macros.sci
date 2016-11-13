// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tbx_builder_macros(tbx_path)

    // INITIALIZATIONS
    fname = "tbx_builder_macros"
    if argn(2)~=1 then
        msg = _("%s: Wrong number of input arguments: %d expected.\n")
        error(msprintf(msg, fname, 1))
    end
    if type(tbx_path) <> 10 then
        msg = _("%s: Argument #%d: Text(s) expected.\n")
        error(msprintf(msg, fname, 1))
    end
    tbx_path = tbx_path(1)

    if ~isdir(tbx_path) then
        msg = _("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n")
        error(msprintf(msg, fname, tbx_path))
    end

    // WORK
    // ----
    builder_macros_dir = tbx_path + "/macros/"
    if isdir(builder_macros_dir)
        mprintf(gettext("Building macros...\n"));
        buildmacros_file = findfiles(builder_macros_dir, "buildmacros*.sce");
        builder_file = findfiles(builder_macros_dir, "builder*.sce");
        builder_macros_file = [];
        if ~isempty(buildmacros_file) then
            builder_macros_file = pathconvert(builder_macros_dir + "/" + buildmacros_file, %F);
        end
        if ~isempty(builder_file) then
            builder_macros_file = [builder_macros_file; pathconvert(builder_macros_dir + "/" + builder_file, %F)];
        end
        // checks there is a buildmacros present and if so, executes it with tbx_builder
        if ~isempty(builder_macros_file)
            tbx_builder(builder_macros_file);
        else
            name = tbx_get_name_from_path(tbx_path)
            // Compiles the bin and lib files of files presents in macros
            tbx_build_macros(name, tbx_path + "/macros/")
        end
    end

endfunction
