//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3075 -->
//
// <-- Short Description -->
//    get_absolute_file_path() doesn't return error message when the input 
//    argument hasn't the right size or a correct value.
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3075
//

a = mopen(TMPDIR + "/test.sce", "wt");
// Since 5.1.0 returns a path finished by a filesep()
if get_absolute_file_path("test.sce") <> TMPDIR + filesep() then pause,end
mclose(a);

ierr = execstr("get_absolute_file_path("""")", "errcatch");
if ierr <> 999 then pause,end

ierr = execstr("get_absolute_file_path([""jdfkfjdk"", ""dkslkd""])", "errcatch");
if ierr <> 999 then pause,end
