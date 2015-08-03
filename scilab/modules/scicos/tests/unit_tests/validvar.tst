// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();

FORBIDDEN_CHARS = [" ","*","/","\",".",",",";",":","^","@",">","<","=","+","-","&","|","(",")","~","\n","\t","''",""""];
for i = 1:size(FORBIDDEN_CHARS, "*")
    assert_checkfalse(validvar("test" + FORBIDDEN_CHARS(i) + "test"));
end

FORBIDDEN_CHARS = ["0","1","2","3","4","5","6","7","8","9"];

for i = 1:size(FORBIDDEN_CHARS, "*")
    assert_checkfalse(validvar(FORBIDDEN_CHARS(i) + "test"));
    assert_checktrue(validvar("test" + FORBIDDEN_CHARS(i)));
end
