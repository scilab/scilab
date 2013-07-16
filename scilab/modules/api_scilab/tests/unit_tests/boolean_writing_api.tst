// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/boolean_writing_api"));
cd(pathconvert(TMPDIR+"/boolean_writing_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/boolean_writing_api.c",pathconvert(TMPDIR+"/boolean_writing_api/boolean_writing_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("boolean_writing",["write_boolean","write_boolean"],"boolean_writing_api.c",[],"","",cflags);
exec("loader.sce");

a_ref = [ %t  %f  %t  %f
          %t  %f  %t  %f
          %t  %f  %t  %f ];
    
b_ref =     [ %f  %t  %f  %t  %f  %t
              %f  %t  %f  %t  %f  %t
              %f  %t  %f  %t  %f  %t
              %f  %t  %f  %t  %f  %t ];

[a,b] = write_boolean();
if or(a <> a_ref) then pause;end
if or(b <> b_ref) then pause;end
