// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/sparse_reading_api"));
cd(pathconvert(TMPDIR+"/sparse_reading_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("sparse_reading",["read_sparse","read_sparse"],SCI+"/modules/core/tests/unit_tests/sparse_reading_api.c",[],[],"",cflags);
exec("loader.sce");

            
sp=sparse([1,2;4,5;3,10],[1 + 2*%i,2 - 3*%i,-3 + 4*%i]);
read_sparse(sp);
            
        
