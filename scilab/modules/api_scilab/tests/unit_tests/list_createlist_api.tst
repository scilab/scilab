// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/list_createlist_api"));
cd(pathconvert(TMPDIR+"/list_createlist_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/list_createlist_api.c",pathconvert(TMPDIR+"/list_createlist_api/list_createlist_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("gw_list_createlist",["list_createlist","list_createlist"],"list_createlist_api.c",[],"","",cflags);
exec("loader.sce");

size_ref    = 8;
type_ref    = ["constant","string","int16","polynomial", "sparse", "boolean", "boolean sparse", "list"];
dim_ref     = list([3,2],[2,3],[2,3],[3,2],[3,10],[3,3],[3,10],2);
l = list_createlist();
if size(l) <> size_ref then pause;end
for i = 1 : size_ref
    if typeof(l(i)) <> type_ref(i) then pause;end
    if size(l(i)) <> dim_ref(i) then pause;end
end
