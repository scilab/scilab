// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

gatewayName = "bug_12426_gateway";
lib1 = "api_scilab";
lib1include = SCI + "/modules/" + lib1 + "/includes";
func1 = "addErrorMessage";

previous_dir = pwd();
cd(get_absolute_file_path("build_" + gatewayName + ".sce"));
current_dir = pwd();
mkdir(TMPDIR + "/" + gatewayName);
cd(TMPDIR + "/" + gatewayName);

ilib_name = gatewayName;
files = ["sci_" + func1 + ".c"];

for i = 1 : size(files, "*")
    copyfile(current_dir + "/" + files(i), TMPDIR + "/" + gatewayName);
end

if getos() == "Windows" then
    libs   = SCI  + "/bin/" + lib1;
    cflags = "-I " + lib1include;

elseif isdir(lib1include)
    // Unix source version
    libs   = SCI + "/modules/" + lib1 + "/.libs/lib" + lib1;
    cflags = "-I " lib1include;
else
    // Unix binary version
    SCI_LIB     = strsubst(SCI,"/share\/scilab$/", "lib/scilab", "r");
    SCI_INCLUDE = strsubst(SCI,"/share\/scilab$/", "include/scilab", "r");
    libs        = SCI_LIB + "/lib" + lib1 + ..
    SCI_LIB + "/lib" + lib1;
    cflags      = "-I " + SCI_INCLUDE;
end

// name known in scilab, C function called
table = [func1, "sci_" + func1];

libname = ilib_build(ilib_name, table, files, libs, [], "", cflags);

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
