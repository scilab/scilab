// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// help_from_sci

// Check the generation of one help page
my_path = fullfile(SCI,"modules","helptools","tests","unit_tests");
my_macros = "macrosExample.sci";
my_generated_xml = "macrosExample.xml";
target_file = fullfile(TMPDIR,my_generated_xml);
//
// Test #1: without output args
help_from_sci(fullfile(my_path,my_macros), TMPDIR);
assert_checktrue(fileinfo(target_file) <> []);
mdelete(target_file);

//
// Test #2: with 2 output args
[helptxt,demotxt]=help_from_sci(fullfile(my_path,my_macros), TMPDIR);
assert_checktrue(fileinfo(target_file) <> []);
mdelete(target_file);
assert_checktrue(helptxt <> []);
assert_checkequal(typeof(helptxt),"string");
assert_checktrue(demotxt <> []);
assert_checkequal(typeof(demotxt),"string");

