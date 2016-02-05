// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
    libs   = SCI + "/modules/" + lib1 + "/.libs/libsci" + lib1;
    cflags = "-I " + lib1include;
else
    // Unix binary version
    SCI_LIB     = strsubst(SCI,"/share\/scilab$/", "lib/scilab", "r");
    SCI_INCLUDE = strsubst(SCI,"/share\/scilab$/", "include/scilab", "r");
    libs        = SCI_LIB + "/libscilab-cli";
    cflags      = "-I " + SCI_INCLUDE;
end

// name known in scilab, C function called
table = [func1, "sci_" + func1];

ilib_build(ilib_name, table, files, libs, [], "", cflags);

cd(previous_dir);

clear ilib_build;
clear table;
clear libs;
clear files;
clear ilib_name;
clear previous_dir;
clear gateway_c_dir;
clear SCI_LIB;
clear SCI_INCLUDE;
