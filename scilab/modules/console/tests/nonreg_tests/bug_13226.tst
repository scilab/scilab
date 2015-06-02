// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 13226 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13226
//
// <-- Short Description -->
// Completion with accentued chars could lead to a a crash
//
// <-- LINUX ONLY -->
file_path = SCI + "/modules/console/tests/nonreg_tests/";
mkdir(TMPDIR + "/console");
cd(TMPDIR + "/console");

ilib_verbose(0);
ilib_name   = "console_c";
files       = "bug_13226.c";

for i = 1 : size(files, "*")
    copyfile(file_path + "/" + files(i), TMPDIR + "/console");
end

if isdir(SCI+"/modules/console/includes") then
    // Unix source version
    libs   = SCI + "/modules/console/.libs/libsciconsole";
    cflags = "-I " + SCI +"/modules/console/includes -I " + SCI + "/modules/localization/includes";

else
    // Unix binary version
    SCI_LIB     = strsubst(SCI,"/share\/scilab$/","lib/scilab"     ,"r");
    SCI_INCLUDE = strsubst(SCI,"/share\/scilab$/","include/scilab" ,"r");
    libs        = SCI_LIB + "/libsciconsole";
    cflags      = "-I " + SCI_INCLUDE;
end

// name known in scilab , C function called
table = ["completecommandline"    ,"sci_completecommandline"];
ilib_build(ilib_name,table,files,libs,[],"",cflags);

clear ilib_build;
clear table;
clear libs;
clear files;
clear ilib_name;
clear gateway_c_dir;
clear SCI_LIB;
clear SCI_INCLUDE;

ierr = exec(TMPDIR + "/console/loader.sce", "errcatch", -1);

createdir(TMPDIR+"/Téléchargements");
mputl("",TMPDIR+"/Téléchargements/foo.bar");
cd(TMPDIR);

[r,c] = completecommandline("atomsInstall(""./Téléchargements/f",33);
assert_checkequal(r,"atomsInstall(""./Téléchargements/foo.bar");
assert_checkequal(c,39);
