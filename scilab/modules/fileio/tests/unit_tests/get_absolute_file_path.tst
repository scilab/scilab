// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// Copyright (C) 2020 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

mess = msprintf(_("%s: Wrong number of input argument(s): at most %d expected.\n"),"get_absolute_file_path",1)
assert_checkerror("get_absolute_file_path(''test.sce'',''test.sce'')",mess);

current_dir = pwd()+ filesep();

a = mopen(TMPDIR+'/test.sce','wt');
// get_absolute_file_path since 5.1.1 returns path with path separator @ the end
d1 = get_absolute_file_path('test.sce');
mclose(a);
assert_checkequal(d1,TMPDIR + filesep());

// file 'test.sce' closed then not found
mess = msprintf(_("%s: The file %s is not opened in scilab.\n"),"get_absolute_file_path","test.sce");
assert_checkerror("get_absolute_file_path(''test.sce'')",mess);

// one input arg form, within new script
mputl("filepath = get_absolute_file_path(""script1.sce"")",TMPDIR+'/script1.sce');
exec(TMPDIR+'/script1.sce');
assert_checkequal(filepath,TMPDIR + filesep());

// one input arg form, within a function in a new script
mputl(["function fp = test()"
       "fp = get_absolute_file_path(""script2.sce"")"
       "end"
       "filepath = test()"],TMPDIR+'/script2.sce');
exec(TMPDIR+'/script2.sce');
assert_checkequal(filepath,TMPDIR + filesep());

// no input arg form, within new script
mputl("[filepath,filename] = get_absolute_file_path()",TMPDIR+'/script3.sce');
exec(TMPDIR+'/script3.sce');
assert_checkequal(filepath,TMPDIR + filesep());
assert_checkequal(filename,"script3.sce");

// no input arg form, within a function in a new script
mputl(["function [fp,fn] = test()"
       "[fp,fn] = get_absolute_file_path()"
       "end"
       "[filepath,filename] = test()"],TMPDIR+'/script4.sce');
exec(TMPDIR+'/script4.sce');
assert_checkequal(filepath,TMPDIR + filesep());
assert_checkequal(filename,"script4.sce");

// no input arg form, within current script
// filepath is TMPDIR of scilab instance calling the test
[filepath1,filename] = get_absolute_file_path();
[filepath2] = get_absolute_file_path("get_absolute_file_path.tst");
assert_checkequal(filename,"get_absolute_file_path.tst");
assert_checkequal(filepath1,filepath2);

