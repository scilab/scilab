// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO

// This file is released under the 3-clause BSD license. See COPYING-BSD.

mode(-1);
lines(0);

TOOLBOX_NAME  = "toolbox_skeleton";
TOOLBOX_TITLE = "Toolbox Skeleton";
toolbox_dir   = get_absolute_file_path("builder.sce");

// Check Scilab's version
// =============================================================================

try
    v = getversion("scilab");
catch
    error(gettext("Scilab 5.3 or more is required."));
end

if v(2) < 3 then
    // new API in scilab 5.3
    error(gettext('Scilab 5.3 or more is required.'));
end
clear v;

// Check modules_manager module availability
// =============================================================================

if ~isdef('tbx_build_loader') then
  error(msprintf(gettext('%s module not installed."), 'modules_manager'));
end

// Action
// =============================================================================

tbx_builder_macros(toolbox_dir);
tbx_builder_src(toolbox_dir);
tbx_builder_gateway(toolbox_dir);
tbx_builder_help(toolbox_dir);
tbx_build_loader(TOOLBOX_NAME, toolbox_dir);
tbx_build_cleaner(TOOLBOX_NAME, toolbox_dir);

// Clean variables
// =============================================================================

clear toolbox_dir TOOLBOX_NAME TOOLBOX_TITLE;

