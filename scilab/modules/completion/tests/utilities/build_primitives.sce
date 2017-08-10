// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

previous_dir = pwd();
cd(get_absolute_file_path("build_primitives.sce"));
file_path = pwd();
mkdir(TMPDIR + "/completion");
cd(TMPDIR + "/completion");

ilib_name   = "completion_c";
files       = [ "sci_completeline.c"    , ..
"sci_getpartlevel.c"    , ..
"sci_getfilepartlevel.c", ..
"sci_getcommonpart.c"   , ..
"sci_getfields.c" ];

for i = 1 : size(files, "*")
    copyfile(file_path + "/" + files(i), TMPDIR + "/completion");
end

if getos() == "Windows" then
    libs   = SCI   + "/bin/scicompletion";
    cflags = "-I " + SCI +"/modules/completion/includes";

elseif isdir(SCI+"/modules/completion/includes")
    // Unix source version
    libs   = SCI + "/modules/completion/.libs/libscicompletion";
    cflags = "-I " + SCI +"/modules/completion/includes -I " + SCI + "/modules/localization/includes";

else
    // Unix binary version
    SCI_LIB     = strsubst(SCI,"/share\/scilab$/","lib/scilab"     ,"r");
    SCI_INCLUDE = strsubst(SCI,"/share\/scilab$/","include/scilab" ,"r");
    libs        = SCI_LIB + "/libscicompletion";
    cflags      = "-I " + SCI_INCLUDE
end

// name known in scilab , C function called
table = ["completeline"    ,"sci_completeline"    ;
"getpartlevel"    ,"sci_getpartlevel"    ;
"getfilepartlevel","sci_getfilepartlevel";
"getcommonpart"   ,"sci_getcommonpart"   ;
"getfields"       ,"sci_getfields" ];


ilib_build(ilib_name,table,files,libs,[],"",cflags);

cd(previous_dir);

clear ilib_build;
clear libname;
clear table;
clear libs;
clear files;
clear ilib_name;
clear previous_dir;
clear gateway_c_dir;
clear SCI_LIB;
clear SCI_INCLUDE;
