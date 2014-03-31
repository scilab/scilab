// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014      - Scilab-Enterprises - Pierre-Aime Agnel <pierre-aime.agnel@scilab-enterprises.com>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Unit test for macro matfile2sci -->

//==============================================================================
//Testing the error messagge on an non empty file with empty content
matfile_content_empty = pathconvert("SCI/modules/m2sci/tests/unit_tests/m2sci_c_empty.mat",%F,%T);
w_file_content_empty  = pathconvert("TMPDIR/w_content_empty.sod",%F,%T);
a_file_content_empty  = pathconvert("TMPDIR/a_content_empty.sod",%F,%T);
errmsg_c_empty = msprintf(_("%s: Could not read variables in %s"), "matfile2sci", matfile_content_empty);

//m2sci_c_empty.mat is not a completely blank file but has no variables
assert_checkerror("matfile2sci(matfile_content_empty, w_file_content_empty, %t)", errmsg_c_empty);
assert_checkerror("matfile2sci(matfile_content_empty, a_file_content_empty, %f)", errmsg_c_empty);

//Check files were not created
assert_checkfalse(isfile(w_file_content_empty));
assert_checkfalse(isfile(a_file_content_empty));

//==============================================================================
//Testing the error message on an empty file
matfile_empty         = pathconvert("SCI/modules/m2sci/tests/unit_tests/m2sci_empty.mat",%F,%T);
w_file_empty          = pathconvert("TMPDIR/w_empty.sod",%F,%T);
a_file_empty          = pathconvert("TMPDIR/a_empty.sod",%F,%T);

errmsg_empty = msprintf(_("%s: Could not read variables in %s"), "matfile2sci", matfile_empty);

assert_checkerror("matfile2sci(matfile_empty, w_file_empty, %t)", errmsg_empty);
assert_checkerror("matfile2sci(matfile_empty, a_file_empty, %f)", errmsg_empty);

//Check files were not created
assert_checkfalse(isfile(w_file_empty));
assert_checkfalse(isfile(a_file_empty));

//==============================================================================
//Testing the error message on a call with third argument different than %t or %f
matfile_notow         = pathconvert("SCI/modules/m2sci/tests/unit_tests/m2sci_empty.mat",%F,%T);
w_file_notow          = pathconvert("TMPDIR/w_empty.sod",%F,%T);

errmsg_notow = msprintf(_("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), "matfile2sci", 3);
errmsg_notow2 = msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "matfile2sci", 3, 1, 1);

assert_checkerror("matfile2sci(matfile_empty, w_file_empty, ""a wrong value"")", errmsg_notow);
assert_checkerror("matfile2sci(matfile_empty, w_file_empty, [%t, %f, %t])", errmsg_notow2);

//==============================================================================
//Testing non empty files
matfile_not_empty1    = pathconvert("SCI/modules/m2sci/tests/unit_tests/m2sci_n_empty1.mat",%F,%T);
matfile_not_empty2    = pathconvert("SCI/modules/m2sci/tests/unit_tests/m2sci_n_empty2.mat",%F,%T);
w_file_not_empty      = pathconvert("TMPDIR/w_not_empty.sod",%F,%T);
def_file_not_empty    = pathconvert("TMPDIR/def_not_empty.sod",%F,%T);
a_file_not_empty      = pathconvert("TMPDIR/a_not_empty.sod",%F,%T);

//Matfile m2sci_n_empty1.mat contains:
// tst_append_double1  = 1;
// tst_append_mdouble1 = [0,1.1,2,3.3];
// tst_append_str1     = "a tested string";

//Matfile m2sci_n_empty2.mat contains:
// tst_append_double2  = 2;
// tst_append_mdouble2 = [11,12.1,12,13.3];
// tst_append_str2     = "a tested string2";

var_ne1 = ["tst_append_double1","tst_append_mdouble1","tst_append_str1"];
var_ne2 = ["tst_append_double2","tst_append_mdouble2","tst_append_str2"];


//==============================================================================
//Testing append Default behaviour
//def_file_not_empty : appended values of matfile_not_empty1 and 2
matfile2sci(matfile_not_empty1, def_file_not_empty);
matfile2sci(matfile_not_empty2, def_file_not_empty);

//File must be written
assert_checktrue(isfile(def_file_not_empty));

load(def_file_not_empty);
//if not overwritten all values from both files should exist
for v = var_ne1
    assert_checktrue(exists(v)==1);
end

for v = var_ne2
    assert_checktrue(exists(v)==1);
end

//clearing variables for future tests
for v = [var_ne1, var_ne2]
    clear(v);
end

//==============================================================================
//File not empty with append flag = default behaviour
matfile2sci(matfile_not_empty1, a_file_not_empty, %f);
matfile2sci(matfile_not_empty2, a_file_not_empty, %f);

//File must be written
assert_checktrue(isfile(a_file_not_empty));

load(a_file_not_empty);
//if not overwritten all values from both files should exist
for v = var_ne1
    assert_checktrue(exists(v)==1);
end

for v = var_ne2
    assert_checktrue(exists(v)==1);
end

//clearing variables for future tests
for v = [var_ne1, var_ne2]
    clear(v);
end

//==============================================================================
//Testing the writing flag
matfile2sci(matfile_not_empty1, w_file_not_empty, %t);

//File must be written
assert_checktrue(isfile(w_file_not_empty));

//Checks variables in the file written
load(w_file_not_empty);
for v = var_ne1
    assert_checktrue(exists(v)==1);
    clear(v) //clearing to have an empty variable for next test
end

matfile2sci(matfile_not_empty2, w_file_not_empty, %t);

//File must be written
assert_checktrue(isfile(w_file_not_empty));

//File w_file_not_empty must be overwritten
load(w_file_not_empty);
for v = var_ne1
    assert_checkfalse(exists(v)==1); //variables in matfile_not_empty1 must not be in the file overwritten
end

for v = var_ne2
    assert_checktrue(exists(v)==1);
end

//clearing variables for future tests
for v = [var_ne1, var_ne2]
    clear(v); // clear is silent on unexisting variables
end
//==============================================================================
