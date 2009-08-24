// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/list_getlist_api"));
cd(pathconvert(TMPDIR+"/list_getlist_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("list_getlist_api",["list_getlist","list_getlist"],SCI+"/modules/core/tests/unit_tests/list_getlist_api.c",[],[],"",cflags);
exec("loader.sce");

            
M=mlist(['V','name','value'],['a','b','c'],[1 2 3]);
T=tlist(['V','value1','value2','value3'],['a','b','c'], [1,2,3], int32([1,2,3]));
L=list(M,T);
list_getlist(L)
            
        
