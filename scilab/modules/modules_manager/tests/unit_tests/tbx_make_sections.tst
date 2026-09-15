// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK ERROR OUTPUT -->
// <-- ENGLISH IMPOSED -->

// Test the compilation of a simple toolbox with tbx_make(module, section)

// Copying the testing toolbox from the test environment
function reset_test_module(source, destDir)
    if isdir(destDir)
        if getos()=="Windows"
            host("rmdir /Q /S """+destDir+"""")
        else
            removedir(destDir);
        end
    end
    status = copyfile(source, destDir);
    assert_checkequal(status, 1);
endfunction
source = SCI + "/modules/modules_manager/tests/tbx/testbox/";
mytb_dir = TMPDIR + "\testbox";

// Build only macros
// -----------------
reset_test_module(source, mytb_dir);
tbx_make(mytb_dir, "macros");
assert_checktrue(isfile(mytb_dir + "/macros/lib"));
assert_checktrue(isfile(mytb_dir + "/macros/scilab_sum.bin"));

assert_checkfalse(isdir(mytb_dir + "/jar"));
subDirs = mytb_dir + "/src/" + ["c" "java" "fortran"];
assert_checkfalse(or(isfile(subDirs + "/loader.sce")));
assert_checkfalse(isfile(mytb_dir + "/sci_gateway/loader_gateway.sce"));
subDirs = mytb_dir + "/locales/" + ["en_US" "fr_FR"] + "/LC_MESSAGES/";
assert_checkfalse(or(isfile(subDirs + "testbox.mo")));
assert_checkfalse(or(isfile(subDirs + "testbox.po")));

// Build only help pages
// ---------------------
// All languages
reset_test_module(source, mytb_dir);
tbx_make(mytb_dir, "help");
assert_checktrue(isdir(mytb_dir + "/help/images/"));
assert_checktrue(isfile(mytb_dir + "/help/images/help_images.index"));

assert_checkfalse(isfile(mytb_dir + "/macros/lib"));
assert_checkfalse(isfile(mytb_dir + "/macros/scilab_sum.bin"));
subDirs = mytb_dir + "/src/" + ["c" "java" "fortran"];
assert_checkfalse(or(isfile(subDirs + "/loader.sce")));
assert_checkfalse(isfile(mytb_dir + "/sci_gateway/loader_gateway.sce"));
subDirs = mytb_dir + "/locales/" + ["en_US" "fr_FR"] + "/LC_MESSAGES/";
assert_checkfalse(or(isfile(subDirs + "testbox.mo")));
assert_checkfalse(or(isfile(subDirs + "testbox.po")));

// Build only locales
// ------------------
reset_test_module(source, mytb_dir);
tbx_make(mytb_dir, "localization");
subDirs = mytb_dir + "/locales/" + ["en_US" "fr_FR"] + "/LC_MESSAGES/";
assert_checktrue(and(isfile(subDirs + "testbox.mo")));
assert_checktrue(and(isfile(subDirs + "testbox.po")));

assert_checkfalse(isfile(mytb_dir + "/macros/lib"));
assert_checkfalse(isdir(mytb_dir + "/jar"));
subDirs = mytb_dir + "/src/" + ["c" "java" "fortran"];
assert_checkfalse(or(isfile(subDirs + "/loader.sce")));
assert_checkfalse(isfile(mytb_dir + "/sci_gateway/loader_gateway.sce"));

// Build sources and gateways
// --------------------------
reset_test_module(source, mytb_dir);
tbx_make(mytb_dir, ["src" "sci_gateway"]);
assert_checktrue(isfile(mytb_dir + "/sci_gateway/loader_gateway.sce"));

assert_checkfalse(isfile(mytb_dir + "/macros/lib"));
assert_checktrue(isdir(mytb_dir + "/jar"));
subDirs = mytb_dir + "/src/" + ["c" "java" "fortran"];
assert_checktrue(or(isfile(subDirs + "/loader.sce")));
subDirs = mytb_dir + "/locales/" + ["en_US" "fr_FR"] + "/LC_MESSAGES/";
assert_checkfalse(or(isfile(subDirs + "testbox.mo")));
assert_checkfalse(or(isfile(subDirs + "testbox.po")));
