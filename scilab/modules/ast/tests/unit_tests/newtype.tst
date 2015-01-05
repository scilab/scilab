// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/newtype"));
cd(pathconvert(TMPDIR+"/newtype"));
copyfile(SCI+"/modules/ast/tests/unit_tests/sci_newtype.cpp",pathconvert(TMPDIR+"/newtype/sci_newtype.cpp",%F));
copyfile(SCI+"/modules/ast/tests/unit_tests/newtype.hxx",pathconvert(TMPDIR+"/newtype/newtype.hxx",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
cflags = cflags + " -I"+TMPDIR+"/newtype";
ilib_build("gw_newtype",["newtype","sci_newtype", "cppsci"],"sci_newtype.cpp",[],"","",cflags);
exec("loader.sce");


// toString call
a = newtype();

// check value
assert_checkequal(string(a), "This is my type.");

// check scilab display
a
disp(a)
l=list(2, a, "test")

// overload call
b = newtype(%f);
function %nt_p(h)
    disp("%nt_p: This is my type.");
end

b
disp(b)
l=list(2, b, "test")
