// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5138 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5138
// <-- Short Description -->
//  if the "makefile name" argument is an empty matrix, the generated Makefile is ".mak"

if getos() == 'Windows' then
  ilib_verbose(0);
  mkdir(pathconvert(TMPDIR+"/string_writing_api"));
  cd(pathconvert(TMPDIR+"/string_writing_api"));
  copyfile(SCI+"/modules/api_scilab/tests/unit_tests/string_writing_api.c",pathconvert(TMPDIR+"/string_writing_api/string_writing_api.c",%F));
  cflags = "-I"+SCI+"/modules/localization/includes";
  ilib_build("string_writing",["write_string","write_string"],"string_writing_api.c",[],[],"",cflags);  
  if isfile(TMPDIR+'/string_writing_api/makelib.mak') <> %T then pause,end
end

// =============================================================================
