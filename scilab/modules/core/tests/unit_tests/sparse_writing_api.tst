// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/sparse_writing_api"));
cd(pathconvert(TMPDIR+"/sparse_writing_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("sparse_writing",["write_sparse","write_sparse"],SCI+"/modules/core/tests/unit_tests/sparse_writing_api.c",[],[],"",cflags);
exec("loader.sce");

            
sp_ref = sparse([1,8;2,4;2,7;3,2],[1+4*%i,2+3*%i,3+2*%i,4+%i], [3,10]);
sp = tonio();
if or(sp <> sp_ref) then pause;end
            
        
