// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
ilib_verbose(0);

// Scalar value
pathTest=SCI+"/modules/core/tests/unit_tests/api_double_test_scalar.c";
cflags="-I"+SCI+"/modules/localization/includes";
ilib_build('foo',['foo_set_scalar','sci_foo_set_scalar';'foo_get_scalar','sci_foo_get_scalar'],pathTest,[],[],"",cflags);

exec loader.sce;

// Create a variable, set it into a function and check that the load is correct 
// and matches what we wrote
a=45;
foo_set_scalar(a)
if foo_get_scalar()<>a then pause, end

b=452;
foo_set_scalar(b)
if foo_get_scalar()<>b then pause, end

b=-42;
foo_set_scalar(b)
if foo_get_scalar()<>b then pause, end


// Matrix
pathTest=SCI+"/modules/core/tests/unit_tests/api_double_test_matrix.c";
ilib_build('foo2',['foo_set_matrix','sci_foo_set_matrix';'foo_get_matrix','sci_foo_get_matrix'],pathTest,[],[],"",cflags);
exec loader.sce;

// Generates a big matrix and to the same test
a=rand(1000,1000);
foo_set_matrix(a)
if foo_get_matrix()<>a then pause, end

// Hardcoded value from C
pathTest=SCI+"/modules/core/tests/unit_tests/api_double_test_hardcoded_matrix.c";
ilib_build('foo3',['foo_get_hardcoded_matrix','sci_foo_get_hardcoded_matrix'],pathTest,[],[],"",cflags);
exec loader.sce;

Foo=[1 4 2 3; ..
    3 9 8 2 ];
if foo_get_hardcoded_matrix()<> Foo then pause, end
