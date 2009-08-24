// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/string_writing_api"));
cd(pathconvert(TMPDIR+"/string_writing_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("string_writing",["write_string","write_string"],SCI+"/modules/core/tests/unit_tests/string_writing_api.c",[],[],"",cflags);
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
            
        
