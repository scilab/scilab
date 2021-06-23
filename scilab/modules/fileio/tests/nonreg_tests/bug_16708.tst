// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 16708 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16708
//
// <-- Short Description -->
// mgetl cannot read from stdin

if (getos() <> "Windows") & ~isfile(SCI+"/bin/scilab") then
    SCI_BIN = strsubst(SCI,"|/share/scilab/?$|","","r");
else
    SCI_BIN = SCI;
end

cmd = msprintf("echo success | %s/bin/scilab -ns -nwni -nb -e %s", SCI_BIN, """str=mgetl(%io(1));mprintf(str)""");
result = unix_g(cmd);
assert_checkequal(result,"success")