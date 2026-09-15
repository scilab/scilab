// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2011 - DIGITEO

// This file is released under the 3-clause BSD license. See COPYING-BSD.

mode(-1);
lines(0);

function main_builder()

    TOOLBOX_NAME  = "xcos_toolbox_skeleton";
    TOOLBOX_TITLE = "Xcos toolbox skeleton";
    toolbox_dir   = get_absolute_file_path("builder.sce");

    if ~isdir(toolbox_dir+filesep()+"images"+filesep()+"h5")
        [status, msg] = mkdir(toolbox_dir+filesep()+"images"+filesep()+"h5");
        if and(status <> [1 2])
            error(msg);
        end
    end

    // Action
    // =============================================================================

    tbx_builder_macros(toolbox_dir);
    tbx_builder_src(toolbox_dir);
    tbx_builder_gateway(toolbox_dir);
    tbx_builder_help(toolbox_dir); // no jar: pre-generates the help images
    tbx_build_loader(toolbox_dir);
    tbx_build_cleaner(toolbox_dir);
endfunction

if with_module("xcos") then
    main_builder();
    clear main_builder; // remove main_builder on stack
end
