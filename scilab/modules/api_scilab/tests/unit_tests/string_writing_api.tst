// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/string_writing_api"));
cd(pathconvert(TMPDIR+"/string_writing_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/string_writing_api.c",pathconvert(TMPDIR+"/string_writing_api/string_writing_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("string_writing",["write_string","write_string"],"string_writing_api.c",[],"Makefile","",cflags);
exec("loader.sce");
 
a_ref = "may the puffin be with you ";
b = [];
a = write_string();
for i=1:size(a,"r")
    for j=1:size(a,"c")
        b = b + a(i,j);
        b = b + " ";
    end
end
if b <> a_ref then pause;end
 
