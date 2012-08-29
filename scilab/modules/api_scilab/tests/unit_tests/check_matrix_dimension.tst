// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/check_matrix_dimension"));
cd(pathconvert(TMPDIR+"/check_matrix_dimension"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/check_matrix_dimension.c",pathconvert(TMPDIR+"/check_matrix_dimension/check_matrix_dimension.c",%F));
ilib_build("gw_check_matrix_dimension",["check_matrix_dimension","check_matrix_dimension"],"check_matrix_dimension.c",[]);
exec("loader.sce");

             
check_matrix_dimension([1,2,3;4,5,6]);
check_matrix_dimension([1,2,3]);
check_matrix_dimension([1;2;3]);
check_matrix_dimension([1]);
check_matrix_dimension([1,2;3,4]);
check_matrix_dimension([]);

       
