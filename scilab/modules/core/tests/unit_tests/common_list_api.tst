// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/common_list_api"));
cd(pathconvert(TMPDIR+"/common_list_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("common_list",["common_list","common_list"],SCI+"/modules/core/tests/unit_tests/common_list_api.c",[],[],"",cflags);
exec("loader.sce");

            
l1 = [1,2*%i,3;%i,2,3*%i];
l2 = ["may","the";"puffin","be";"with","you"];
l3 = int8([1,2,3]);
l5 = list(l1,l2,l3);
l4 = list(l5, list(l5,l5));
l6 = uint16([1000,2000,3000]);
l = list(l1,l2,l3,l6,l4,l5);
common_list(l)
            
        
