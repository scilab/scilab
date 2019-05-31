// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// ============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// unit tests for atomsGetInstalledPath()

offlineStatus = atomsGetConfig("offline");
atomsSetConfig("offline","True");
atomsInstall("SCI/modules/atoms/tests/unit_tests/toolbox_7V6_1.0-1.bin.zip", "user");

if getos() == "Windows" then
    ref = pathconvert("SCIHOME\atoms\x64\toolbox_7V6\1.0", %f, %f);
else
    ref = pathconvert("SCIHOME\atoms\toolbox_7V6\1.0", %f, %f);
end

p = atomsGetInstalledPath("toolbox_7V6");   // exact technical name
assert_checkequal(p, ref);
p = atomsGetInstalledPath("ToolBox_7v6");   // else: case-insensitive one
assert_checkequal(p, ref);
p = atomsGetInstalledPath("TOOLBOX_7");     // else: case-insensitive fragment
assert_checkequal(p, ref);
p = atomsGetInstalledPath("toolbox_7", "all");
assert_checkequal(p, ref);
p = atomsGetInstalledPath("toolbox_7", "allusers");
assert_checkequal(p, "");                         // wrong section
p = atomsGetInstalledPath(["toolbox_7","2"]);   // wrong version
assert_checkequal(p, "");
p = atomsGetInstalledPath(["toolbox_7","1"]);   // wrong version
assert_checkequal(p, "");
p = atomsGetInstalledPath(["toolbox_7","1.0"]); // version found
assert_checkequal(p, ref);

atomsRemove("toolbox_7V6","user");
atomsSetConfig("offline", offlineStatus);
