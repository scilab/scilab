// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/list_createlist_api"));
cd(pathconvert(TMPDIR+"/list_createlist_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("list_createlist",["list_createlist","list_createlist"],SCI+"/modules/core/tests/unit_tests/list_createlist_api.c",[],[],"",cflags);
exec("loader.sce");

            
size_ref    = 8;
type_ref    = ["constant","string","int16","polynomial", "sparse", "boolean", "boolean sparse", "list"];
dim_ref     = list([3,2],[2,3],[2,3],[3,2],[3,10],[3,3],[3,10],3);

l = list_createlist();
if size(l) <> size_ref then pause;end
for i = 1 : size_ref
    if typeof(l(i)) <> type_ref(i) then pause;end
    if size(l(i)) <> dim_ref(i) then pause;end
end

            
        
